
//void Setup(AliReducedAnalysisJpsi2ee* processor, TString prod="LHC10h");
//AliHistogramManager* SetupHistogramManager(TString prod="LHC10h");
//void DefineHistograms(AliHistogramManager* man, TString prod="LHC10h");

//__________________________________________________________________________________________
AliAnalysisTask* AddTask_llayer_EvSel_RAA_Finalv2_jpsi2ee(Bool_t isAliRoot=kTRUE, Int_t runMode=1, TString prod="LHC10h"){    
   //
   // isAliRoot=kTRUE for ESD/AOD analysis in AliROOT, kFALSE for root analysis on reduced trees
   // runMode=1 (AliAnalysisTaskReducedEventProcessor::kUseOnTheFlyReducedEvents)
   //               =2 (AliAnalysisTaskReducedEventProcessor::kUseEventsFromTree)
   //
   //get the current analysis manager

  
  Bool_t isMCAnalysis = kTRUE;
  cout << "Is MC: " << isMCAnalysis << endl;
  
  printf("INFO on AddTask_llayer_ EvSel_jpsi2ee(): (isAliRoot, runMode) :: (%d,%d) \n", isAliRoot, runMode);

  AliReducedAnalysisJpsi2ee* jpsi2eeAnalysis = new AliReducedAnalysisJpsi2ee("Jpsi2eeAnalysis","Jpsi->ee analysis");
  jpsi2eeAnalysis->Init();
  Setup(jpsi2eeAnalysis, prod);
  
  if (!isMCAnalysis){ 
    jpsi2eeAnalysis->SetRunEventMixing(kTRUE); 
    jpsi2eeAnalysis->SetRunOverMC(kFALSE);///NOT MC ANALYSIS!!!!!!!!!!!!!!111111
  } else { 
    jpsi2eeAnalysis->SetRunEventMixing(kFALSE); 
    jpsi2eeAnalysis->SetRunOverMC(kTRUE); 
  }
  
  // initialize an AliAnalysisTask which will wrapp the AliReducedAnalysisJpsi2ee such that it can be run in an aliroot analysis train (e.g. LEGO, local analysis etc)
  AliAnalysisTaskReducedEventProcessor* task = new AliAnalysisTaskReducedEventProcessor("ReducedEventAnalysisManager", runMode);
  task->AddTask(jpsi2eeAnalysis);
  
  if(isAliRoot){
     AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
     if (!mgr) {
        Error("AddTask_llayer_EvSel_dst", "No analysis manager found.");
        return 0;
     }
     
     AliAnalysisDataContainer* cReducedEvent = NULL;
     if(runMode==AliAnalysisTaskReducedEventProcessor::kUseOnTheFlyReducedEvents) {
       printf("INFO on AddTask_llayer_jpsi2ee(): use on the fly events ");
       cReducedEvent = (AliAnalysisDataContainer*)mgr->GetContainers()->FindObject("ReducedEventDQ");
       if(!cReducedEvent) {
         printf("ERROR: In AddTask_llayer_jpsi2ee(), couldn't find exchange container with ReducedEvent! ");
         printf("             You need to use AddTask_llayer_dst() in the on-the-fly reduced events mode!");
         return 0x0;
       }
     }
            
     mgr->AddTask(task);
      
     if(runMode==AliAnalysisTaskReducedEventProcessor::kUseEventsFromTree) 
        mgr->ConnectInput(task,  0, mgr->GetCommonInputContainer());
      
     if(runMode==AliAnalysisTaskReducedEventProcessor::kUseOnTheFlyReducedEvents) 
       mgr->ConnectInput(task, 0, cReducedEvent);
  
     AliAnalysisDataContainer *cOutputHist = mgr->CreateContainer("jpsi2eeHistos", THashList::Class(),
                                                                  AliAnalysisManager::kOutputContainer, "Jpsi_EvSel.root");
     mgr->ConnectOutput(task, 1, cOutputHist );
     
     AliAnalysisDataContainer *cOutputTree = mgr->CreateContainer("jpsi2eeTree", TTree::Class(),
                                                                  AliAnalysisManager::kOutputContainer, "Jpsi_EvSel.root");
     mgr->ConnectOutput (task,  2, cOutputTree);
     
  }
  else {
    // nothing at the moment   
  }
  
  return task;
}


//_________________________________________________________________
void Setup(AliReducedAnalysisJpsi2ee* processor, TString prod /*="LHC10h"*/) {
  //
  // Configure the analysis task
  // Setup histograms, handlers, cuts, etc.
  //
  if(prod.Contains("LHC15o_highIR")){
    //corection maps
    TFile* corrFile = TFile::Open("/lustre/nyx/alice/users/rjimenez/software/sw/ubuntu1404_x86-64/AliPhysics/ali-dev-1/PWGDQ/reducedTree/macros/CorrMaps_CentralityNew_LHC15o_pass1.root"); // Change path to the correct ones
    AliReducedVarManager::SetTPCelectronCorrectionMaps((TH2F*)corrFile->Get("CorrMapCentroid"), (TH2F*)corrFile->Get("CorrMapWidth"), AliReducedVarManager::kEta, AliReducedVarManager::kCentVZERO);
    corrFile->Close();
    //event plane correction maps and event planes
    AliReducedVarManager::SetVZEROCalibrationPath("/lustre/nyx/alice/users/rjimenez/software/sw/ubuntu1404_x86-64/AliPhysics/ali-dev-1/PWGDQ/reducedTree/macros/filesEventPlane/DQ_PbPb_LEGO287/analysisOutputs/2017_01_09_VZEROepRecentering");
    AliReducedVarManager::SetCalibrateVZEROqVector(kTRUE); 
    AliReducedVarManager::SetRecenterVZEROqVector(kTRUE);
  }
//if(prod.Contains("LHC15o_pidfix")){
  if(prod.Contains("LHC15o_pidfix") || prod.Contains("LHC15o_lowIR")) {
    //corection maps
    TFile* corrFile = TFile::Open("/lustre/nyx/alice/users/rjimenez/software/sw/ubuntu1404_x86-64/AliPhysics/ali-dev-1/PWGDQ/reducedTree/macros/CorrMaps_CentralityNew_LHC15o_pass1_pidfix.root");
    AliReducedVarManager::SetTPCelectronCorrectionMaps((TH2F*)corrFile->Get("CorrMapCentroid"), (TH2F*)corrFile->Get("CorrMapWidth"), AliReducedVarManager::kEta, AliReducedVarManager::kCentVZERO);
    corrFile->Close();
  }
  if(prod.Contains("LHC15o_pidfix")) {
    //event plane
    AliReducedVarManager::SetVZEROCalibrationPath("/lustre/nyx/alice/users/rjimenez/software/sw/ubuntu1404_x86-64/AliPhysics/ali-dev-1/PWGDQ/reducedTree/macros/filesEventPlane/DQ_PbPb_LEGO288_pidfix/analysisOutputs/2017_01_09_VZEROepRecentering");
    AliReducedVarManager::SetCalibrateVZEROqVector(kTRUE); 
    AliReducedVarManager::SetRecenterVZEROqVector(kTRUE);
  }    

  // Set event cuts
  AliReducedEventCut* evCut1 = new AliReducedEventCut("Centrality","Centrality selection");
  evCut1->AddCut(AliReducedVarManager::kCentVZERO, 0., 90.);
  evCut1->AddCut(AliReducedVarManager::kVtxZ, -10.0, 10.0);
  evCut1->AddCut(AliReducedVarManager::kIsPhysicsSelection, 1.0, 2.);   // request physics selection
  evCut1->EnableVertexDistanceCut();
  
  if(prod.Contains("LHC15o")) {
    TF1* cutCorrTPCoutVZEROmult = new TF1("cutCorrTPCoutVZEROmult", "[0]+[1]*x+[2]*x*x", 0., 1.e+5);  // NOT USE IT FOR MC
    //ionuts params     cutCorrTPCoutVZEROmult->SetParameters(-4900., 2.9);    
    cutCorrTPCoutVZEROmult->SetParameters(-2200., 2.5,1.2e-5);
    // params lhc15opidfix
    //cutCorrTPCoutVZEROmult->SetParameters(-1000., 2.6);
    evCut1->AddCut(AliReducedVarManager::kVZEROTotalMult, cutCorrTPCoutVZEROmult, 99999., kFALSE, AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout, 0., 99998.);
    

  }

  processor->AddEventCut(evCut1);
  cout<< "Set the event cut!" << endl; 
   
   
  // Set track cuts
    AliReducedTrackCut* trackCut1 = new AliReducedTrackCut("standard","standard pt>1.0");
  trackCut1->AddCut(AliReducedVarManager::kPt, 1.0,99.0);
  trackCut1->AddCut(AliReducedVarManager::kEta, -0.9,0.9);
  trackCut1->AddCut(AliReducedVarManager::kDcaXY, -1.0,1.0);
  trackCut1->AddCut(AliReducedVarManager::kDcaZ, -3.0,3.0);
  trackCut1->AddCut(AliReducedVarManager::kTPCncls, 70.,160.0);
  trackCut1->AddCut(AliReducedVarManager::kTPCnSig+AliReducedVarManager::kProton, 3.5, 1.0e+30);
  trackCut1->AddCut(AliReducedVarManager::kTPCnSig+AliReducedVarManager::kPion, 3.5, 1.0e+30);
  if (isMCAnalysis == true){
    trackCut1->AddCut(AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron, -3.0, 3.0);
  }else{
    trackCut1->AddCut(AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, -3.0, 3.0);
  }
  trackCut1->AddCut(AliReducedVarManager::kTPCNclusBitsFired, 7.0, 9.0);
  trackCut1->SetRejectKinks();
  trackCut1->SetRequestITSrefit();
  trackCut1->SetRequestTPCrefit();
  trackCut1->SetRequestSPDany();  
  trackCut1->AddCut(AliReducedVarManager::kITSchi2, 0., 10.);
  trackCut1->AddCut(AliReducedVarManager::kNclsSFracITS, 0.0., 0.4);
  trackCut1->AddCut(AliReducedVarManager::kTPCchi2, 0.1, 2.5, kFALSE, AliReducedVarManager::kCentVZERO, 0.0, 90.0);
  processor->AddTrackCut(trackCut1);


    
  // set track prefilter cut
  //AliReducedTrackCut* prefTrackCut1 = new AliReducedTrackCut("prefCutPt09","prefilter P selection");
  //prefTrackCut1->AddCut(AliReducedVarManager::kP, 0.9,100.0);
  //prefTrackCut1->SetRequestTPCrefit();
  processor->AddPrefilterTrackCut(trackCut1);  
  
  // set pair prefilter cuts
  AliReducedVarCut* prefPairCut = new AliReducedVarCut("prefCutM50MeV","prefilter pair cuts");
  prefPairCut->AddCut(AliReducedVarManager::kMass, 0.0, 0.05, kTRUE);
  processor->AddPrefilterPairCut(prefPairCut);
  
  // Set pair cuts
  AliReducedTrackCut* pairCut1 = new AliReducedTrackCut("Ptpair","Pt pair selection");
  pairCut1->AddCut(AliReducedVarManager::kPt, 0.0,100.0);
  //pairCut1->AddCut(AliReducedVarManager::kEta, -1.5,1.5);
  //processor->AddPairCut(pairCut1);
  
  //SetupHistogramManager(processor->GetHistogramManager(), prod);
  
  SetupHistogramManager(processor, prod);   // MC out Dennis??
  SetupMixingHandler(processor);
}


//_________________________________________________________________
void SetupMixingHandler(AliReducedAnalysisJpsi2ee* task) {
   //
   // setup the mixing handler
   //
   AliMixingHandler* handler = task->GetMixingHandler();
   handler->SetPoolDepth(20);
   handler->SetMixingThreshold(1.0);
   handler->SetDownscaleEvents(1);
   handler->SetDownscaleTracks(1);
    handler->SetEventVariables(AliReducedVarManager::kCentVZERO,AliReducedVarManager::kVtxZ,AliReducedVarManager::kVZERORP+6+1);
    Float_t centLims[13] = {0.0, 1.0, 2.0, 10.0,15.0, 20.0, 30.0, 40., 50., 60.,70.,80.,90.};
    /*10.0, 12.0, 14.0, 16.0, 18.0, 
    20.0, 25.0, 30.0, 35.0, 40.0, 
    45.0, 50.0, 60.0, 70.0, 90.0};*/
   Float_t zLims[9] = {-10.,-7.,-4.,-2.,0.,2.,4.,7.,10.};
   
    //Float_t timeRelativeSORLims[45] = {0.0,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,110.,120.,130.,140.,150.,160.,170.,180.,190.,200.,210.,220.,230.,240.,250.,260.,270.,280.,290.,300.,310.,320.,330.,340.,350.,360.,370.,380.,390.,400.,410.,420.,430.,440.};

//    Float_t zLims[2] = {-10.,10.};
  // Float_t epLims[9]; //= {0.,1.,2.};
//for(Int_t i=0;i<=8;++i) epLims[i] = -0.5*TMath::Pi()+i*TMath::Pi()/8.;
  Float_t epLims[9];
   epLims[0] = -4.0*TMath::Pi()/8.;
   epLims[1] = -3.0*TMath::Pi()/8.;
   epLims[2] = -2.0*TMath::Pi()/8.;
   epLims[3] = -1.0*TMath::Pi()/8.;
   epLims[4] = -0.0*TMath::Pi()/8.;
   epLims[5] = 1.0*TMath::Pi()/8.;
   epLims[6] = 2.0*TMath::Pi()/8.;
   epLims[7] = 3.0*TMath::Pi()/8.;
   epLims[8] = 4.0*TMath::Pi()/8.;


   
   
   handler->SetCentralityLimits(13,centLims);
   handler->SetEventVertexLimits(9,zLims);
   handler->SetEventPlaneLimits(9,epLims); 
    
   //Float_t timeLims[225];
  // for(Int_t i=0;i<225;++i) timeLims[i] = 0.0+i*2;
  // Float_t epLims[2] = {-10000.,10000.};
  // handler->SetCentralityLimits(3,centLims);
  // handler->SetEventVertexLimits(225,timeLims);
   //handler->SetEventPlaneLimits(2,epLims);
}


//_________________________________________________________________
void SetupHistogramManager(AliReducedAnalysisJpsi2ee* task, TString prod /*="LHC10h"*/) {
  //
  // setup the histograms manager
  //
  AliReducedVarManager::SetDefaultVarNames();
  
  DefineHistograms(task, prod);
  
  AliReducedVarManager::SetUseVars(task->GetHistogramManager()->GetUsedVars());
}


//_________________________________________________________________
void DefineHistograms(AliReducedAnalysisJpsi2ee* task, TString prod /*="LHC10h"*/) {
  //
  // define histograms
  // NOTE: The DefineHistograms need to be called after the track cuts are defined because the name of the cuts
  //           are used in the histogram lists
   // NOTE: The name of the pair histogram lists for event mixing need to contain "PairMEPP", "PairMEPM", "PairMEMM", in their name, see below.
   //  TODO: make needed changes such that this becomes less prone to mistakes
   
  AliHistogramManager* man = task->GetHistogramManager(); 
   
  TString histClasses = "";
  //  histClasses += "Event_BeforeCuts;";
  histClasses += "Event_AfterCuts;";   
  histClasses += "EventTag_BeforeCuts;";   
  histClasses += "EventTag_AfterCuts;";   
  histClasses += "EventTriggers_BeforeCuts;";
  histClasses += "EventTriggers_AfterCuts;";   
  // histClasses += "Track_BeforeCuts;";
//  histClasses += "TrackStatusFlags_BeforeCuts;";
//  histClasses += "TrackITSclusterMap_BeforeCuts;";
//  histClasses += "TrackTPCclusterMap_BeforeCuts;";
  for(Int_t i=0; i<task->GetNTrackCuts(); ++i) {
    TString cutName = task->GetTrackCutName(i);
    histClasses += Form("Track_%s;", cutName.Data());
   // histClasses += Form("TrackStatusFlags_%s;", cutName.Data());
   // histClasses += Form("TrackITSclusterMap_%s;", cutName.Data());
   // histClasses += Form("TrackTPCclusterMap_%s;", cutName.Data());
    histClasses += Form("PairSEPP_%s;PairSEPM_%s;PairSEMM_%s;", cutName.Data(), cutName.Data(), cutName.Data());
    histClasses += Form("PairMEPP_%s;PairMEPM_%s;PairMEMM_%s;", cutName.Data(), cutName.Data(), cutName.Data());
  }
  
  Int_t runNBins = 0;//{-10.,-5.,-4.,-3.,-2.,-1,0.,1.,2.,3.,4.,5.,10.}
  Double_t runHistRange[2] = {0.0,0.0};
  
  // Pb-Pb from 2010 run range is default: LHC10h
  runNBins = 2500;
  runHistRange[0] = 137100.;
  runHistRange[1] = 139600.;
  
  // Pb-Pb of 2011
  if(!prod.CompareTo("LHC11h")) {
    runNBins = 2700;
    runHistRange[0] = 167900.;
    runHistRange[1] = 170600.;
  }
  
  // Pb-Pb of 2015
    if(prod.Contains("LHC15o")) {
      //  if(!prod.CompareTo("LHC15o_highIR") ||!prod.CompareTo("LHC15o_pidfix")  ) {
     runNBins = 2100;
     runHistRange[0] = 244900.;
     runHistRange[1] = 247000.;
  }
  
  // p-Pb of 2013
  if(!prod.CompareTo("LHC13b") || !prod.CompareTo("LHC13c")) {
    runNBins = 400;
    runHistRange[0] = 195300.;
    runHistRange[1] = 195700.;
  }
  
  // pp at 13 TeV
  if(!prod.CompareTo("LHC16l")) {
     runNBins = 1140;
     runHistRange[0] = 258880.;
     runHistRange[1] = 260020.;
  }
  
  const Int_t kNBCBins = 3600;
  Double_t bcHistRange[2] = {-0.5,3599.5};
  
  TString classesStr(histClasses);
  TObjArray* arr=classesStr.Tokenize(";");
  
  cout << "Histogram classes included in the Histogram Manager" << endl;
  for(Int_t iclass=0; iclass<arr->GetEntries(); ++iclass) {
    TString classStr = arr->At(iclass)->GetName();
    
    // Event wise histograms
    if(classStr.Contains("EventTag_")) {
       man->AddHistClass(classStr.Data());
       cout << classStr.Data() << endl;
       TString tagNames = "";
       tagNames += "AliAnalysisUtils 2013 selection;";
       tagNames += "AliAnalysisUtils MV pileup;";
       tagNames += "AliAnalysisUtils MV pileup, no BC check;";
       tagNames += "AliAnalysisUtils MV pileup, min wght dist 10;";
       tagNames += "AliAnalysisUtils MV pileup, min wght dist 5;";
       tagNames += "IsPileupFromSPD(3,0.6,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(4,0.6,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(5,0.6,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(6,0.6,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(3,0.8,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(4,0.8,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(5,0.8,3.,2.,5.);";
       tagNames += "IsPileupFromSPD(6,0.8,3.,2.,5.);";
       tagNames += "vtx distance selected;";
       man->AddHistogram(classStr.Data(), "EventTags", "Event tags", kFALSE,
                         20, -0.5, 19.5, AliReducedVarManager::kEventTag, 0, 0.0, 0.0, AliReducedVarManager::kNothing, 0, 0.0, 0.0, AliReducedVarManager::kNothing, tagNames.Data());
       man->AddHistogram(classStr.Data(), "EventTags_CentVZERO", "Event tags vs VZERO centrality", kFALSE,
                         20, -0.5, 19.5, AliReducedVarManager::kEventTag, 9, 0.0, 90.0, AliReducedVarManager::kCentVZERO, 0, 0.0, 0.0, AliReducedVarManager::kNothing, tagNames.Data());
       continue;
    }
    
    if(classStr.Contains("EventTriggers_")) {
       man->AddHistClass(classStr.Data());
       cout << classStr.Data() << endl;
       TString triggerNames = "";
       for(Int_t i=0; i<64; ++i) {triggerNames += AliReducedVarManager::fgkOfflineTriggerNames[i]; triggerNames+=";";}
       
       man->AddHistogram(classStr.Data(), "Triggers2", "", kFALSE,
                         64, -0.5, 63.5, AliReducedVarManager::kOnlineTriggerFired2, 0, 0.0, 0.0, AliReducedVarManager::kNothing, 0, 0.0, 0.0, AliReducedVarManager::kNothing, triggerNames.Data());
       man->AddHistogram(classStr.Data(), "CentVZERO_Triggers2", "", kFALSE,
                         64, -0.5, 63.5, AliReducedVarManager::kOnlineTriggerFired2, 9, 0.0, 90.0, AliReducedVarManager::kCentVZERO, 0, 0.0, 0.0, AliReducedVarManager::kNothing, triggerNames.Data());
       continue;
    }
    
    if(classStr.Contains("Event_")) {
      man->AddHistClass(classStr.Data());
      cout << classStr.Data() << endl;
      man->AddHistogram(classStr.Data(),"RunNo","Run numbers",kFALSE, runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo);
   //   man->AddHistogram(classStr.Data(),"TimeFromSOR","Events vs time",kFALSE, 450, 0., 450., AliReducedVarManager::kTimeRelativeSOR);
      man->AddHistogram(classStr.Data(),"VtxX","Vtx X",kFALSE,300,-0.4,0.4,AliReducedVarManager::kVtxX);
    // man->AddHistogram(classStr.Data(),"VtxX_TimeFromSOR_prof","<Vtx X> vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 300,-0.4,0.4,AliReducedVarManager::kVtxX);
      man->AddHistogram(classStr.Data(),"VtxY","Vtx Y",kFALSE,300,-0.4,0.4,AliReducedVarManager::kVtxY);
    //  man->AddHistogram(classStr.Data(),"VtxY_TimeFromSOR_prof","<Vtx Y> vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 300,-0.4,0.4,AliReducedVarManager::kVtxY);
      man->AddHistogram(classStr.Data(),"VtxZ","Vtx Z",kFALSE,300,-15.,15.,AliReducedVarManager::kVtxZ);
      man->AddHistogram(classStr.Data(),"EventPlane2ndHarmonic","EventPlane2ndHarmonic",kFALSE,100,-4.0.,4.0,AliReducedVarManager::kVZERORP+6+1);
      //   man->AddHistogram(classStr.Data(),"VtxZ_TimeFromSOR_prof","<Vtx Z> vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 300,-15.0,15.0,AliReducedVarManager::kVtxZ);
      man->AddHistogram(classStr.Data(),"CentVZERO","Centrality(VZERO)",kFALSE, 100, 0.0, 100.0, AliReducedVarManager::kCentVZERO);
    man->AddHistogram(classStr.Data(),"CentVZERO_VtxZ","Centrality(VZERO) vs vtxZ",kFALSE, 50, 0.0, 100.0, AliReducedVarManager::kCentVZERO,
         50, -10., 10., AliReducedVarManager::kVtxZ);
    
    
  //    man->AddHistogram(classStr.Data(),"CentVZERO_TimeFromSOR","Centrality(VZERO) vs time from SOR",kFALSE,
    //                    90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 50, 0.0, 100.0, AliReducedVarManager::kCentVZERO);
     // man->AddHistogram(classStr.Data(),"CentSPD","Centrality(SPD)",kFALSE, 100, 0.0, 100.0, AliReducedVarManager::kCentSPD);
     // man->AddHistogram(classStr.Data(),"CentSPD_VtxZ","Centrality(SPD) vs vtxZ",kFALSE, 50, 0.0, 100.0, AliReducedVarManager::kCentSPD,
        // 50, -10., 10., AliReducedVarManager::kVtxZ);
    //  man->AddHistogram(classStr.Data(),"CentTPC","Centrality(TPC)",kFALSE, 100, 0.0, 100.0, AliReducedVarManager::kCentTPC);
    //  man->AddHistogram(classStr.Data(),"CentTPC_VtxZ","Centrality(TPC) vs vtxZ",kFALSE, 100, 0.0, 100.0, AliReducedVarManager::kCentTPC,
      //   50, -10., 10., AliReducedVarManager::kVtxZ);

      man->AddHistogram(classStr.Data(),"CentQuality","Centrality quality",kFALSE, 500, 0., 500., AliReducedVarManager::kCentQuality);
//      man->AddHistogram(classStr.Data(),"IRIntClosestInt1Map","Closest out of bunch interactions Int1",kFALSE,7000,-3500.,3500.,AliReducedVarManager::kIRIntClosestIntMap);
    //  man->AddHistogram(classStr.Data(),"IRIntClosestInt2Map","Closest out of bunch interactions Int2",kFALSE,7000,-3500.,3500.,AliReducedVarManager::kIRIntClosestIntMap+1);
      man->AddHistogram(classStr.Data(),"NTracksTotal","Number of total tracks per event",kFALSE,500,0.,30000.,AliReducedVarManager::kNtracksTotal);
      man->AddHistogram(classStr.Data(),"NTracksTotal_BeamIntensity0_prof","Number of total tracks per event",kTRUE,100, 3.0e+12, 9.0e+12, AliReducedVarManager::kBeamIntensity0, 500,0.,20000.,AliReducedVarManager::kNtracksTotal);
      man->AddHistogram(classStr.Data(),"NTracksSelected","Number of selected tracks per event",kFALSE,300,0.,300.,AliReducedVarManager::kNtracksSelected);
     // man->AddHistogram(classStr.Data(),"NTracksSelected_TimeFromSOR","Averaged number of selected tracks per event vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,100.,AliReducedVarManager::kNtracksSelected);
     // man->AddHistogram(classStr.Data(),"NTracksSelected_CentVZERO_TimeFromSOR","Averaged number of selected tracks per event per centrality vs time from SOR",kTRUE, 20, 0.0, 100.0, AliReducedVarManager::kCentVZERO, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,100.,AliReducedVarManager::kNtracksSelected);
      man->AddHistogram(classStr.Data(),"DeltaVtxZ","Z_{global}-Z_{TPC}",kFALSE,300,-1.,1.,AliReducedVarManager::kDeltaVtxZ);
        

	    
	    
     // man->AddHistogram(classStr.Data(),"DeltaVtxZ_TimeFromSOR_prof","Z_{global}-Z_{TPC} vs time from SOR",kTRUE,90, 0.0, 450.,  AliReducedVarManager::kTimeRelativeSOR, 300,-1.,1.,AliReducedVarManager::kDeltaVtxZ);
      man->AddHistogram(classStr.Data(),"NVtxTPCContributors","",kFALSE,500,0.,10000.,AliReducedVarManager::kNVtxTPCContributors);
 //     man->AddHistogram(classStr.Data(),"NVtxTPCContributors_BeamIntensity0","",kTRUE, 100, 3.0e+12, 9.0e+12, AliReducedVarManager::kBeamIntensity0, 500,0.,10000.,AliReducedVarManager::kNVtxTPCContributors);
 //     man->AddHistogram(classStr.Data(),"SPDntracklets", "SPD #tracklets in |#eta|<1.0", kFALSE, 200, 0., 5000., AliReducedVarManager::kSPDntracklets);
      
      
    //  man->AddHistogram(classStr.Data(),"SPDntracklets_TimeFromSOR_prof", "SPD <#tracklets> in |#eta|<1.0 vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200, 0., 5000., AliReducedVarManager::kSPDntracklets);
      for(Int_t il=0; il<2; ++il) {
        man->AddHistogram(classStr.Data(), Form("SPDfiredChips_layer%d",il+1), Form("SPD fired chips in layer %d",il+1), 
			  kFALSE, 200, 0., 1000., AliReducedVarManager::kSPDFiredChips+il);
       // man->AddHistogram(classStr.Data(), Form("SPDfiredChips_layer%d_TimeFromSOR_prof",il+1), Form("SPD <#fired chips> in layer %d vs time from SOR",il+1), 
         //                 kTRUE, 90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR, 200, 0., 1000., AliReducedVarManager::kSPDFiredChips+il);
      }
      for(Int_t il=0; il<6; ++il) {
        man->AddHistogram(classStr.Data(), Form("ITSclusters_layer%d",il+1), Form("ITS clusters in layer %d",il+1), 
			  kFALSE, 300, 0., 15000., AliReducedVarManager::kITSnClusters+il);
    //    man->AddHistogram(classStr.Data(), Form("ITSclusters_layer%d_TimeFromSOR_prof",il+1), Form("ITS <clusters> in layer %d vs time from SOR",il+1), 
      //                    kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 300, 0., 15000., AliReducedVarManager::kITSnClusters+il);
      }
      man->AddHistogram(classStr.Data(), "SPDnSingleClusters", "SPD single clusters", 
			kFALSE, 200, 0., 10000., AliReducedVarManager::kSPDnSingleClusters);	
    //  man->AddHistogram(classStr.Data(), "SPDnSingleClusters_TimeFromSOR_prof", "SPD single <#clusters> vs time from SOR", 
      //                  kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200, 0., 10000., AliReducedVarManager::kSPDnSingleClusters);	
      man->AddHistogram(classStr.Data(), "SPDnSingleClusters_BeamInt0_prof", "SPD single <#clusters> vs beam intensity", 
                        kTRUE, 100, 3.0e+12, 9.0e+12., AliReducedVarManager::kBeamIntensity0, 200, 0., 10000., AliReducedVarManager::kSPDnSingleClusters);	
      man->AddHistogram(classStr.Data(),"VZEROmult", "", kFALSE, 500, 0.0, 50000., AliReducedVarManager::kVZEROTotalMult);
      man->AddHistogram(classStr.Data(),"VZEROmult_VtxContributors", "", kFALSE, 
		   200, 0.0, 40000., AliReducedVarManager::kVZEROTotalMult, 200, 0.0, 10000., AliReducedVarManager::kNVtxContributors);
        man->AddHistogram(classStr.Data(),"NTPCVtxContibutors_VtxContributors", "", kFALSE, 
		   200, 0.0, 10000., AliReducedVarManager::kNVtxTPCContributors, 200, 0.0, 10000., AliReducedVarManager::kNVtxContributors);
      man->AddHistogram(classStr.Data(),"VZEROmult_SPDntracklets", "", kFALSE, 
                        200, 0.0, 5000., AliReducedVarManager::kSPDntracklets, 200, 0.0, 40000., AliReducedVarManager::kVZEROTotalMult);
      man->AddHistogram(classStr.Data(),"IsPhysicsSelection","Physics selection flag",kFALSE,
                        2,-0.5,1.5,AliReducedVarManager::kIsPhysicsSelection, 0,0.0,0.0,AliReducedVarManager::kNothing, 0,0.0,0.0,AliReducedVarManager::kNothing, "off;on");
  //    man->AddHistogram(classStr.Data(),"VZEROmult_VtxContributors", "", kFALSE,
//200, 0.0, 10000., AliReducedVarManager::kNVtxTPCContributors, 200, 0.0, 10000., AliReducedVarManager::kNVtxContributors);
    //  man->AddHistogram(classStr.Data(),"IsPhysicsSelection_TimeFromSOR_prof","Fraction of physics selection selected",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, //2,-0.5,1.5,AliReducedVarManager::kIsPhysicsSelection);
      man->AddHistogram(classStr.Data(),"NPosTracksAnalyzed","#positrons per event",kFALSE,100,0.,100.,AliReducedVarManager::kNtracksPosAnalyzed);
      man->AddHistogram(classStr.Data(),"NNegTracksAnalyzed","#electrons per event",kFALSE,100,0.,100.,AliReducedVarManager::kNtracksNegAnalyzed);
      man->AddHistogram(classStr.Data(),"NTotalTracksAnalyzed","#leg candidates per event",kFALSE,100,0.,100.,AliReducedVarManager::kNtracksAnalyzed);
//      man->AddHistogram(classStr.Data(),"NTotalPairsAnalyzed","#dielectron candidates per event",kFALSE,100,0.,100.,AliReducedVarManager::kNpairsSelected);
  /*    man->AddHistogram(classStr.Data(),"NPosTracksAnalyzed_TimeFromSOR_prof","<#positrons> per event vs time from SOR",kTRUE,90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,100.,AliReducedVarManager::kNtracksPosAnalyzed);
   //   man->AddHistogram(classStr.Data(),"NNegTracksAnalyzed_TimeFromSOR_prof","<#electrons> per event vs time from SOR",kTRUE,90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR,100,0.,100.,AliReducedVarManager::kNtracksNegAnalyzed);
    //  man->AddHistogram(classStr.Data(),"NTotalTracksAnalyzed_TimeFromSOR_prof","<#leg candidates> per event vs time from SOR",kTRUE,90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR,100,0.,100.,AliReducedVarManager::kNtracksAnalyzed);
      man->AddHistogram(classStr.Data(),"NTotalTracksAnalyzed_BeamInt0_prof","<#leg candidates> per event vs beam intensity",kTRUE,100, 3.0e+12, 9.0e+12, AliReducedVarManager::kBeamIntensity0,100,0.,100.,AliReducedVarManager::kNtracksAnalyzed);
     // man->AddHistogram(classStr.Data(),"NTotalPairsAnalyzed_TimeFromSOR_prof","<#dielectron> candidates per event vs time from SOR",kTRUE,90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR,100,0.,100.,AliReducedVarManager::kNpairsSelected);
      man->AddHistogram(classStr.Data(),"NTotalPairsAnalyzed_BeamInt0_prof","<#dielectron> candidates vs beam intensity",kTRUE,100, 3.0e+12, 9.0e+12, AliReducedVarManager::kBeamIntensity0,100,0.,100.,AliReducedVarManager::kNpairsSelected);
      
      for(Int_t i=0;i<36;++i)
         man->AddHistogram(classStr.Data(),Form("NTotalTracksAnalyzedInPhiBins_phiSec%d_%s_TimeFromSOR_prof", i%18, (i<18 ? "negEta" : "posEta")), Form("<#leg candidates> per event in #varphi sector %d and %s vs time from SOR", i%18, (i<18 ? "negative #eta" : "positive #eta")),kTRUE,90, 0.0, 450., AliReducedVarManager::kTimeRelativeSOR,100,0.,100.,AliReducedVarManager::kNtracksAnalyzedInPhiBins+i);
      
      man->AddHistogram(classStr.Data(),"TZEROpileup", "TZERO pileup", kFALSE, 2, -0.5, 1.5, AliReducedVarManager::kTZEROpileup);
      man->AddHistogram(classStr.Data(),"TZEROpileup_TimeFromSOR_prof", "TZERO pileup vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 2, -0.5, 1.5, AliReducedVarManager::kTZEROpileup);
      man->AddHistogram(classStr.Data(),"TZEROsatellite", "TZERO satellite", kFALSE, 2, -0.5, 1.5, AliReducedVarManager::kTZEROsatellite);
      man->AddHistogram(classStr.Data(),"TZEROsatellite_TimeFromSOR_prof", "TZERO satellite vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 2, -0.5, 1.5, AliReducedVarManager::kTZEROsatellite);
      
      man->AddHistogram(classStr.Data(),"
      NTracksTPCoutVsITSout","",kFALSE,100,0.,15.,AliReducedVarManager::kNTracksTPCoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTPCoutVsITSout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTPCoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsITSout","",kFALSE,100,0.,5.,AliReducedVarManager::kNTracksTRDoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsITSout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTRDoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsITSout","",kFALSE,100,0.,1.,AliReducedVarManager::kNTracksTOFoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsITSout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTOFoutVsITSout);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsTPCout","",kFALSE,100,0.,1.,AliReducedVarManager::kNTracksTRDoutVsTPCout);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsTPCout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTRDoutVsTPCout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsTPCout","",kFALSE,100,0.,0.6,AliReducedVarManager::kNTracksTOFoutVsTPCout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsTPCout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTOFoutVsTPCout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsTRDout","",kFALSE,100,0.,2.,AliReducedVarManager::kNTracksTOFoutVsTRDout);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsTRDout_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTOFoutVsTRDout);
      man->AddHistogram(classStr.Data(),"NTracksITSoutVsSPDtracklets","",kFALSE,100,0.,50.,AliReducedVarManager::kNTracksITSoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksITSoutVsSPDtracklets_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksITSoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTPCoutVsSPDtracklets","",kFALSE,100,0.,50.,AliReducedVarManager::kNTracksTPCoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTPCoutVsSPDtracklets_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTPCoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsSPDtracklets","",kFALSE,100,0.,30.,AliReducedVarManager::kNTracksTRDoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTRDoutVsSPDtracklets_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTRDoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsSPDtracklets","",kFALSE,100,0.,10.,AliReducedVarManager::kNTracksTOFoutVsSPDtracklets);
      man->AddHistogram(classStr.Data(),"NTracksTOFoutVsSPDtracklets_TimeFromSOR_prof","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,15.,AliReducedVarManager::kNTracksTOFoutVsSPDtracklets);
      */
   //   man->AddHistogram(classStr.Data(),"NTracksITSin","",kFALSE,300,0.,15000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kITSin);
   //   man->AddHistogram(classStr.Data(),"NTracksITSout","",kFALSE,300,0.,15000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kITSout);
   //   man->AddHistogram(classStr.Data(),"NTracksTPCin","",kFALSE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCin);
  //    man->AddHistogram(classStr.Data(),"NTracksTPCout","",kFALSE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout);
   //   man->AddHistogram(classStr.Data(),"NTracksTRDout","",kFALSE,300,0.,15000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTRDout);
   //   man->AddHistogram(classStr.Data(),"NTracksTRDin","",kFALSE,300,0.,15000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTRDin);
      man->AddHistogram(classStr.Data(),"NTracksTPCout_VZEROmult","",kFALSE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);

    man->AddHistogram(classStr.Data(),"NTracksTPCout_VtxZ","",kFALSE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout,20,-10,10,AliReducedVarManager::kVtxZ);
    


      man->AddHistogram(classStr.Data(),"NTracksTPCout_VZEROmult_VtxZ","",kTRUE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult,20,-10,10,AliReducedVarManager::kVtxZ);    

      man->AddHistogram(classStr.Data(),"NTracksTRDout_VZEROmult","",kFALSE,300,0.,15000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTRDout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);
 //     man->AddHistogram(classStr.Data(),"NTracksITSout_VZEROmult","",kFALSE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kITSout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);
 //     man->AddHistogram(classStr.Data(),"NTracksTOFout_VZEROmult","",kFALSE,300,0.,3000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTOFout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);
  //    man->AddHistogram(classStr.Data(),"NTracksTotal_VZEROmult","",kFALSE,300,0.,30000.,AliReducedVarManager::kNtracksTotal, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);
  //    man->AddHistogram(classStr.Data(),"NTracksSelected_VZEROmult","",kFALSE,300,0.,300.,AliReducedVarManager::kNtracksSelected, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);
   //   man->AddHistogram(classStr.Data(),"NTracksAnalyzed_VZEROmult","",kFALSE,100,0.,100.,AliReducedVarManager::kNtracksAnalyzed, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult);



      Bool_t isCalibrated = kTRUE;
      /*for(Int_t i=0; i<64; ++i) {
         man->AddHistogram(classStr.Data(), Form("VZEROmult_ch%d",i), Form("Multiplicity in VZERO channel %d",i), kFALSE, 
                      200, 0.0, (isCalibrated ? 10.0 : 1000.0), AliReducedVarManager::kVZEROChannelMult+i);
         man->AddHistogram(classStr.Data(), Form("VZEROmult_ch%d_zoomLowMult",i), Form("Multiplicity in VZERO channel %d",i), kFALSE, 
                           100, 0.0, (isCalibrated ? 0.1 : 10.0), AliReducedVarManager::kVZEROChannelMult+i);
         man->AddHistogram(classStr.Data(),Form("VZEROmult_ch%d_Run_prof",i),Form("Multiplicity in VZERO channel %d vs run",i), kTRUE,
                      runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo, 500, 0.0, 1000.0, AliReducedVarManager::kVZEROChannelMult+i);
         man->AddHistogram(classStr.Data(),Form("VZEROmult_ch%d_VtxZ_prof",i),Form("Multiplicity in VZERO channel %d vs event vertex Z",i), kTRUE,
                      20, -10., 10., AliReducedVarManager::kVtxZ, 500, 0.0, 1000.0, AliReducedVarManager::kVZEROChannelMult+i);
         man->AddHistogram(classStr.Data(),Form("VZEROmult_ch%d_VtxCent_prof",i),Form("Multiplicity in VZERO channel %d vs (vtxZ,centrality SPD);",i),kTRUE,
                      20, -10.,10., AliReducedVarManager::kVtxZ, 18, 0.0, 90.0, AliReducedVarManager::kCentSPD, 100, 0., 1000., AliReducedVarManager::kVZEROChannelMult+i);
         man->AddHistogram(classStr.Data(),Form("VZEROmultOccupancy_ch%d_VtxCent",i),Form("Multiplicity in VZERO channel %d vs (vtxZ,centrality SPD);",i),kFALSE,
                      20, -10., 10., AliReducedVarManager::kVtxZ, 18, 0., 90., AliReducedVarManager::kCentSPD, 100, 0., 1000., AliReducedVarManager::kVZEROChannelMult+i);
      }  // end loop over channels
      */
     // man->AddHistogram(classStr.Data(), "VZEROA_NEmptyChannels_VtxCent_prof", "No. VZERO-A empty channels per event vs. centrality SPD and vertex Z", kTRUE,
      //             24, -12.0, +12.0, AliReducedVarManager::kVtxZ, 20, 0.0, 100., AliReducedVarManager::kCentSPD, 32, 0.0, 32.0, AliReducedVarManager::kVZEROAemptyChannels);
    //  man->AddHistogram(classStr.Data(), "VZEROC_NEmptyChannels_VtxCent_prof", "No. VZERO-C empty channels per event vs. centrality SPD and vertex Z", kTRUE,
       //            24, -12.0, +12.0, AliReducedVarManager::kVtxZ, 20, 0.0, 100., AliReducedVarManager::kCentSPD, 32, 0.0, 32.0, AliReducedVarManager::kVZEROCemptyChannels);
      
      const Char_t sname[2] = {'A','C'};
      for(Int_t ih=1; ih<2; ++ih) {
         man->AddHistogram(classStr.Data(), Form("QvecX_sideA_vs_sideC_h%d", ih+1), Form("Q_{x}, side A vs side C, harmonic %d",ih+1), kFALSE, 
                      100, (isCalibrated ? -20.0 : -1500.0), (isCalibrated ? 20.0 : 1500.0), AliReducedVarManager::kVZEROQvecX+0*6+ih,
                      100, (isCalibrated ? -20.0 : -1500.0), (isCalibrated ? 20.0 : 1500.0), AliReducedVarManager::kVZEROQvecX+1*6+ih);
         man->AddHistogram(classStr.Data(), Form("QvecY_sideA_vs_sideC_h%d", ih+1), Form("Q_{y}, side A vs side C, harmonic %d",ih+1), kFALSE, 
                           100, (isCalibrated ? -20.0 : -1500.0), (isCalibrated ? 20.0 : 1500.0), AliReducedVarManager::kVZEROQvecY+0*6+ih,
                           100, (isCalibrated ? -20.0 : -1500.0), (isCalibrated ? 20.0 : 1500.0), AliReducedVarManager::kVZEROQvecY+1*6+ih);
         man->AddHistogram(classStr.Data(), Form("RP_sideA_vs_sideC_h%d",ih+1), Form("VZERO RP-Aside vs RP-Cside, harmonic %d",ih+1), kFALSE, 
                           100, -4.0/Double_t(ih+1), 4.0/Double_t(ih+1), AliReducedVarManager::kVZERORP+0*6+ih,
                           100, -4.0/Double_t(ih+1), 4.0/Double_t(ih+1), AliReducedVarManager::kVZERORP+1*6+ih);
         man->AddHistogram(classStr.Data(), Form("VZERORPres_h%d_Cent",ih+1), Form("VZERO RP resolution vs centrality, harmonic %d", ih+1),
                           kTRUE, 100, 0.0, 100.0, AliReducedVarManager::kCentSPD, 100, -1., +1., AliReducedVarManager::kVZERORPres+ih);
         man->AddHistogram(classStr.Data(), Form("VZERORPres_h%d_VtxZ",ih+1), Form("VZERO RP resolution vs vertex Z, harmonic %d", ih+1),
                           kTRUE, 48, -12.0, 12.0, AliReducedVarManager::kVtxZ, 100, -1., +1., AliReducedVarManager::kVZERORPres+ih);
         man->AddHistogram(classStr.Data(), Form("VZERORPres_h%d_CentVtxZ",ih+1), Form("VZERO RP resolution vs (centrality,vtx. Z), harmonic %d", ih+1),
                           kTRUE, 100, 0.0, 100.0, AliReducedVarManager::kCentSPD, 48, -12.0, 12.0, AliReducedVarManager::kVtxZ, 100, -1., +1., AliReducedVarManager::kVZERORPres+ih);
         
         man->AddHistogram(classStr.Data(), Form("VZERO_XaXc_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{x}^{C}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaXc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XaYa_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{y}^{A}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaYa+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XaYc_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{y}^{C}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaYc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_YaXc_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{y}^{A}#timesQ_{x}^{C}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROYaXc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_YaYc_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{y}^{A}#timesQ_{y}^{C}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROYaYc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XcYc_h%d_centSPD", ih+1), 
                      Form("VZERO Q_{x}^{C}#timesQ_{y}^{C}, harmonic %d, vs centSPD", ih+1), kTRUE,
                           20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXcYc+ih);
         
    /*     man->AddHistogram(classStr.Data(), Form("VZERO_XaXc_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{x}^{C}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaXc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XaYa_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{y}^{A}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaYa+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XaYc_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{x}^{A}#timesQ_{y}^{C}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXaYc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_YaXc_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{y}^{A}#timesQ_{x}^{C}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROYaXc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_YaYc_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{y}^{A}#timesQ_{y}^{C}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROYaYc+ih);
         man->AddHistogram(classStr.Data(), Form("VZERO_XcYc_h%d_VtxZ", ih+1), 
                      Form("VZERO Q_{x}^{C}#timesQ_{y}^{C}, harmonic %d, vs VtxZ", ih+1), kTRUE,
                           24, -12.0, 12.0, AliReducedVarManager::kVtxZ, 500, (isCalibrated ? -20.0 : -2000.0), (isCalibrated ? 20.0 : 2000.0), AliReducedVarManager::kVZEROXcYc+ih);
      */   
      /*  for(Int_t iS=0; iS<2; ++iS) {
            man->AddHistogram(classStr.Data(), Form("QvecX_side%c_h%d_CentSPD",sname[iS],ih+1), 
                         Form("Q_{x}, side %c, harmonic %d vs CentSPD",sname[iS],ih+1), kFALSE, 
                              20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 100, (isCalibrated ? -10.0 : -1500.0), (isCalibrated ? 10.0 : 1500.0), AliReducedVarManager::kVZEROQvecX+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("QvecY_side%c_h%d_CentSPD",sname[iS],ih+1), 
                         Form("Q_{y}, side %c, harmonic %d vs CentSPD",sname[iS],ih+1), kFALSE, 
                              20, 0.0, 100.0, AliReducedVarManager::kCentSPD, 100, (isCalibrated ? -10.0 : -1500.0), (isCalibrated ? 10.0 : 1500.0), AliReducedVarManager::kVZEROQvecY+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("QvecX_side%c_h%d_Run_prof", sname[iS], ih+1), 
                         Form("<Q_{x}>, VZERO side %c, harmonic %d, vs run", sname[iS], ih+1), kTRUE,
                              runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo, 100, -100., 100., AliReducedVarManager::kVZEROQvecX+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("QvecY_side%c_h%d_Run_prof", sname[iS], ih+1), 
                         Form("<Q_{y}>, VZERO side %c, harmonic %d, vs run", sname[iS], ih+1), kTRUE,
                              runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo, 100, -100., 100., AliReducedVarManager::kVZEROQvecY+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("QvecX_side%c_h%d_CentSPDVtxZ_prof",sname[iS],ih+1), 
                         Form("Q_{x}, side %c, harmonic %d, vs centSPD and vtxZ",sname[iS],ih+1), kTRUE, 
                              18, 0.,90., AliReducedVarManager::kCentSPD, 20, -10.,10., AliReducedVarManager::kVtxZ, 10, 0.,100., AliReducedVarManager::kVZEROQvecX+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("QvecY_side%c_h%d_CentSPDVtxZ_prof",sname[iS],ih+1), 
                         Form("Q_{y}, side %c, harmonic %d, vs centSPD and vtxZ",sname[iS],ih+1), kTRUE, 
                              18, 0.,90., AliReducedVarManager::kCentSPD, 20, -10.,10., AliReducedVarManager::kVtxZ, 10, 0.,100., AliReducedVarManager::kVZEROQvecY+iS*6+ih);
            man->AddHistogram(classStr.Data(), Form("RP_side%c_h%d_CentSPD",sname[iS],ih+1), 
                         Form("VZERO reaction plane, side %c, harmonic %d, vs centrality SPD",sname[iS],ih+1), kFALSE, 
                              100, -4.0/Double_t(ih+1), 4.0/Double_t(ih+1), AliReducedVarManager::kVZERORP+iS*6+ih, 20, 0.0, 100.0, AliReducedVarManager::kCentSPD);
            man->AddHistogram(classStr.Data(), Form("RP_side%c_h%d_VtxZ",sname[iS],ih+1), 
                         Form("VZERO reaction plane, side %c, harmonic %d, vs vtxZ",sname[iS],ih+1), kFALSE, 
                              100, -4.0/Double_t(ih+1), 4.0/Double_t(ih+1), AliReducedVarManager::kVZERORP+iS*6+ih, 24, -12.0, +12.0, AliReducedVarManager::kVtxZ);
            
         }  */ // end loop over VZERO sides
      }   // end loop over harmonics




      
      
      continue;
    }  // end if className contains "Event"    





    
    // Track histograms
   /*&&&& if(classStr.Contains("TrackITSclusterMap_")) {
       man->AddHistClass(classStr.Data());
       cout << classStr.Data() << endl;
    //   man->AddHistogram(classStr.Data(), "ITSlayerHit", "Hits in the ITS layers", kFALSE,
      //                   6, 0.5, 6.5, AliReducedVarManager::kITSlayerHit);
       man->AddHistogram(classStr.Data(), "ITSlayerHit_Phi", "Hits in the ITS layers vs #varphi", kFALSE,
                         180, 0.0, 6.29, AliReducedVarManager::kPhi, 6, 0.5, 6.5, AliReducedVarManager::kITSlayerHit);
       man->AddHistogram(classStr.Data(), "ITSlayerHit_Eta", "Hits in the ITS layers vs #eta", kFALSE,
                         100, -1.0, 1.0, AliReducedVarManager::kEta, 6, 0.5, 6.5, AliReducedVarManager::kITSlayerHit);
       //man->AddHistogram(classStr.Data(), "ITSlayerHit_DCAxy", "Hits in the ITS layers vs DCA_{xy}", kFALSE,
       //                  1000, -0.5, 0.5, AliReducedVarManager::kDcaXY, 6, 0.5, 6.5, AliReducedVarManager::kITSlayerHit);
       //man->AddHistogram(classStr.Data(), "ITSlayerHit_DCAz", "Hits in the ITS layers vs DCA_{z}", kFALSE,
       //                  1800, -1.0, 1.0, AliReducedVarManager::kDcaZ, 6, 0.5, 6.5, AliReducedVarManager::kITSlayerHit);
       continue;
    } */ // end of ITSclusterMap histogram definitions
    /* &&&
    if(classStr.Contains("TrackTPCclusterMap_")) {
       man->AddHistClass(classStr.Data());
       cout << classStr.Data() << endl;
   //    man->AddHistogram(classStr.Data(), "TPCclusterMap", "TPC cluster map", kFALSE,
     //                    8, -0.5, 7.5, AliReducedVarManager::kTPCclusBitFired);
       man->AddHistogram(classStr.Data(), "TPCclusterMap_Phi", "TPC cluster map vs #varphi", kFALSE,
                         180, 0.0, 6.29, AliReducedVarManager::kPhi, 8, -0.5, 7.5, AliReducedVarManager::kTPCclusBitFired);
       man->AddHistogram(classStr.Data(), "TPCclusterMap_Eta", "TPC cluster map vs #eta", kFALSE,
                         100, -1.0, 1.0, AliReducedVarManager::kEta, 8, -0.5, 7.5, AliReducedVarManager::kTPCclusBitFired);
       man->AddHistogram(classStr.Data(), "TPCclusterMap_Pt", "TPC cluster map vs p_{T}", kFALSE,
                         100, 0.0, 10.0, AliReducedVarManager::kPt, 8, -0.5, 7.5, AliReducedVarManager::kTPCclusBitFired);
       continue;
    }  // end of TPCclusterMap histogram definitions
    */
   /* TString trkStatusNames = "";
    for(Int_t iflag=0; iflag<AliReducedVarManager::kNTrackingStatus; ++iflag) {
       trkStatusNames += AliReducedVarManager::fgkTrackingStatusNames[iflag];
       trkStatusNames += ";";
    }
*/  /*&&&  if(classStr.Contains("TrackStatusFlags_")) {
       man->AddHistClass(classStr.Data());
       cout << classStr.Data() << endl;
       man->AddHistogram(classStr.Data(), "TrackingFlags", "Tracking flags;;", kFALSE,
                         AliReducedVarManager::kNTrackingStatus, -0.5, AliReducedVarManager::kNTrackingStatus-0.5, AliReducedVarManager::kTrackingFlag, 
                         0, 0.0, 0.0, AliReducedVarManager::kNothing, 0, 0.0, 0.0, AliReducedVarManager::kNothing, trkStatusNames.Data());
    //   man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_TrackingFlags","Corrected TPC N_{#sigma} electron vs. inner param P vs tracking flags;;",kFALSE,
      //                   50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, AliReducedVarManager::kNTrackingStatus, -0.5, AliReducedVarManager::kNTrackingStatus-0.5, AliReducedVarManager::kTrackingFlag, "", "", trkStatusNames.Data());
       continue;
    }
    */
    if(classStr.Contains("Track_")) {
      man->AddHistClass(classStr.Data());
      cout << classStr.Data() << endl;
    //  man->AddHistogram(classStr.Data(), "P_Pin", "P vs Pin", kFALSE, 200, 0.0, 10.0, AliReducedVarManager::kP, 200, 0.0, 10.0, AliReducedVarManager::kPin);
      man->AddHistogram(classStr.Data(), "Pt", "p_{T} distribution", kFALSE, 1000, 0.0, 50.0, AliReducedVarManager::kPt);
     // man->AddHistogram(classStr.Data(), "Pt_TimeFromSOR", "<p_{T}> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 1000, 0.0, 50.0, AliReducedVarManager::kPt);
      man->AddHistogram(classStr.Data(), "Eta", "", kFALSE, 1000, -1.5, 1.5, AliReducedVarManager::kEta);
     // man->AddHistogram(classStr.Data(), "Eta_TimeFromSOR", "<#eta> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 1000, -1.5, 1.5, AliReducedVarManager::kEta);
      man->AddHistogram(classStr.Data(), "Phi", "", kFALSE, 1000, 0.0, 6.3, AliReducedVarManager::kPhi);
     // man->AddHistogram(classStr.Data(), "Phi_TimeFromSOR", "<#varphi> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 1000, 0.0, 6.3, AliReducedVarManager::kPhi);
      man->AddHistogram(classStr.Data(), "DCAxy", "DCAxy", kFALSE, 200, -5.0, 5.0, AliReducedVarManager::kDcaXY);
     // man->AddHistogram(classStr.Data(), "DCAxy_TimeFromSOR", "<DCAxy> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200, -5.0, 5.0, AliReducedVarManager::kDcaXY);
      man->AddHistogram(classStr.Data(), "DCAz", "DCAz", kFALSE, 200, -5.0, 5.0, AliReducedVarManager::kDcaZ);
     // man->AddHistogram(classStr.Data(), "DCAz_TimeFromSOR", "<DCAz> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200, -5.0, 5.0, AliReducedVarManager::kDcaZ);
      man->AddHistogram(classStr.Data(),"ITSnclsShared", "ITS nclusters Shared", kFALSE, 7,-0.5,6.5,AliReducedVarManager::kITSnclsShared);
      man->AddHistogram(classStr.Data(),"Fraction ITSnclsShared", "Fraction ITS nclusters Shared", kFALSE, 12,-0.1,1.1,AliReducedVarManager::kNclsSFracITS);
      //check for mixing
/*	man->AddHistogram(classStr.Data(),"MassVSVertexProfile", "Inv Mass vs Z Vertex", kTRUE, 21,-10.,10.,AliReducedVarManager::kVtxZ,150, 0.0, 6.0, 	AliReducedVarManager::kMass); 
	
	man->AddHistogram(classStr.Data(),"dedxVSVertexProfile", "dEdx vs Z Vertex", kTRUE, 21,-10.,10.,AliReducedVarManager::kVtxZ,180, 0.0, 	180.0, AliReducedVarManager::kTPCsignal);
      
	man->AddHistogram(classStr.Data(),"TPCclsVSVertexProfile", "TPCclusters vs Z Vertex", kTRUE, 21,-10.,10.,AliReducedVarManager::kVtxZ,160, 0.0, 159.0, AliReducedVarManager::kTPCncls);
*/
	man->AddHistogram(classStr.Data(),"TPCchi2VSPhiProfile", "TPCchi2 vs phi", kTRUE, 630,0.,6.3,AliReducedVarManager::kPhi,40, 0.0, 10.0, AliReducedVarManager::kTPCchi2);

	man->AddHistogram(classStr.Data(),"PairsSelectedVSPhiProfile", "Pairs vs phi", kTRUE, 630,0.,6.3,AliReducedVarManager::kPhi,100, 0.0, 100.0, AliReducedVarManager::kNpairsSelected);
	
	man->AddHistogram(classStr.Data(),"TracksSelectedVSPhiProfile", "TrackLeg vs phi", kTRUE, 630,0.,6.3,AliReducedVarManager::kPhi,100, 0.0, 100.0, AliReducedVarManager::kNtracksSelected);


	man->AddHistogram(classStr.Data(),"TPCchi2VSEtaProfile", "TPCchi2 vs eta", kTRUE, 40,-1.0,1.0,AliReducedVarManager::kEta,100, 0.0, 10.0, AliReducedVarManager::kTPCchi2);

	man->AddHistogram(classStr.Data(),"TPCchi2VSEta", "TPCchi2 vs eta", kFALSE, 40,-1.0,1.0,AliReducedVarManager::kEta,100, 0.0, 10.0, AliReducedVarManager::kTPCchi2);

	man->AddHistogram(classStr.Data(),"PairsSelectedVSEtaProfile", "Pairs vs eta", kTRUE, 40.,-1.0,1.0,AliReducedVarManager::kEta,100, 0.0, 100.0, AliReducedVarManager::kNpairsSelected);

       	man->AddHistogram(classStr.Data(),"PairsSelectedVSEta", "Tracks vs eta", kFALSE, 40.,-1.0,1.0,AliReducedVarManager::kEta,100, 0.0, 100.0, AliReducedVarManager::kNpairsSelected);
	
	man->AddHistogram(classStr.Data(),"TracksSelectedVSEtaProfile", "TrackLeg vs eta", kTRUE, 40,-1.,1.0,AliReducedVarManager::kEta,100, 0.0, 100.0, AliReducedVarManager::kNtracksSelected);
	
       man->AddHistogram(classStr.Data(),"TracksSelectedVSEta", "TrackLeg vs eta", kFALSE, 40,-1.,1.0,AliReducedVarManager::kEta,100, 0.0, 100.0, AliReducedVarManager::kNtracksSelected);
	
       //      man->AddHistogram(classStr.Data(),"NTracksTPCout_VZEROmult_TPChi2","",kTRUE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout, 300, 0., 40000., AliReducedVarManager::kVZEROTotalMult,80,0.,5.,AliReducedVarManager::kTPCchi2);
          man->AddHistogram(classStr.Data(),"NTracksTPCout_TPCchi2","",kFALSE,5000,0.,20000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout,80,0.,5.,AliReducedVarManager::kTPCchi2);

    
      man->AddHistogram(classStr.Data(),"NTracksTPCout_TPCchi2_Profile","",kTRUE,300,0.,30000.,AliReducedVarManager::kNTracksPerTrackingStatus+AliReducedVarManager::kTPCout,80,0.,5.,AliReducedVarManager::kTPCchi2);

      man->AddHistogram(classStr.Data(),"VoMult_TPCchi2_Profile","",kTRUE,100,0.,100.,AliReducedVarManager::kCentVZERO,80,0.,5.,AliReducedVarManager::kTPCchi2);
      man->AddHistogram(classStr.Data(),"VoMult_TPCchi2","",kFALSE,100,0.,100.,AliReducedVarManager::kCentVZERO,80,0.,5.,AliReducedVarManager::kTPCchi2);      
	//check for mixing
	//man->AddHistogram(classStr.Data(),"MassVSVertex", "Inv Mass vs Z Vertex", kFALSE, 21,-10.,10.,AliReducedVarManager::kVtxZ,150, 0.0, 6.0, 	AliReducedVarManager::kMass); 
	
	//man->AddHistogram(classStr.Data(),"dedxVSVertex", "dEdx vs Z Vertex", kFALSE, 21,-10.,10.,AliReducedVarManager::kVtxZ,180, 0.0, 	180.0, AliReducedVarManager::kTPCsignal);
      
//	man->AddHistogram(classStr.Data(),"TPCclsVSVertex", "TPCclusters vs Z Vertex", kFALSE, 21,-10.,10.,AliReducedVarManager::kVtxZ,160, 0.0, 159.0, AliReducedVarManager::kTPCncls);

	man->AddHistogram(classStr.Data(),"TPCchi2VSPhi", "TPCchi2 vs phi", kFALSE, 630,0.,6.3,AliReducedVarManager::kPhi,40, 0.0, 10.0, AliReducedVarManager::kTPCchi2);

	//man->AddHistogram(classStr.Data(),"PairsSelectedVSPhi", "Pairs vs phi", kFALSE, 630,0.,6.3,AliReducedVarManager::kPhi,100, 0.0, 100.0, AliReducedVarManager::kNpairsSelected);	
	
	man->AddHistogram(classStr.Data(),"TracksSelectedVSPhi", "TrackLeg vs phi", kFALSE, 630,0.,6.3,AliReducedVarManager::kPhi,100, 0.0, 100.0, AliReducedVarManager::kNtracksSelected);

	//	man->AddHistogram(classStr.Data(),"Eta_Phi_FracITSsharedCls_prof","its <Fraction_Sharedcls> vs (#eta,#phi)",kTRUE,
	//               36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 11, 0.0, 1.1, AliReducedVarManager::kNclsSFracITS);
	
    //	man->AddHistogram(classStr.Data(),"Eta_Phi_TPCchi2_prof","TPC <#chi^{2}> vs (#eta,#phi)",kTRUE,
    //               36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 100, 0.0, 1000, AliReducedVarManager::kTPCchi2);
	
	
	//	man->AddHistogram(classStr.Data(),"Eta_Phi_TPCSegments","TPC <segments> vs (#eta,#phi)",kTRUE,
	//		  36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 8, 0.0, 8.0, AliReducedVarManager::kTPCNclusBitsFired);

	man->AddHistogram(classStr.Data(),"Eta_Phi_TracksCandidates","TPC <#chi^{2}> vs (#eta,#phi)",kTRUE,
			  36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 8, 0.0, 8.0, AliReducedVarManager::kNtracksSelected);
	
	
        man->AddHistogram(classStr.Data(),"ITSncls", "ITS nclusters", kFALSE, 7,-0.5,6.5,AliReducedVarManager::kITSncls);
     //   man->AddHistogram(classStr.Data(),"ITSncls_TimeFromSOR", "<ITS nclusters> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 7,-0.5,6.5,AliReducedVarManager::kITSncls);
	//        man->AddHistogram(classStr.Data(),"Eta_Phi_ITSncls_prof","ITS <nclusters> vs (#eta,#phi)",kTRUE,
	//           36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 7, -0.5, 6.5, AliReducedVarManager::kITSncls);
	man->AddHistogram(classStr.Data(),"ITSchi2", "ITS #chi^{2}", kFALSE, 200,0.0,100.0, AliReducedVarManager::kITSchi2);
       // man->AddHistogram(classStr.Data(),"ITSchi2_TimeFromSOR", "<ITS #chi^{2}> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200,0.0,20.0, AliReducedVarManager::kITSchi2);
	//	man->AddHistogram(classStr.Data(),"Eta_Phi_ITSchi2_prof","ITS <#chi^{2}> vs (#eta,#phi)",kTRUE,
	//           36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 100, 0.0, 1000, AliReducedVarManager::kITSchi2);
      
        cout << "1" << endl;
        man->AddHistogram(classStr.Data(),"Chi2TPCConstrainedVsGlobal","",kFALSE,200,0.,200.,AliReducedVarManager::kChi2TPCConstrainedVsGlobal);
      //  man->AddHistogram(classStr.Data(),"Chi2TPCConstrainedVsGlobal_TimeFromSOR_prof","<Chi2 TPC constrained vs global> vs time from SOR",kTRUE,90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 100,0.,100.,AliReducedVarManager::kChi2TPCConstrainedVsGlobal);
        man->AddHistogram(classStr.Data(),"TPCncls","",kFALSE,160,-0.5,159.5,AliReducedVarManager::kTPCncls);
	man->AddHistogram(classStr.Data(),"TPCnclsRatio3","",kFALSE,100,0.0,10.0,AliReducedVarManager::kTPCnclsRatio3);

	
        //man->AddHistogram(classStr.Data(),"TPCncls_TimeFromSOR","<TPC #cls> vs time from SOR",kTRUE,90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 160,-0.5,159.5,AliReducedVarManager::kTPCncls);
	man->AddHistogram(classStr.Data(),"TPCcrossedRows","", kFALSE, 160,-0.5,159.5,AliReducedVarManager::kTPCcrossedRows);
	man->AddHistogram(classStr.Data(),"TPCnclsShared","",kFALSE, 160,-0.5,159.5,AliReducedVarManager::kTPCnclsShared);
	
	man->AddHistogram(classStr.Data(),"TPCnclsratio","",kFALSE, 100,-0.0,1.1,AliReducedVarManager::kTPCnclsRatio);
	man->AddHistogram(classStr.Data(),"TPCnclsratio2","",kFALSE, 100,-0.0,1.1,AliReducedVarManager::kTPCnclsRatio2);


       // man->AddHistogram(classStr.Data(),"TPCnclsShared_TimeFromSOR","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 160,-0.5,159.5,AliReducedVarManager::kTPCnclsShared);
	man->AddHistogram(classStr.Data(),"TPCchi2","",kFALSE, 200,0.0,10.0,AliReducedVarManager::kTPCchi2);
	man->AddHistogram(classStr.Data(),"TPCchi2perRun","tpcchi2 vs. run",kTRUE,
                     runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,100,0.0,5.0,AliReducedVarManager::kTPCchi2);
	man->AddHistogram(classStr.Data(),"TPCsegmentsperRun","tpcsegments vs. run",kTRUE,
			  runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,9.0,0.0,9.0,AliReducedVarManager::kTPCNclusBitsFired);

		
        man->AddHistogram(classStr.Data(),"TPCsegments","",kFALSE, 9,0.0,9.0,AliReducedVarManager::kTPCNclusBitsFired);
     //   man->AddHistogram(classStr.Data(),"TPCsegments_TimeFromSOR","",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 8,0.0,8.0,AliReducedVarManager::kTPCNclusBitsFired);
     //   man->AddHistogram(classStr.Data(),"TPCchi2_TimeFromSOR","TPC <#chi^{2}> vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 200,0.0,10.0,AliReducedVarManager::kTPCchi2);
        man->AddHistogram(classStr.Data(),"Eta_Phi_TPCncls_prof","TPC <nclusters> vs (#eta,#phi)",kTRUE,
                     36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 160, -0.5, 159.5, AliReducedVarManager::kTPCncls);    
	//	man->AddHistogram(classStr.Data(),"Eta_Phi_TPCcrossedRows_prof","TPC <n crossed rows> vs (#eta,#phi)",kTRUE,
	//           36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 160, -0.5, 159.5, AliReducedVarManager::kTPCcrossedRows);
	man->AddHistogram(classStr.Data(),"Eta_Phi_TPCnclsF_prof","TPC <nclusters findable> vs (#eta,#phi)",kTRUE,
                     36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 160, -0.5, 159.5, AliReducedVarManager::kTPCnclsF);
        man->AddHistogram(classStr.Data(),"TPCsignal_Pin","TPC dE/dx vs. inner param P",kFALSE,
                     300,0.0,30.0,AliReducedVarManager::kPin,200,-0.5,199.5,AliReducedVarManager::kTPCsignal);
	
	      man->AddHistogram(classStr.Data(),"TPCsignal_RunNo","TPC dE/dx vs.run",kFALSE,
                   runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,200,-0.5,199.5,AliReducedVarManager::kTPCsignal);
	
	man->AddHistogram(classStr.Data(),"TPCsignalN","",kFALSE,160,-0.5,159.5,AliReducedVarManager::kTPCsignalN);

	man->AddHistogram(classStr.Data(),"TPCcrossedrows_ratio","",kFALSE,20,-0.1,1.1,AliReducedVarManager::kTPCnclsRatio);
	
       // man->AddHistogram(classStr.Data(),"TPCsignalN_TimeFromSOR","TPC <#cls pid> vs time from SOR",kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 160,-0.5,159.5,AliReducedVarManager::kTPCsignalN);
	//	man->AddHistogram(classStr.Data(),"Eta_Phi_TPCsignalN_prof","TPC <nclusters pid> vs (#eta,#phi)",kTRUE,
	//           36, -0.9, 0.9, AliReducedVarManager::kEta, 90, 0.0, 2.0*TMath::Pi(), AliReducedVarManager::kPhi, 160, -0.5, 159.5, AliReducedVarManager::kTPCsignalN);    
        cout << "2" << endl;
        //man->AddHistogram(classStr.Data(),"TPCnsigElectron_TimeFromSOR","TPC N_{#sigma} electron vs. time from SOR",kFALSE,
         //                 90, 0., 450.,AliReducedVarManager::kTimeRelativeSOR,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron);
     //   man->AddHistogram(classStr.Data(),"TPCnsigElectron_TimeFromSOR_prof","<TPC N_{#sigma} electron> vs. time from SOR",kTRUE,
       //                   90, 0., 450.,AliReducedVarManager::kTimeRelativeSOR,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron);
        man->AddHistogram(classStr.Data(),"TPCnsigElectron_Pin","TPC N_{#sigma} electron vs. inner param P",kFALSE,
                     100,0.0,10.0,AliReducedVarManager::kPin,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron);
        //man->AddHistogram(classStr.Data(),"TPCnsigElectron_Pin_TimeFromSOR","TPC N_{#sigma} electron vs. inner param P vs time from SOR",kFALSE,
          //                100,0.0,10.0,AliReducedVarManager::kPin,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron, 15, 0., 450., AliReducedVarManager::kTimeRelativeSOR);
        man->AddHistogram(classStr.Data(),"TPCnsigElectron_Eta","TPC N_{#sigma} electron vs. #eta",kFALSE,
                          36,-0.9,0.9,AliReducedVarManager::kEta,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron);
        //man->AddHistogram(classStr.Data(),"TPCnsigElectron_Eta_TimeFromSOR","TPC N_{#sigma} electron vs. #eta vs time from SOR",kFALSE,
          //                36,-0.9,0.9,AliReducedVarManager::kEta,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron, 15, 0., 450., AliReducedVarManager::kTimeRelativeSOR);
        man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin","Corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
                          100,0.0,10.0,AliReducedVarManager::kPin,100,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron);
  //      man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_DCAxy","DCAxy vs corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
    //                      50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 80, -2., 2., AliReducedVarManager::kDcaXY);
    //    man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_DCAz","DCAz vs corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
      //                    50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 120, -3., 3., AliReducedVarManager::kDcaZ);
	// man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_ITSchi2","ITS chi2 vs corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 50., AliReducedVarManager::kITSchi2);
	//  man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_ITSchi2_prof","ITS <chi2> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 50., AliReducedVarManager::kITSchi2);
	// man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_TPCchi2","TPC chi2 vs corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 5., AliReducedVarManager::kTPCchi2);
        //man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_TPCchi2_prof","TPC <chi2> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
      //                    50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 5., AliReducedVarManager::kTPCchi2);
      //  man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_Chi2TPCConstrainedVsGlobal","TPC constrained vs global chi2 vs corrected TPC N_{#sigma} electron vs. inner param P",kFALSE,
        //                  50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 200., AliReducedVarManager::kChi2TPCConstrainedVsGlobal);
	//  man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_Chi2TPCConstrainedVsGlobal_prof","TPC constrained vs global <chi2> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 50, 0., 200., AliReducedVarManager::kChi2TPCConstrainedVsGlobal);
	//  man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_NclsITS_prof","<#ITScls> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 7, 0., 7., AliReducedVarManager::kITSncls);
	// man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_NclsTPC_prof","<#TPCcls> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
	//                50,0.0,5.0,AliReducedVarManager::kPin,50,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 80, 0., 160., AliReducedVarManager::kTPCncls);
        
	//        man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Eta","TPC N_{#sigma} electron vs. #eta",kFALSE,
	//                36,-0.9,0.9,AliReducedVarManager::kEta,100,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron);
      //  man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_TimeFromSOR_prof","<corrected TPC N_{#sigma} electron> vs. time from SOR",kTRUE,
        //                  90, 0., 450.,AliReducedVarManager::kTimeRelativeSOR,80,-4.0,4.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron);
       // man->AddHistogram(classStr.Data(),"MassUsedForTracking","",kFALSE,40,0.,4.,AliReducedVarManager::kMassUsedForTracking);
       // man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Pin_MassUsedForTracking_prof","<mass used for tracking> vs corrected TPC N_{#sigma} electron vs. inner param P",kTRUE,
         //                 100,0.0,10.0,AliReducedVarManager::kPin,100,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 40, 0., 4., AliReducedVarManager::kMassUsedForTracking);
       // man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Eta_MassUsedForTracking_prof","<mass used for tracking> vs corrected TPC N_{#sigma} electron vs. eta",kTRUE,
         //                 36,-0.9,0.9,AliReducedVarManager::kEta,100,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron, 40, 0., 4., AliReducedVarManager::kMassUsedForTracking);
        cout << "3" << endl;
	man->AddHistogram(classStr.Data(),"TPCnsigElectron_Run","TPC N_{#sigma} electron vs. run",kTRUE,
                     runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,100,-5.0,5.0,AliReducedVarManager::kTPCnSig+AliReducedVarManager::kElectron);
      
	man->AddHistogram(classStr.Data(),"TPCnsigElectronCorrected_Run","Corrected TPC N_{#sigma} electron vs. run",kTRUE,
                         runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,100,-5.0,5.0,AliReducedVarManager::kTPCnSigCorrected+AliReducedVarManager::kElectron);
	
       // cout << "4" << endl;
	//       man->AddHistogram(classStr.Data(),"TOFbeta_P","TOF #beta vs P",kFALSE,
	//              200,0.0,20.0,AliReducedVarManager::kP, 220,0.0,1.1,AliReducedVarManager::kTOFbeta);

       man->AddHistogram(classStr.Data(),"TOFnsigElectron_Run","TOF N_{#sigma} electron vs. run",kTRUE,
                     runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,100,-5.0,5.0,AliReducedVarManager::kTOFnSig+AliReducedVarManager::kElectron);

        man->AddHistogram(classStr.Data(),"TOFnsigElectron_Pin"," TOF N_{#sigma} electron vs. inner param P",kFALSE,
                          100,0.0,10.0,AliReducedVarManager::kPin,100,-10.0,10.0,AliReducedVarManager::kTOFnSig+AliReducedVarManager::kElectron);
         man->AddHistogram(classStr.Data(),"TOFnsigPion_Pin"," TOF N_{#sigma} pion vs. inner param P",kFALSE,
			   100,0.0,10.0,AliReducedVarManager::kPin,100,-10.0,10.0,AliReducedVarManager::kTOFnSig+AliReducedVarManager::kPion);
          man->AddHistogram(classStr.Data(),"TOFnsigProton_Pin"," TOF N_{#sigma} proton vs. inner param P",kFALSE,
                          100,0.0,10.0,AliReducedVarManager::kPin,100,-10.0,10.0,AliReducedVarManager::kTOFnSig+AliReducedVarManager::kProton);
       //  cout << "5" << endl;
      continue;
    }  // end if "TrackQA"
        
   /* const Int_t kNMassBins = 125;
    Double_t massBins[kNMassBins];
    for(Int_t i=0; i<kNMassBins; ++i) massBins[i] = 0.0 + i*0.04; 
    
    //const Int_t kNPtBins = 55;
    const Int_t kNPtBins = 8;
    Double_t ptBins[kNPtBins] = {0., 1.0, 3.0, 5.0, 7.0, 10.0, 15.0, 20.};
    //for(Int_t i=0; i<=50; ++i) ptBins[i] = 0.0+ i*0.01;
    //ptBins[51] = 1.5; ptBins[52] = 4.5; ptBins[53] = 10.0; ptBins[54] = 20.0;
    
    
    const Int_t kNCentBins = 3;
    //Double_t centBins[kNCentBins] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,15.,20.,25.,30.,35.,40.,50.,60.,70.,90.};
    Double_t centBins[kNCentBins] = {
       0.0,   5.0,   10.0}; 

    
    //const Int_t kNVtxBins = 2;
    //Double_t vtxBins[kNVtxBins] = {-10.,10.};
    const Int_t kNTimeBins = 225;
    Double_t timeBins[kNTimeBins];
    for(Int_t i=0; i<kNTimeBins; ++i) timeBins[i] = 0.0 + i*2.0;
    
    const Int_t kNEPbins = 2;
    Double_t epBins[kNEPbins] = {-0.5*TMath::Pi()*10000.,0.5*TMath::Pi()*10000.};
    
    //Int_t vars[5] = {AliReducedVarManager::kMass, AliReducedVarManager::kPt, 
    //   AliReducedVarManager::kCentVZERO, AliReducedVarManager::kVtxZ, AliReducedVarManager::kVZERORP+6+1};
    Int_t vars[5] = {AliReducedVarManager::kMass, AliReducedVarManager::kPt, 
       AliReducedVarManager::kCentVZERO, AliReducedVarManager::kTimeRelativeSOR, AliReducedVarManager::kVZERORP+6+1};
       
    //TArrayD pairHistBinLimits[5] = {TArrayD(kNMassBins,massBins), TArrayD(kNPtBins,ptBins), TArrayD(kNCentBins,centBins), TArrayD(kNVtxBins,vtxBins), TArrayD(kNEPbins,epBins)};
    TArrayD pairHistBinLimits[5];
    pairHistBinLimits[0] = TArrayD(kNMassBins,massBins);
    pairHistBinLimits[1] = TArrayD(kNPtBins,ptBins);
    pairHistBinLimits[2] = TArrayD(kNCentBins,centBins);
    pairHistBinLimits[3] = TArrayD(kNTimeBins,timeBins);
    pairHistBinLimits[4] = TArrayD(kNEPbins,epBins);
  */
       
    const Int_t kNMassBins = 150;
    Double_t massBins[kNMassBins];
    for(Int_t i=0; i<=kNMassBins; ++i) massBins[i] = 0.0 + i*0.04; 
    
    //const Int_t kNPtBins = 55;
    const Int_t kNPtBins = 15;
    Double_t ptBins[kNPtBins] = {0.,0.15,1.0,1.3,1.5, 2.0, 2.5, 3.0, 4.0 ,5.0, 6.0, 7.0, 8.0, 10.0, 20.};
    //for(Int_t i=0; i<=50; ++i) ptBins[i] = 0.0+ i*0.01;
    //ptBins[51] = 1.5; ptBins[52] = 4.5; ptBins[53] sm= 10.0; ptBins[54] = 20.0;
    
    const Int_t kNCentBins = 13;
    //Double_t centBins[kNCentBins] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,15.,20.,25.,30.,35.,40.,50.,60.,70.,90.};
    Double_t centBins[kNCentBins] = {0.0, 1.0, 2.0, 10.0,15.0,20.0, 30.0, 40., 50., 60.,70.,80., 90.};
//    {0.0, 5.0, 10.0, 20.0, 30.0, 40., 50., 60., 70.,80.,90.};
//Double_t timeBins[kNTimeBins];
    //  for(Int_t i=0; i<kNTimeBins; ++i) timeBins[i] = 0.0 + i*2.0;

    const Int_t kNVtxBins =9;
    //Double_t vtxBins[kNVtxBins] ={-10,10};
    Double_t vtxBins[kNVtxBins]={-10.,-7.,-4.,-2.,0.,2.,4.,7.,10.};
//  Double_t vtxBins[kNVtxBins] ={-10.,-5,-4,-3,-2,-1,0,1,2,3,4,5,10};
//  {-10.,-8.,-6.,-4.,-2.,0.,2.,4.,6.,8.,10.}; -4.0*TMath::Pi()/8.,
    
    //const Int_t kNtracksTotalBins=3;
   // Double_t tracksTotalBins[kNtracksTotalBins]= {0.,1.,2.};

        const Int_t kNEPBins=9;
	//	Double_t epBins[kNEPBins] =
	  ;
   // for(Int_t i=0; i<kNTimeBins; ++i) timeBins[i] = 0.0 + i*450.0;
   // for(Int_t i=0; i<kNTimeBins; ++i) timeBins[i] = 0.0 + i*450.0;



	  Double_t epBins[9] ={ -4.0*TMath::Pi()/8., -3.0*TMath::Pi()/8., -2.0*TMath::Pi()/8., -1.0*TMath::Pi()/8., 0.0*TMath::Pi()/8., 1.0*TMath::Pi()/8., 2.0*TMath::Pi()/8., 3.0*TMath::Pi()/8., 4.0*TMath::Pi()/8.};
	  
	  // Double_t epBins[9]= {-0.5*TMath::Pi()+1.*TMath::Pi()/8.,-0.5*TMath::Pi()+2.*TMath::Pi()/8.,-0.5*TMath::Pi()+3.*TMath::Pi()/8.,-0.5*TMath::Pi()+4.*TMath::Pi()/8., -0.5*TMath::Pi()+5.*TMath::Pi()/8.,
	  //-0.5*TMath::Pi()+6.*TMath::Pi()/8.,     -0.5*TMath::Pi()+7.*TMath::Pi()/8., -0.5*TMath::Pi()+8.*TMath::epPi()/8., -0.5*TMath::Pi()+9.*TMath::Pi()/8.};
   // const Int_t kNtimeRelativeSORBins = 6;

   //    Double_t timeRelativeSORBins[kNtimeRelativeSORBins] ={0.0,100.,200.,300.,400.,500.}; //10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,110.,120.,130.,140.,150.,160.,170.,180.,190.,200.,210.,220.,230.,240.,250.,260.,270.,280.,290.,300.,310.,320.,330.,340.,350.,360.,370.,380.,390.,400.,410.,420.,430.,440.};
    
   Int_t vars[5] = {AliReducedVarManager::kMass, AliReducedVarManager::kPt,
		    AliReducedVarManager::kCentVZERO, AliReducedVarManager::kVtxZ,AliReducedVarManager::kVZERORP+6+1};

    //Int_t vars[] = {AliReducedVarManager::kMass,
    //		   AliReducedVarManager::kCentVZERO, AliReducedVarManager::kVtxZ, AliReducedVarManager::kTimeRelativeSOR};

    
    //TArrayD pairHistBinLimits[5] = {TArrayD(kNMassBins,massBins), TArrayD(kNPtBins,ptBins), TArrayD(kNCentBins,centBins), TArrayD(kNVtxBins,vtxBins), TArrayD(kNEPbins,epBins)};
    TArrayD pairHistBinLimits[5];
    pairHistBinLimits[0] = TArrayD(kNMassBins,massBins);
    pairHistBinLimits[1] = TArrayD(kNPtBins,ptBins);
    pairHistBinLimits[2] = TArrayD(kNCentBins,centBins);
    //pairHistBinLimits[3] = TArrayD(kNtracksTotalBins,tracksTotalBins);
    pairHistBinLimits[3] = TArrayD(kNVtxBins,vtxBins);
      pairHistBinLimits[4] = TArrayD(kNEPBins,epBins);
      // pairHistBinLimits[4] = TArrayD(kNtimeRelativeSORBins,timeRelativeSORBins);


    // Histograms for pairs
    if(classStr.Contains("Pair")) {
      man->AddHistClass(classStr.Data());
      cout << classStr.Data() << endl;
      man->AddHistogram(classStr.Data(), "PairInvMass", "Differential pair inv. mass", 5, vars, pairHistBinLimits);
      if(classStr.Contains("PairSE") || classStr.Contains("PairPrefilterSE")) {
        man->AddHistogram(classStr.Data(), "PairType", "Pair type", kFALSE, 4, -0.5, 3.5, AliReducedVarManager::kPairType);
	man->AddHistogram(classStr.Data(), "Mass", "Invariant mass", kFALSE, kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);

	//     man->AddHistogram(classStr.Data(), "Mass_TimeFromSOR", "Invariant mass vs time from SOR", kFALSE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
	//   man->AddHistogram(classStr.Data(), "Mass_TimeFromSOR_prof", "<Invariant mass> vs time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
	//	man->AddHistogram(classStr.Data(), "Mass_Cent_TimeFromSOR_prof", "<Invariant mass> vs centrality and time from SOR", kTRUE, 90, 0., 450., AliReducedVarManager::kTimeRelativeSOR, 18, 0.0, 90., AliReducedVarManager::kCentVZERO, kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
        man->AddHistogram(classStr.Data(), "Pt", "", kFALSE, 1000, 0.0, 20.0, AliReducedVarManager::kPt);
	man->AddHistogram(classStr.Data(), "Rapidity", "Rapidity", kFALSE, 240, -1.2, 1.2, AliReducedVarManager::kRap);
	man->AddHistogram(classStr.Data(), "Phi", "Azimuthal distribution", kFALSE, 315, 0.0, 6.3, AliReducedVarManager::kPhi);
        man->AddHistogram(classStr.Data(),"NTotalPairsAnalyzedperEvent","#dielectron candidates per event",kFALSE,4000,0.,40.,AliReducedVarManager::kNpairsSelected);
	man->AddHistogram(classStr.Data(),"RapvsMass", "Rapidity vs mass", kTRUE, 40,-1.0,1.0,AliReducedVarManager::kRap,kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
	man->AddHistogram(classStr.Data(),"PhivsMass", "Phi vs mass", kTRUE, 315,0.0,6.3, AliReducedVarManager::kPhi,kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
	man->AddHistogram(classStr.Data(),"PtvsMass", "pt vs mass", kTRUE,kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass,200,0.0,20.0,AliReducedVarManager::kPt);
	man->AddHistogram(classStr.Data(),"MassvsPt2d", "mass vs pt", kFALSE,kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass,200,0.0,20.0,AliReducedVarManager::kPt);
	man->AddHistogram(classStr.Data(),"NTotalPairsAnalyzedperEventvsRun","Pairs selected vs. run",kTRUE,
                     runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,4000,0.0,40.0,AliReducedVarManager::kNpairsSelected);
	man->AddHistogram(classStr.Data(),"MassperEventvsRun","Pairs selected vs. run",kTRUE,
                     runNBins, runHistRange[0], runHistRange[1], AliReducedVarManager::kRunNo,kNMassBins, 0.0, 6.0, AliReducedVarManager::kMass);
	
       man->AddHistogram(classStr.Data(),"MassVSVertex_pair", "Inv Mass vs Z Vertex", kFALSE, 21,-10.,10.,AliReducedVarManager::kVtxZ,150, 0.0, 6.0,AliReducedVarManager::kMass); 
       man->AddHistogram(classStr.Data(),"MassVSVertex_pairprofile", "Inv Mass vs Z Vertex", kTRUE, 21,-10.,10.,AliReducedVarManager::kVtxZ,150, 0.0, 6.0,AliReducedVarManager::kMass); 

}   // end if "QA"
      
      continue;
    }   // end if for Pair classes of histograms
  }  // end loop over histogram classes
}
