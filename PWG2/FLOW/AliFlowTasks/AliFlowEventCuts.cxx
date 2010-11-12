/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */ 

// AliFlowEventCuts:
// An event cut class for the flow framework
//
// origin: Mikolaj Krzewicki (mikolaj.krzewicki@cern.ch)

#include <limits.h>
#include <float.h>
#include "TMath.h"
#include "TNamed.h"
#include "AliVVertex.h"
#include "AliVEvent.h"
#include "AliESDEvent.h"
#include "AliESDVZERO.h"
#include "AliMultiplicity.h"
#include "AliMCEvent.h"
#include "AliFlowEventCuts.h"
#include "AliFlowTrackCuts.h"

ClassImp(AliFlowEventCuts)

//-----------------------------------------------------------------------
AliFlowEventCuts::AliFlowEventCuts():
  TNamed(),
  fCutNumberOfTracks(kFALSE),
  fNumberOfTracksMax(INT_MAX),
  fNumberOfTracksMin(INT_MIN),
  fCutRefMult(kFALSE),
  fRefMultMethod(kTPConly),
  fRefMultMax(INT_MAX),
  fRefMultMin(INT_MIN),
  fRefMultCuts(NULL),
  fMeanPtCuts(NULL),
  fCutPrimaryVertexX(kFALSE),
  fPrimaryVertexXmax(INT_MAX),
  fPrimaryVertexXmin(INT_MIN),
  fCutPrimaryVertexY(kFALSE),
  fPrimaryVertexYmax(INT_MAX),
  fPrimaryVertexYmin(INT_MIN),
  fCutPrimaryVertexZ(kFALSE),
  fPrimaryVertexZmax(INT_MAX),
  fPrimaryVertexZmin(INT_MIN),
  fCutNContributors(kFALSE),
  fNContributorsMax(INT_MAX),
  fNContributorsMin(INT_MIN),
  fCutMeanPt(kFALSE),
  fMeanPtMax(-DBL_MAX),
  fMeanPtMin(DBL_MAX)
{
  //constructor 
}

//-----------------------------------------------------------------------
AliFlowEventCuts::AliFlowEventCuts(const char* name, const char* title):
  TNamed(name, title),
  fCutNumberOfTracks(kFALSE),
  fNumberOfTracksMax(INT_MAX),
  fNumberOfTracksMin(INT_MIN),
  fCutRefMult(kFALSE),
  fRefMultMethod(kTPConly),
  fRefMultMax(INT_MAX),
  fRefMultMin(INT_MIN),
  fRefMultCuts(NULL),
  fMeanPtCuts(NULL),
  fCutPrimaryVertexX(kFALSE),
  fPrimaryVertexXmax(INT_MAX),
  fPrimaryVertexXmin(INT_MIN),
  fCutPrimaryVertexY(kFALSE),
  fPrimaryVertexYmax(INT_MAX),
  fPrimaryVertexYmin(INT_MIN),
  fCutPrimaryVertexZ(kFALSE),
  fPrimaryVertexZmax(INT_MAX),
  fPrimaryVertexZmin(INT_MIN),
  fCutNContributors(kFALSE),
  fNContributorsMax(INT_MAX),
  fNContributorsMin(INT_MIN),
  fCutMeanPt(kFALSE),
  fMeanPtMax(-DBL_MAX),
  fMeanPtMin(DBL_MAX)
{
  //constructor 
}

////-----------------------------------------------------------------------
AliFlowEventCuts::AliFlowEventCuts(const AliFlowEventCuts& that):
  TNamed(that),
  fCutNumberOfTracks(that.fCutNumberOfTracks),
  fNumberOfTracksMax(that.fNumberOfTracksMax),
  fNumberOfTracksMin(that.fNumberOfTracksMin),
  fCutRefMult(that.fCutRefMult),
  fRefMultMethod(that.fRefMultMethod),
  fRefMultMax(that.fRefMultMax),
  fRefMultMin(that.fRefMultMin),
  fRefMultCuts(NULL),
  fMeanPtCuts(NULL),
  fCutPrimaryVertexX(that.fCutPrimaryVertexX),
  fPrimaryVertexXmax(that.fPrimaryVertexXmax),
  fPrimaryVertexXmin(that.fPrimaryVertexXmin),
  fCutPrimaryVertexY(that.fCutPrimaryVertexX),
  fPrimaryVertexYmax(that.fPrimaryVertexYmax),
  fPrimaryVertexYmin(that.fPrimaryVertexYmin),
  fCutPrimaryVertexZ(that.fCutPrimaryVertexX),
  fPrimaryVertexZmax(that.fPrimaryVertexZmax),
  fPrimaryVertexZmin(that.fPrimaryVertexZmin),
  fCutNContributors(that.fCutNContributors),
  fNContributorsMax(that.fNContributorsMax),
  fNContributorsMin(that.fNContributorsMin),
  fCutMeanPt(that.fCutMeanPt),
  fMeanPtMax(that.fMeanPtMax),
  fMeanPtMin(that.fMeanPtMin)
{
  //copy constructor 
  if (that.fRefMultCuts)
    fRefMultCuts = new AliFlowTrackCuts(*(that.fRefMultCuts));
  if (that.fMeanPtCuts)
    fMeanPtCuts = new AliFlowTrackCuts(*(that.fMeanPtCuts));
}

////-----------------------------------------------------------------------
AliFlowEventCuts::~AliFlowEventCuts()
{
  //dtor
  delete fMeanPtCuts;
  delete fRefMultCuts;
}

////-----------------------------------------------------------------------
AliFlowEventCuts& AliFlowEventCuts::operator=(const AliFlowEventCuts& that)
{
  //assignment
  fCutNumberOfTracks=that.fCutNumberOfTracks;
  fNumberOfTracksMax=that.fNumberOfTracksMax;
  fNumberOfTracksMin=that.fNumberOfTracksMin;
  fCutRefMult=that.fCutRefMult;
  fRefMultMethod=that.fRefMultMethod;
  fRefMultMax=that.fRefMultMax;
  fRefMultMin=that.fRefMultMin;
  if (that.fRefMultCuts) *fRefMultCuts=*(that.fRefMultCuts);
  if (that.fMeanPtCuts) *fMeanPtCuts=*(that.fMeanPtCuts);
  fCutPrimaryVertexX=that.fCutPrimaryVertexX;
  fPrimaryVertexXmin=that.fPrimaryVertexXmin;
  fPrimaryVertexXmax=that.fPrimaryVertexXmax;
  fPrimaryVertexYmin=that.fPrimaryVertexYmin;
  fPrimaryVertexYmax=that.fPrimaryVertexYmax;
  fPrimaryVertexZmin=that.fPrimaryVertexZmin;
  fPrimaryVertexZmax=that.fPrimaryVertexZmax;
  fCutNContributors=that.fCutNContributors;
  fNContributorsMax=that.fNContributorsMax;
  fNContributorsMin=that.fNContributorsMin;
  fCutMeanPt=that.fCutMeanPt;
  fMeanPtMax=that.fMeanPtMax;
  fMeanPtMin=that.fMeanPtMin;
  return *this;
}

//----------------------------------------------------------------------- 
Bool_t AliFlowEventCuts::IsSelected(const TObject* obj)
{
  //check cuts
  const AliVEvent* vevent = dynamic_cast<const AliVEvent*>(obj);
  if (vevent) return PassesCuts(vevent);
  return kFALSE;  //when passed wrong type of object
}
//----------------------------------------------------------------------- 
Bool_t AliFlowEventCuts::PassesCuts(const AliVEvent *event)
{
  ///check if event passes cuts
  if(fCutNumberOfTracks) {if (event->GetNumberOfTracks() < fNumberOfTracksMin || event->GetNumberOfTracks() >= fNumberOfTracksMax ) return kFALSE;}
  if(fCutRefMult)
  {
    //reference multiplicity still to be defined
    Double_t refMult = RefMult(event);
    if (refMult < fRefMultMin || refMult >= fRefMultMax )
      return kFALSE;
  }
  const AliVVertex* pvtx=event->GetPrimaryVertex();
  Double_t pvtxx = pvtx->GetX();
  Double_t pvtxy = pvtx->GetY();
  Double_t pvtxz = pvtx->GetZ();
  Int_t ncontrib = pvtx->GetNContributors();
  if (fCutNContributors)
  {
    if (ncontrib < fNContributorsMin || ncontrib >= fNContributorsMax)
      return kFALSE;
  }
  if (fCutPrimaryVertexX)
  {
    if (pvtxx < fPrimaryVertexXmin || pvtxx >= fPrimaryVertexXmax)
      return kFALSE;
  }
  if (fCutPrimaryVertexY)
  {
    if (pvtxy < fPrimaryVertexYmin || pvtxy >= fPrimaryVertexYmax)
      return kFALSE;
  }
  if (fCutPrimaryVertexZ)
  {
    if (pvtxz < fPrimaryVertexZmin || pvtxz >= fPrimaryVertexZmax)
      return kFALSE;
  }
  if (fCutMeanPt)
  {
    Float_t meanpt=0.0;
    Int_t ntracks=event->GetNumberOfTracks();
    for (Int_t i=0; i<ntracks; i++)
    {
      AliVParticle* track = event->GetTrack(i);
      if (!track) continue;
      Bool_t pass=kTRUE;
      if (fMeanPtCuts) pass=fMeanPtCuts->IsSelected(track);
      if (pass) meanpt += track->Pt();
    }
    meanpt=meanpt/ntracks;
    if (meanpt<fMeanPtMin || meanpt >= fMeanPtMax) return kFALSE;
  }
  return kTRUE;
}

//----------------------------------------------------------------------- 
AliFlowEventCuts* AliFlowEventCuts::StandardCuts()
{
  //make a set of standard event cuts, caller becomes owner
  AliFlowEventCuts* cuts = new AliFlowEventCuts();
  return cuts;
}

//----------------------------------------------------------------------- 
Int_t AliFlowEventCuts::RefMult(const AliVEvent* event)
{
  //calculate the reference multiplicity, if all fails return 0
  AliESDVZERO* vzero = NULL;
  const AliESDEvent* esdevent = dynamic_cast<const AliESDEvent*>(event);
  Int_t refmult=0;
  if (!fRefMultCuts)
  {
    switch (fRefMultMethod)
    {
      case kTPConly:
        fRefMultCuts = AliFlowTrackCuts::GetStandardTPCOnlyTrackCuts();
        fRefMultCuts->SetEtaRange(-0.8,0.8);
        fRefMultCuts->SetPtMin(0.15);
        break;
      case kSPDtracklets:
        fRefMultCuts = new AliFlowTrackCuts();
        fRefMultCuts->SetParamType(AliFlowTrackCuts::kESD_SPDtracklet);
        fRefMultCuts->SetEtaRange(-0.8,0.8);
        break;
      case kV0:
        if (!esdevent) return 0;
        vzero=esdevent->GetVZEROData();
        if (!vzero) return 0;
        refmult+=TMath::Nint(vzero->GetMTotV0A());
        refmult+=TMath::Nint(vzero->GetMTotV0C());
        return refmult;
      default:
        return 0;
    }
  }

  fRefMultCuts->SetEvent(const_cast<AliVEvent*>(event));
  for (Int_t i=0; i<fRefMultCuts->GetNumberOfInputObjects(); i++)
  {
    if (fRefMultCuts->IsSelected(fRefMultCuts->GetInputObject(i),i))
      refmult++;
  }
  return refmult;
}
