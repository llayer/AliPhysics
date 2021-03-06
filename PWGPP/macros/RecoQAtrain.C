// RS: This is macro to build an analysis train which will run from the reco loop
//     Uses special AliRecoInputHandler 
// Should be attached in rec.C as
// AliReconstruction rec; rec.SetAnalysisMacro(thisMacroPath);

void LoadLibraries();
void AddAnalysisTasks(Bool_t writeITSTP=kFALSE, Bool_t useTPCcrv=kFALSE); 


void RecoQAtrain(Bool_t writeITSTP=kFALSE, Bool_t useTPCcrv=kFALSE)
{
  //
  TGrid::Connect("alien://");
  if (!gGrid || !gGrid->IsConnected()) {
    ::Error("QAtrain", "No grid connection");
    return;
  }   
  // Set temporary merging directory to current one
  gSystem->Setenv("TMPDIR", gSystem->pwd());
  // Set temporary compilation directory to current one
  gSystem->SetBuildDir(gSystem->pwd(), kTRUE);
  // Load libraries
  LoadLibraries();
  printf("Include path: %s\n", gSystem->GetIncludePath());
  // Create manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("Analysis", "Reco QA train");
  // Input handler
  AliRecoInputHandler *esdHandler = new AliRecoInputHandler();
  esdHandler->SetReadFriends(kTRUE);
  esdHandler->SetActiveBranches("ESDfriend");
  mgr->SetInputEventHandler(esdHandler);
  //  
  // AnalysisTasks
  AddAnalysisTasks(writeITSTP,useTPCcrv);
  //
  if (!mgr->InitAnalysis()) printf("Failed to initialize Reco Analysis");
}

void LoadLibraries()
{
  gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_PHYSICS/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/ITS -I$ALICE_PHYSICS/TRD -I$ALICE_PHYSICS/PWGPP -I$ALICE_PHYSICS/PWGPP/TRD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libCORRFW");
  gSystem->Load("libTender");
  //  gSystem->Load("libPWG0base");
  //  gSystem->Load("libPWG0dep");
  //  gSystem->Load("libPWG0selectors");
  gSystem->Load("libPWGPP");
}

void AddAnalysisTasks(Bool_t writeITSTP, Bool_t useTPCcrv)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  mgr->SetCommonFileName("RecoQAresults.root");
  //
  // Event Statistics (Jan Fiete)
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(kFALSE /*MC*/);
  //
  // add standard ITSAlignQA task with only SDD calibration histos activated
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskITSAlign.C");
  AliAnalysisTaskITSAlignQA *itsAlign = AddTaskITSAlign(0,2011,kTRUE);
  itsAlign->SetLoadGeometryFromOCDB(kFALSE);
  itsAlign->SetDoSPDResiduals(kFALSE);
  itsAlign->SetDoSDDResiduals(kFALSE);
  itsAlign->SetDoSSDResiduals(kFALSE);
  itsAlign->SetDoSDDDriftTime(kFALSE);
  itsAlign->SetMinMaxMult(20.,1070.);
  itsAlign->SetUseTPCMomentum(useTPCcrv);
  itsAlign->SetDoFillTPTree(writeITSTP);
  //
  if (writeITSTP) {
    AliAnalysisDataContainer *coutputTP = mgr->CreateContainer("ITSSumTP",
							       TTree::Class(),
							       AliAnalysisManager::kOutputContainer,
							       "AODtpITS.root" );
    mgr->ConnectOutput(itsAlign, 2, coutputTP);
  }
  //
}
