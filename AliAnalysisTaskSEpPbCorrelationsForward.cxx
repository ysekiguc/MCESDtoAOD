/**************************************************************************************************
*      Leading Charged Track+V0 Correlations.(Works for Real  Data)                               *
*      Yuko Sekiguchi * Center for Nuclear Study(CNS) , University of Tokyo                       *
*      Email:y_sekiguchi@cns.s.u-tokyo.ac.jp                                                      *
**************************************************************************************************/
#include "AliAnalysisManager.h"
#include "AliLog.h"
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMap.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TPDGCode.h>
#include <TParameter.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TTree.h>
#include <TProfile.h>
#include "AliAnalysisUtils.h"
#include "AliAODTrack.h"
#include "AliAODTracklets.h"
#include "AliESDtrackCuts.h"
#include "AliCFContainer.h"
#include "AliGenEventHeader.h"
#include "AliTHn.h"

#include "AliAODEvent.h"
#include "AliESDAD.h"
#include "AliESDEvent.h"
#include "AliVAD.h"
#include "AliVEvent.h"
#include "AliAODVertex.h"
#include "AliAODv0.h"
#include "AliESDVertex.h"
//#include "AliAODPid.h"
#include "AliAODHandler.h"
#include "AliAODInputHandler.h"
#include "AliAODMCHeader.h"
#include "AliAODMCParticle.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliStack.h"
#include "AliAODcascade.h"
#include "AliAnalyseLeadingTrackUE.h"
#include "AliCentrality.h"
#include "AliEventPoolManager.h"
#include "AliExternalTrackParam.h"
#include "AliInputEventHandler.h"
#include "AliMultiplicity.h"
#include "AliPID.h"
#include "AliPIDResponse.h"
#include "AliVParticle.h"
#include "Riostream.h"
//#include "AliFlowEventSimple.h"
///#include "AliFlowVector.h"/
//#include "AliFlowTrackSimple.h"
//#include "AliAnalysisTaskMSP.h"
//#include "AliFlowAnalysisWithMSP.h"
#include "AliMultSelection.h"

#include "AliAODForwardMult.h"
#include "AliForwardUtil.h"
#include "AliAnalysisTaskSEpPbCorrelationsForward.h"

ClassImp(AliAnalysisTaskSEpPbCorrelationsForward)
ClassImp(AliAssociatedTrackYS)
ClassImp(AliMixTrackYS)
ClassImp(AliAssociatedVZEROYS)

AliAnalysisTaskSEpPbCorrelationsForward::AliAnalysisTaskSEpPbCorrelationsForward()
    : AliAnalysisTaskSE(),
      fcollisiontype("pPb"),
      fDataType(kTRUE),
      frun2(kTRUE),
      fQA(kTRUE),
      fIsAOD(kTRUE),
      fAnaMode("V0AV0C"),
      fasso("Phi"),
      fPID(kFALSE),
      fCentType("ZNA"),
      lCentrality(0),
      bSign(0),
      fZVertex(10.),
      fOutputList(0),
      fOutputList1(0),
      fOutputList2(0),
      fPIDResponse(0),
      multSelection(0),
      ffilterbit(5),
      fPtMin(0.2),
      fEtaMax(0.8),
      fEventCuts(0),
      fNEntries(0),
      inputEvent(0),
      fEvent(0),
      fESD(0),
      mctruth(0),
      mcEvent(0),
      lPrimaryBestVtx(0),
      fPrimaryZVtx(0),
      fvzero(0),
      fPoolMgr(0),
      fPoolMgr1(0),
      poolmin(0),
      poolmax(0),
      fPoolMaxNEvents(2000),
      fPoolMinNTracks(5000),
      fMinEventsToMix(5),
      fNzVtxBins(0),
      fNCentBins(0),
      fMaxnSigmaTPCTOF(3.),
      fHistzvertex(0),
      fHistCentrality(0),
      fHistCentrality_aftercut(0),
      fHistLeadQA(0),
      fhistits(0),
      fHistPIDQA(0),
      fhistmcprim(0),
      fhistmcprimfinal(0),
      fhmcprimvzeta(0),
      fhmcprimpdgcode(0),
      fh2_FMD_acceptance_prim(0),
      fh2_FMD_eta_phi_prim(0),
      fh2_FMD_acceptance(0),
      fh2_FMD_eta_phi(0),
      fhistfmdphiacc(0),
      fhistfmd(0),
      fFMDV0(0),
      fFMDV0_post(0),
      fFMDV0A(0),
      fFMDV0A_post(0),
      fFMDV0C(0),
      fFMDV0C_post(0),
      fHist_vzeromult(0),
      fHist_vzeromultEqweighted(0),
      fHist2dmult(0),
      fHistVZERO(0),
      fHist_Stat(0),
      fHist_V0Stat(0),
      fHistTriggerTrack(0),
      fHistTriggerTrack1(0),
      fHistReconstTrack(0),
      fHistReconstTrack1(0),
      fHistTriggerTrackMix(0),
      fHistReconstTrackMix(0),
      fHistReconstTrackMix1(0)
      {
		for (Int_t iBin = 0; iBin < 100; iBin++) {
		  fZvtxBins[iBin] = 0.;
		  fCentBins[iBin] = 0.;
		}
		for (Int_t i = 0; i < 3; i++) {
		  tPrimaryVtxPosition[i] = 0;
		}
		for (Int_t i = 0; i < 6; i++) {
		  fHistPosNsig[i] = 0;
		  fHistNegNsig[i] = 0;
		  fHistPosNsigQA[i] = 0;
		  fHist_AP[i] = 0;
		}
		for(Int_t i=0;i<3;i++){
		  fh3NegNsig[i]=0;
		  fh3PosNsig[i]=0;
		}

		for (Int_t i = 0; i < 6; i++) {
		  fHistNsig[i]=0;
		  fHistNsigcorr[i]=0;
		}
		
		
		for(Int_t i=0;i<4;i++){
		  fhrefetaFMD[i]=0;
		  fhrefphiFMD[i]=0;
		}
		
		
		for(Int_t i=0;i<4;i++){
		  fhistdphirec[i]=0;
		  fhistitsdeltaphi[i]=0;
		  fhistitsrefdeltaphi[i]=0;
		  fhistitsrefdeltaphiaftercut[i]=0;
		  fhistitsrefdeltaetaaftercut[i]=0;
		  fhistitsrefdeltaphicorr[i]=0;
		  fhistitsrefdeltaphicorraftercut[i]=0;
		  fhistitsdeltarefphideltaphimc[i]=0;
		}
		
	  }

AliAnalysisTaskSEpPbCorrelationsForward::AliAnalysisTaskSEpPbCorrelationsForward(const char *name)
    : AliAnalysisTaskSE(name),
      fcollisiontype("pPb"),
      fDataType(kTRUE),
      frun2(kTRUE),
      fQA(kTRUE),
      fIsAOD(kTRUE),
      fAnaMode("V0AV0C"),
      fasso("Phi"),
      fPID(kFALSE),
      fCentType("ZNA"),
      lCentrality(0),
      bSign(0),
      fZVertex(10.),
      fOutputList(0),
      fOutputList1(0),
      fOutputList2(0),
      fPIDResponse(0),
      multSelection(0),
      ffilterbit(5),
      fPtMin(0.2),
      fEtaMax(0.8),
      fEventCuts(0),
      fNEntries(0),
      inputEvent(0),
      fEvent(0),
      fESD(0),
      mctruth(0),
      mcEvent(0),
      lPrimaryBestVtx(0),
      fPrimaryZVtx(0),
      fvzero(0),
      fPoolMgr(0),
      fPoolMgr1(0),
      poolmin(0),
      poolmax(0),
      fPoolMaxNEvents(2000),
      fPoolMinNTracks(5000),
      fMinEventsToMix(5),
      fNzVtxBins(0),
      fNCentBins(0),
      fMaxnSigmaTPCTOF(3.),
      fHistzvertex(0),
      fHistCentrality(0),
      fHistCentrality_aftercut(0),
      fHistLeadQA(0),
      fhistits(0),
      fHistPIDQA(0),
      fhistmcprim(0),
      fhistmcprimfinal(0),
      fhmcprimvzeta(0),
      fhmcprimpdgcode(0),
      fh2_FMD_acceptance_prim(0),
      fh2_FMD_eta_phi_prim(0),
      fh2_FMD_acceptance(0),
      fh2_FMD_eta_phi(0),
      fhistfmdphiacc(0),
      fhistfmd(0),
      fFMDV0(0),
      fFMDV0_post(0),
      fFMDV0A(0),
      fFMDV0A_post(0),
      fFMDV0C(0),
      fFMDV0C_post(0),
      fHist_vzeromult(0),
      fHist_vzeromultEqweighted(0),
      fHist2dmult(0),
      fHistVZERO(0),
      fHist_Stat(0),
      fHist_V0Stat(0),
      fHistTriggerTrack(0),
      fHistTriggerTrack1(0),
      fHistReconstTrack(0),
      fHistReconstTrack1(0),
      fHistTriggerTrackMix(0),
      fHistReconstTrackMix(0),
      fHistReconstTrackMix1(0)
      {
        for (Int_t iBin = 0; iBin < 100; iBin++) {
          fZvtxBins[iBin] = 0.;
          fCentBins[iBin] = 0.;
        }
        for (Int_t i = 0; i < 3; i++) {
          tPrimaryVtxPosition[i] = 0;
        }

		for (Int_t i = 0; i < 6; i++) {
		  fHistPosNsig[i] = 0;
		  fHistNegNsig[i] = 0;
		  fHistPosNsigQA[i] = 0 ;
		  fHist_AP[i] = 0;
		}
		for(Int_t i=0;i<3;i++){
		  fh3NegNsig[i]=0;
		  fh3PosNsig[i]=0;
		}
		for (Int_t i = 0; i < 6; i++) {
          fHistNsig[i] = 0;
          fHistNsigcorr[i]=0;
        }
		for(Int_t i=0;i<4;i++){
          fhrefetaFMD[i]=0;
          fhrefphiFMD[i]=0;
        }
		for(Int_t i=0;i<4;i++){
		  fhistdphirec[i]=0;
		  fhistitsdeltaphi[i]=0;
		  fhistitsrefdeltaphi[i]=0;
		  fhistitsrefdeltaphiaftercut[i]=0;
		  fhistitsrefdeltaetaaftercut[i]=0;
		  fhistitsrefdeltaphicorr[i]=0;
		  fhistitsrefdeltaphicorraftercut[i]=0;
		  fhistitsdeltarefphideltaphimc[i]=0;
		}
		DefineOutput(1, TList::Class());
        DefineOutput(2, TList::Class());
        DefineOutput(3, TList::Class());
	  }

AliAnalysisTaskSEpPbCorrelationsForward::~AliAnalysisTaskSEpPbCorrelationsForward()
{
  if (fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
	delete fOutputList;
	fOutputList = 0x0;
  }
  
  if (fOutputList1 && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
	delete fOutputList1;
	fOutputList1 = 0x0;
  }
  
  if (fOutputList2 && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
	delete fOutputList2;
	fOutputList2 = 0x0;
  }

  if (fPIDResponse) {
	delete fPIDResponse;
	fPIDResponse = 0;
  }
}

void AliAnalysisTaskSEpPbCorrelationsForward::UserCreateOutputObjects() {
  fOutputList = new TList();
  fOutputList->SetOwner(kTRUE);
  fOutputList->SetName("global");
  DefineGeneralOutput();
  PostData(1, fOutputList);
  
  fOutputList1 = new TList();
  fOutputList1->SetOwner(kTRUE);
  fOutputList1->SetName("anahistos");
  DefineCorrOutput();
  DefineVZEROOutput();
  PostData(2, fOutputList1);

  fOutputList2 = new TList();
  fOutputList2->SetOwner(kTRUE);
  fOutputList2->SetName("QA");
  DefinedQAHistos();
  PostData(3, fOutputList2);

  fPoolMgr = new AliEventPoolManager(fPoolMaxNEvents, fPoolMinNTracks, fNCentBins,fCentBins, fNzVtxBins, fZvtxBins);
  if (!fPoolMgr)  return;
  fPoolMgr->SetTargetValues(fPoolMinNTracks, 0.1, 5);

  fPoolMgr1 = new AliEventPoolManager(fPoolMaxNEvents, fPoolMinNTracks, fNCentBins,fCentBins, fNzVtxBins, fZvtxBins);
  if (!fPoolMgr1)  return;
  fPoolMgr1->SetTargetValues(fPoolMinNTracks, 0.1, 5);
}

void AliAnalysisTaskSEpPbCorrelationsForward::DefineGeneralOutput() {

  fHist_Stat = new TH1F("fHist_Stat", "Stat Histogram", 11, -0.5, 10.5);
  fHist_Stat->GetXaxis()->SetBinLabel(1, "All Events");
  fHist_Stat->GetXaxis()->SetBinLabel(2, "Analyzed Events");
  fHist_Stat->GetXaxis()->SetBinLabel(3, "Trigger OK");
  fHist_Stat->GetXaxis()->SetBinLabel(4, "Vertex OK");
  fHist_Stat->GetXaxis()->SetBinLabel(5, "Centrality OK");
  fHist_Stat->GetXaxis()->SetBinLabel(6, "Pile-Up rejected");
  fHist_Stat->GetXaxis()->SetBinLabel(7, "SPD vetex OK");
  fHist_Stat->GetXaxis()->SetBinLabel(8, "FMD multi cut");
  fOutputList->Add(fHist_Stat);

  fHist_V0Stat = new TH1F("fHist_V0Stat", "Stat Histogram", 16, -0.5, 15.5);
  fHist_V0Stat->GetXaxis()->SetBinLabel(1, "all");
  fHist_V0Stat->GetXaxis()->SetBinLabel(2, "On-Fly");
  fHist_V0Stat->GetXaxis()->SetBinLabel(3, "Off-Fly");
  fHist_V0Stat->GetXaxis()->SetBinLabel(4, "V0 pseudorapidity");
  fHist_V0Stat->GetXaxis()->SetBinLabel(5, "DCA Dau. tracks to PV");
  fHist_V0Stat->GetXaxis()->SetBinLabel(6, "DCA dauthers");
  fHist_V0Stat->GetXaxis()->SetBinLabel(7, "Fiducial volume");
  fHist_V0Stat->GetXaxis()->SetBinLabel(8, "Pass IsAcceptedV0");
  fHist_V0Stat->GetXaxis()->SetBinLabel(9, "track cut");
  fHist_V0Stat->GetXaxis()->SetBinLabel(10, "charge");
  fHist_V0Stat->GetXaxis()->SetBinLabel(11, "PID for K0s");
  fHist_V0Stat->GetXaxis()->SetBinLabel(12, "ctau for k0s");
  fHist_V0Stat->GetXaxis()->SetBinLabel(13, "AP cut for K0s");
  fOutputList->Add(fHist_V0Stat);

  fHistzvertex = new TH1F("fHistzvertex", ";VZ;count", 60, -15, 15);
  fOutputList->Add(fHistzvertex);

  fHistCentrality = new TH1F("fHistCentrality", ";centrality;count", 20, 0, 100);
  fOutputList->Add(fHistCentrality);
  fHistCentrality_aftercut = new TH1F("fHistCentrality_aftercut", ";centrality;count", 20, 0, 100);
  fOutputList->Add(fHistCentrality_aftercut);
  TTree *settingsTree = new TTree("UEAnalysisSettings", "Analysis Settings in UE estimation");
  settingsTree->Branch("fZVertex", &fZVertex, "fZVertex/D");
  settingsTree->Branch("fEtaMax", &fEtaMax, "fEtaMax/D");
  settingsTree->Branch("fPtMin", &fPtMin, "fPtMin/D");
  settingsTree->Branch("fMaxnSigmaTPCTOF", &fMaxnSigmaTPCTOF, "fMaxnSigmaTPCTOF/D");

  // Phi
  settingsTree->Branch("ffilterbit", &ffilterbit, "ffilterbit/I");

  //  settingsTree->Branch("fanamode",&fAnaMode,"fAnaMode/B");
  //  settingsTree->Branch("fanalysisasso",&fanalysisasso,"fanalysisasso/I");
  // settingsTree->Branch("fanalysiscent",&fanalysiscent,"fanalysiscent/I");
  settingsTree->Fill();
  fOutputList->Add(settingsTree);
}
void AliAnalysisTaskSEpPbCorrelationsForward::DefineVZEROOutput() {

  const Int_t nVZEROBins[3] = {10, 8, 15};
  Double_t binning_eta_vzero[11] = {-3.7, -3.2, -2.7, -2.2, -1.7, 0., 2.8,  3.4,  3.9,  4.5,  5.1};
  Double_t binning_phi_vzero[9] = {0., 0.7853, 1.5707, 2.3561, 3.1415, 3.9269, 4.7123, 5.4977, 6.2831};
  Double_t binning_cent[16] = {0.,  1.,  2.,  3.,  4.,  5.,  10., 20., 30., 40., 50., 60., 70., 80., 90., 100.1};

  if(fAnaMode=="TPCV0A"||fAnaMode=="TPCV0C"||fAnaMode=="V0AV0C"){
    fHist_vzeromult = new TH2F("fHist_vzeromult", "fHist_vzeromult", 64, -0.5, 63.5, 500, 0, 500);
    fOutputList1->Add(fHist_vzeromult);
    fHist_vzeromultEqweighted =  new TH2F("fHist_vzeromultEqweighted", "fHist_vzeromultEqweighted", 64, -0.5, 63.5, 500, 0, 500);
    fOutputList1->Add(fHist_vzeromultEqweighted);
    fHist2dmult = new TH3F("fHist2dmult", "fHist2dmult", 64, -0.5, 63.5, 500, 0, 500, 500, 0, 500);
    fOutputList1->Add(fHist2dmult);
    fHistVZERO = new AliTHn("fHistVZERO", "fHistVZERO", 1, 3, nVZEROBins);
    fHistVZERO->SetBinLimits(0, binning_eta_vzero);
    fHistVZERO->SetBinLimits(1, binning_phi_vzero);
    fHistVZERO->SetBinLimits(2, binning_cent);
    fOutputList1->Add(fHistVZERO);
  }

}
void AliAnalysisTaskSEpPbCorrelationsForward::DefinedQAHistos() {

  const Int_t ipidBin[5] = {12, 40, 72, 15,20};
  Double_t binning_pt_lead[13] = {0, 0.2, 0.5, 0.75, 1.0, 1.25, 1.5,
                                  2.0, 2.5, 3.0,  3.5, 4.0,  8.0};
  Double_t binning_eta[41] = {-1.,   -0.95, -0.9,  -0.85, -0.8,  -0.75, -0.7,
                              -0.65, -0.6,  -0.55, -0.5,  -0.45, -0.4,  -0.35,
                              -0.3,  -0.25, -0.2,  -0.15, -0.1,  -0.05, 0.,
                              0.05,  0.1,   0.15,  0.2,   0.25,  0.3,   0.35,
                              0.4,   0.45,  0.5,   0.55,  0.6,   0.65,  0.7,
                              0.75,  0.8,   0.85,  0.9,   0.95,  1.0};
  Double_t binning_dphi[73] = {
      -1.570796, -1.483530, -1.396263, -1.308997, -1.221730, -1.134464,
      -1.047198, -0.959931, -0.872665, -0.785398, -0.698132, -0.610865,
      -0.523599, -0.436332, -0.349066, -0.261799, -0.174533, -0.087266,
      0.0,       0.087266,  0.174533,  0.261799,  0.349066,  0.436332,
      0.523599,  0.610865,  0.698132,  0.785398,  0.872665,  0.959931,
      1.047198,  1.134464,  1.221730,  1.308997,  1.396263,  1.483530,
      1.570796,  1.658063,  1.745329,  1.832596,  1.919862,  2.007129,
      2.094395,  2.181662,  2.268928,  2.356194,  2.443461,  2.530727,
      2.617994,  2.705260,  2.792527,  2.879793,  2.967060,  3.054326,
      3.141593,  3.228859,  3.316126,  3.403392,  3.490659,  3.577925,
      3.665191,  3.752458,  3.839724,  3.926991,  4.014257,  4.101524,
      4.188790,  4.276057,  4.363323,  4.450590,  4.537856,  4.625123,
      4.712389};
  Double_t binning_cent[16] = {0.,  1.,  2.,  3.,  4.,  5.,  10., 20., 30., 40., 50., 60., 70., 80., 90., 100.1};
  if(fasso=="PID" && fQA){
  fHistPIDQA = new AliTHn("fHistPIDQA", "fHistPIDQA", 3, 5, ipidBin);
  fHistPIDQA->SetBinLimits(0, binning_pt_lead);
  fHistPIDQA->SetBinLimits(1, binning_eta);
  fHistPIDQA->SetBinLimits(2, binning_dphi);
  fHistPIDQA->SetBinLimits(3, binning_cent);
  fHistPIDQA->SetVarTitle(0, "pt");
  fHistPIDQA->SetVarTitle(1, "eta");
  fHistPIDQA->SetVarTitle(2, "phi");
  fHistPIDQA->SetVarTitle(3, "centrality");
  fOutputList1->Add(fHistPIDQA);
  }

  fHistLeadQA = new AliTHn("fHistLeadQA", "fHistLeadQA", 1, 5, ipidBin);
  fHistLeadQA->SetBinLimits(0, binning_pt_lead);
  fHistLeadQA->SetBinLimits(1, binning_eta);
  //  fHistLeadQA->SetBinLimits(2, binning_dphi);
  fHistLeadQA->SetBinLimits(2, 0,2*TMath::Pi());
  fHistLeadQA->SetBinLimits(3, binning_cent);
  fHistLeadQA->SetBinLimits(4, -10.,10.);
  fHistLeadQA->SetVarTitle(0, "pt");
  fHistLeadQA->SetVarTitle(1, "eta");
  fHistLeadQA->SetVarTitle(2, "phi");
  fHistLeadQA->SetVarTitle(3, "centrality");
  fHistLeadQA->SetVarTitle(4, "vz");
  fOutputList1->Add(fHistLeadQA);

  const Int_t iitsbin[4]={200,20,20,10};
  const	 Double_t MinITS[4]={-4,0.,0.,-10.};
  const	 Double_t MaxITS[4]={6,2*TMath::Pi(),100.,10.};
  fhistits=new THnSparseF("fhistits","fhistits",4,iitsbin,MinITS,MaxITS);
  fOutputList2->Add(fhistits);
   
  const Int_t imcprimbin[5]={12,40,72,15,20};
  Double_t centBins_mcprim[12] = {0.,  5.,  10., 20.,
                            30., 40., 50., 60., 70., 80., 90., 100.0};
    
  Double_t binning_pt_mcprim[13] = {0.,0.2, 0.5, 0.75, 1.0, 1.25, 1.5, 2.0, 2.5, 3.0,  3.5, 4.0,  8.0};
  if(!fDataType){
	
	
	for(Int_t i=0;i<4;i++){
	  fhistdphirec[i]=new TH2F(Form("fhistdphirec_%d",i),"fhistdphirec",300,0,3,100,0,3);
	  fOutputList2->Add(fhistdphirec[i]);
	  
	  fhistitsdeltaphi[i]=new TH1F(Form("fhistitsdeltaphi_%d",i),"fhistitsdeltaphi",100,0,100);
	  fOutputList2->Add(fhistitsdeltaphi[i]);
	  fhistitsrefdeltaphi[i]=new TH2F(Form("fhistitsrefdeltaphi_%d",i),"fhistitsrefdeltaphi",300,0,3,200,-100,100);
	  fOutputList2->Add(fhistitsrefdeltaphi[i]);
	  fhistitsrefdeltaphicorr[i]=new TH2F(Form("fhistitsrefdeltaphicorr_%d",i),"fhistitsrefdeltaphicorr",300,0,3,200,-100,100);
	  fOutputList2->Add(fhistitsrefdeltaphicorr[i]);
	  
	  fhistitsdeltarefphideltaphimc[i]=new TH2F(Form("fhistitsdeltarefphideltaphimc_%d",i),"fhistitsdeltarefphideltaphimc",200,-100,100,100,-100,100);
	  fOutputList2->Add(fhistitsdeltarefphideltaphimc[i]);
	  
	fhistitsrefdeltaphiaftercut[i]=new TH2F(Form("fhistitsrefdeltaphiaftercut_%d",i),"fhistitsrefdeltaphiaftercut",300,0,3,200,-100,100);
	fOutputList2->Add(fhistitsrefdeltaphiaftercut[i]);
	fhistitsrefdeltaphicorraftercut[i]=new TH2F(Form("fhistitsrefdeltaphicorraftercut_%d",i),"fhistitsrefdeltaphicorraftercut",300,0,3,200,-100,100);
	fOutputList2->Add(fhistitsrefdeltaphicorraftercut[i]);

	fhistitsrefdeltaetaaftercut[i]=new TH2F(Form("fhistitsrefdeltaetaaftercut_%d",i),"fhistitsrefdeltaetaaftercut",300,0,3,200,-0.1,0.1);
	fOutputList2->Add(fhistitsrefdeltaetaaftercut[i]);
	}

    fhistmcprim=new AliTHn("fhistmcprim","fhistmcprim",1,5,ipidBin);
    fhistmcprim->SetBinLimits(0,binning_pt_mcprim);
    fhistmcprim->SetBinLimits(1,binning_eta);
    //    fhistmcprim->SetBinLimits(2,binning_dphi);
	fhistmcprim->SetBinLimits(2,0,2*TMath::Pi());
    fhistmcprim->SetBinLimits(3,binning_cent);
	fhistmcprim->SetBinLimits(4,-10.,10.);
    fhistmcprim->SetVarTitle(0,"pt");
    fhistmcprim->SetVarTitle(1,"eta");
    fhistmcprim->SetVarTitle(2,"phi");
    fhistmcprim->SetVarTitle(3,"centrality");
    fhistmcprim->SetVarTitle(4,"vz");
    fOutputList2->Add(fhistmcprim);

    fhistmcprimfinal=new AliTHn("fhistmcprimfinal","fhistmcprimfinal",1,5,ipidBin);
    fhistmcprimfinal->SetBinLimits(0,binning_pt_mcprim);
    fhistmcprimfinal->SetBinLimits(1,binning_eta);
    fhistmcprimfinal->SetBinLimits(2,binning_dphi);
    fhistmcprimfinal->SetBinLimits(3,binning_cent);
    fhistmcprimfinal->SetBinLimits(4,-10.,10.);
    fhistmcprimfinal->SetVarTitle(0,"pt");
    fhistmcprimfinal->SetVarTitle(1,"eta");
    fhistmcprimfinal->SetVarTitle(2,"phi");
    fhistmcprimfinal->SetVarTitle(3,"centrality");
    fhistmcprimfinal->SetVarTitle(4,"vz");
    fOutputList2->Add(fhistmcprimfinal);

    fhmcprimvzeta=new TH2D("fhmcprimvzeta","fhmcprimvzeta",200,-4,6,20,-10,10);
    fOutputList2->Add(fhmcprimvzeta);
    fhmcprimpdgcode=new TH1D("fhmcprimpdgcode","fhmcprimpdgcode",4000,-0.5,3999.5);
    fOutputList2->Add(fhmcprimpdgcode);
    fh2_FMD_acceptance_prim=new TH2D("fh2_FMD_acceptance_prim","fh2_FMD_acceptance_prim",200,-4,6,200,-10,10);
    fOutputList2->Add(fh2_FMD_acceptance_prim);
    fh2_FMD_eta_phi_prim=new TH2D("fh2_FMD_eta_phi_prim","fh2_FMD_eta_phi_prim",200,-4,6,20,0,2*TMath::Pi());
    fOutputList2->Add(fh2_FMD_eta_phi_prim);

    for(Int_t i=0;i<4;i++){
      fhrefetaFMD[i]=new TH1D(Form("fhrefetaFMD_%d",i),Form("fhrefetaFMD_%d",i),200,-4,6);
      fhrefphiFMD[i]=new TH1D(Form("fhrefphiFMD_%d",i),Form("fhrefphiFMD_%d",i),100,0,2*TMath::Pi());
      fOutputList2->Add(fhrefetaFMD[i]);
      fOutputList2->Add(fhrefphiFMD[i]);
    }
  }

  const Int_t ifmdbin[4]={200,20,11,20};
  if(fAnaMode=="TPCFMD" || fAnaMode=="TPCFMDC" || fAnaMode=="FMDFMD"){
    fFMDV0 = new TH2F("FMDV0", "FMD vs V0 pre cut;FMD;V0;",2000, 0, 2000, 2000, 0, 2000);
    fOutputList2->Add(fFMDV0);
    fFMDV0_post=new TH2F("FMDV0_post", "FMD vs V0 post cut;FMD;V0;",2000, 0, 2000, 2000, 0, 2000);
    fOutputList2->Add(fFMDV0_post);
    fFMDV0A = new TH2F("FMDV0A", "FMD vs V0A;FMD;V0A;",1000, 0, 1000, 1000, 0, 1000);
    fOutputList2->Add(fFMDV0A);
    fFMDV0A_post = new TH2F("FMDV0A_post", "FMD vs V0A post cut;FMD;V0A;",1000, 0, 1000, 1000, 0, 1000);
    fOutputList2->Add(fFMDV0A_post);
    fFMDV0C = new TH2F("FMDV0C", "FMD vs V0C;FMD;V0C;",1000, 0, 1000, 1000, 0, 1000);
    fOutputList2->Add(fFMDV0C);
    fFMDV0C_post = new TH2F("FMDV0C_post", "FMD vs V0C post cut;FMD;V0C;",1000, 0, 1000, 1000, 0, 1000);
    fOutputList2->Add(fFMDV0C_post);

    fh2_FMD_acceptance=new TH2D("fh2_FMD_acceptance","fh2_FMD_acceptance",200,-4,6,200,-10,10);
    fOutputList2->Add(fh2_FMD_acceptance);
    fh2_FMD_eta_phi=new TH2D("fh2_FMD_eta_phi","fh2_FMD_eta_phi",200,-4,6,20,0,2*TMath::Pi());
    fOutputList2->Add(fh2_FMD_eta_phi);

    fhistfmdphiacc=new TH2D("fhistfmdphiacc","fhistfmdphiacc",200,-4,6,20,0,100);
    fOutputList2->Add(fhistfmdphiacc);

    
    fhistfmd=new AliTHn("fhistfmd","fhistfmd",1,4,ifmdbin);
    fhistfmd->SetBinLimits(0,-4.,6.);
    fhistfmd->SetBinLimits(1,0.,2*TMath::Pi());
    fhistfmd->SetBinLimits(2,centBins_mcprim);
    fhistfmd->SetBinLimits(3,-10.,10.);
    fhistfmd->SetVarTitle(0,"eta");
    fhistfmd->SetVarTitle(1,"phi");
    fhistfmd->SetVarTitle(2,"centrality");
    fhistfmd->SetVarTitle(3,"zvertex");
    fOutputList2->Add(fhistfmd);
  }


 if(fasso=="PID"){
   for(Int_t i=0;i<6;i++){
     fHistNsig[i]=new TH2D(Form("fHistNsig_%d",i),Form("HistNsig_%d",i), 160, 0., 8., 600, -30., 30);
     fOutputList2->Add(fHistNsig[i]);
     fHistNsigcorr[i]=new TH2D(Form("fHistNsigcorr_%d",i),"fHistNsigcorr",500,-10,10,500,-10,10);
     fOutputList2->Add(fHistNsigcorr[i]);
   }
 }
 
}

void AliAnalysisTaskSEpPbCorrelationsForward::DefineCorrOutput() {


  Double_t binning_pt_assoc[13] = {0, 0.2, 0.5, 0.75, 1.0, 1.25, 1.5,
                                    2.0, 2.5, 3.0,  3.5, 4.0,  8.0};
  Double_t binning_pt_lead[13] = {0, 0.2, 0.5, 0.75, 1.0, 1.25, 1.5,
                                   2.0, 2.5, 3.0,  3.5, 4.0,  8.0};
   Double_t binning_cent[12] = {0., 5.,  10., 20.,
                                30., 40., 50., 60., 70., 80., 90., 100.1};
   Double_t binning_deta[49] = {
       -2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5,
       -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5,
       -0.4, -0.3, -0.2, -0.1, 0,    0.1,  0.2,  0.3,  0.4,  0.5,
       0.6,  0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.3,  1.4,  1.5,
       1.6,  1.7,  1.8,  1.9,  2.0,  2.1,  2.2,  2.3,  2.4};

   Double_t binning_deta_TPCTPC[33] = {
       -1.6, -1.5,
       -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5,
       -0.4, -0.3, -0.2, -0.1, 0,    0.1,  0.2,  0.3,  0.4,  0.5,
       0.6,  0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.3,  1.4,  1.5,
       1.6};

   Double_t binning_dphi[73] = {
       -1.570796, -1.483530, -1.396263, -1.308997, -1.221730, -1.134464,
       -1.047198, -0.959931, -0.872665, -0.785398, -0.698132, -0.610865,
       -0.523599, -0.436332, -0.349066, -0.261799, -0.174533, -0.087266,
       0.0,       0.087266,  0.174533,  0.261799,  0.349066,  0.436332,
       0.523599,  0.610865,  0.698132,  0.785398,  0.872665,  0.959931,
       1.047198,  1.134464,  1.221730,  1.308997,  1.396263,  1.483530,
       1.570796,  1.658063,  1.745329,  1.832596,  1.919862,  2.007129,
       2.094395,  2.181662,  2.268928,  2.356194,  2.443461,  2.530727,
       2.617994,  2.705260,  2.792527,  2.879793,  2.967060,  3.054326,
       3.141593,  3.228859,  3.316126,  3.403392,  3.490659,  3.577925,
       3.665191,  3.752458,  3.839724,  3.926991,  4.014257,  4.101524,
       4.188790,  4.276057,  4.363323,  4.450590,  4.537856,  4.625123,
       4.712389};
   const Int_t nEvtVars = 2;
   const Int_t iEvtBin[2] = {12, 11};

   Int_t nCFStepstrig=1;
   if(fasso=="hadron")  nCFStepstrig=1;
   else  if (fasso == "V0" || fasso == "Phi")    nCFStepstrig = 7;
   else  if (fasso == "Cascade")    nCFStepstrig = 6;
   else  if(fasso=="PID")    nCFStepstrig=3;

   Double_t binning_eta_vzero[11]={-3.7,-3.2,-2.7,-2.2,-1.7,0.,2.8,3.4,3.9,4.5,5.1};
   Double_t binning_phi_vzero[9]={0.,0.7853,1.5707,2.3561,3.1415,3.9269,4.7123,5.4977,6.2831};
   //Bins for FMD
   const Double_t binning_etafmd[33]={
     1.7,1.8,1.9,
     2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,
     3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,
     4.0,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9};
   const Double_t binning_etafmdc[18]={
     -3.4,-3.3,-3.2,-3.1,-3.0,
     -2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,
     -1.9,-1.8,-1.7};
   Double_t binning_zvx[11] = {-10,-8,-6,-4,-2,0,2,4,6,8,10};
   if(fAnaMode=="FMDFMD"){
     const Int_t nEvtVarsV0Leading=3;
     const Int_t iEvtBinV0Leading[3]={11,32,20};
     fHistTriggerTrack= new AliTHn("fHistTriggerTrack", "fHistTriggerTrack", nCFStepstrig, nEvtVarsV0Leading, iEvtBinV0Leading);
     fHistTriggerTrack->SetBinLimits(0,binning_cent);
     if(fAnaMode=="SECA" || fAnaMode=="FMDFMD") fHistTriggerTrack->SetBinLimits(1,binning_etafmd);
     else  if(fAnaMode=="SECC") fHistTriggerTrack->SetBinLimits(1,binning_etafmdc);
     fHistTriggerTrack->SetBinLimits(2,-10,10);
     fHistTriggerTrack->SetVarTitle(0,"centrality");
     fHistTriggerTrack->SetVarTitle(1,"eta");
     fHistTriggerTrack->SetVarTitle(2,"z vertex");
     const Int_t nEvtVarsV0Leadingmix=2;
     const Int_t iEvtBinV0Leadingmix[2]={11,32};
     fHistTriggerTrackMix= new AliTHn("fHistTriggerTrackMix", "fHistTriggerTrackMix", nCFStepstrig, nEvtVarsV0Leadingmix, iEvtBinV0Leadingmix);
     fHistTriggerTrackMix->SetBinLimits(0,binning_cent);
     if(fAnaMode=="SECA" || fAnaMode=="FMDFMD")    fHistTriggerTrackMix->SetBinLimits(1,binning_etafmd);
     else if(fAnaMode=="SECC")   fHistTriggerTrackMix->SetBinLimits(1,binning_etafmdc);

     fHistTriggerTrackMix->SetVarTitle(0,"centrality");
     fHistTriggerTrackMix->SetVarTitle(1,"eta");
     //    fHistTriggerTrackMiadx->SetVarTitle(2,"z vertex");
   }else if(fAnaMode=="TPCFMD" ||fAnaMode=="TPCFMDC"||fAnaMode=="ITSFMD" || fAnaMode=="ITSFMDC"){
     const Int_t nEvtVarsFMD = 4;
     Int_t netabin;
	 if(fAnaMode=="TPCFMD"||fAnaMode=="TPCFMDC")	   netabin=8;
	 else    netabin=18;
	 const Int_t iEvtBinFMD[4] = {12,11,20,netabin};
	 Double_t binning_eta_tpcfmd[9]={-0.8,-0.6,-0.4,-0.2,0.,0.2,0.4,0.6,0.8};
	 Double_t binning_eta_itsfmd[19]={-1.7, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.,0.2,  0.4,  0.6,  0.8,  1.0,  1.2,  1.4,  1.6,  1.7};
  
     fHistTriggerTrack = new AliTHn("fHistTriggerTrack", "fHistTriggerTrack", nCFStepstrig, nEvtVarsFMD, iEvtBinFMD);
     fHistTriggerTrack->SetBinLimits(0, binning_pt_lead);
     fHistTriggerTrack->SetBinLimits(1, binning_cent);
     fHistTriggerTrack->SetBinLimits(2, -10.,10.);

     if(fAnaMode=="TPCFMD"||fAnaMode=="TPCFMDC")	   fHistTriggerTrack->SetBinLimits(3, binning_eta_tpcfmd);
     else fHistTriggerTrack->SetBinLimits(3, binning_eta_itsfmd); 
     
     fHistTriggerTrack->SetVarTitle(0, "leading p_{T} GeV/c");
     fHistTriggerTrack->SetVarTitle(1, "centrality");
     fHistTriggerTrack->SetVarTitle(2, "zvertex");
     fHistTriggerTrack->SetVarTitle(3, "TPC/Eta eta");

          fHistTriggerTrack1 = new AliTHn("fHistTriggerTrack1", "fHistTriggerTrack1", nCFStepstrig, nEvtVarsFMD, iEvtBinFMD);
     fHistTriggerTrack1->SetBinLimits(0, binning_pt_lead);
     fHistTriggerTrack1->SetBinLimits(1, binning_cent);
     fHistTriggerTrack1->SetBinLimits(2, -10.,10.);
     

          if(fAnaMode=="TPCFMD"||fAnaMode=="TPCFMDC")	   fHistTriggerTrack1->SetBinLimits(3, binning_eta_tpcfmd);
     else fHistTriggerTrack1->SetBinLimits(3, binning_eta_itsfmd); 
	  
     fHistTriggerTrack1->SetVarTitle(0, "leading p_{T} GeV/c");
     fHistTriggerTrack1->SetVarTitle(1, "centrality");
     fHistTriggerTrack1->SetVarTitle(2, "zvertex");
     fHistTriggerTrack1->SetVarTitle(3, "TPC/Eta eta");

     fHistTriggerTrackMix = new AliTHn("fHistTriggerTrackMix", "fHistTriggerTrackMix", nCFStepstrig, nEvtVars, iEvtBin);
     fHistTriggerTrackMix->SetBinLimits(0, binning_pt_lead);
     fHistTriggerTrackMix->SetBinLimits(1, binning_cent);
     fHistTriggerTrackMix->SetVarTitle(0, "leading p_{T} GeV/c");
     fHistTriggerTrackMix->SetVarTitle(1, "centrality");
     fOutputList1->Add(fHistTriggerTrack1);
   }else{
     fHistTriggerTrack = new AliTHn("fHistTriggerTrack", "fHistTriggerTrack", nCFStepstrig, nEvtVars, iEvtBin);
     fHistTriggerTrack->SetBinLimits(0, binning_pt_lead);
     fHistTriggerTrack->SetBinLimits(1, binning_cent);
     fHistTriggerTrack->SetVarTitle(0, "leading p_{T} GeV/c");
     fHistTriggerTrack->SetVarTitle(1, "centrality");

     fHistTriggerTrackMix = new AliTHn("fHistTriggerTrackMix", "fHistTriggerTrackMix", nCFStepstrig, nEvtVars, iEvtBin);
     fHistTriggerTrackMix->SetBinLimits(0, binning_pt_lead);
     fHistTriggerTrackMix->SetBinLimits(1, binning_cent);
     fHistTriggerTrackMix->SetVarTitle(0, "leading p_{T} GeV/c");
     fHistTriggerTrackMix->SetVarTitle(1, "centrality");
   }
   fOutputList1->Add(fHistTriggerTrack);
   fOutputList1->Add(fHistTriggerTrackMix);

   //////////////////////////////////////////
   //Containers two particle correlation
   //////////////////////////////////////////

   Int_t nCFSteps = 1;
   if(fasso=="hadron")    nCFSteps=1;
   else if (fasso == "V0" || fasso == "Phi")    nCFSteps = 7;
   else  if (fasso == "Cascade")    nCFSteps = 6;
   else    if(fasso=="PID")    nCFSteps=3;
   Double_t binning_dphi_vzero[9]={-1.178097,-0.392699,0.392699,1.178097,1.963495,2.748893,3.534291,4.319689,5.105088};
   if(fAnaMode=="TPCTPC") {
	 Double_t binning_cent_tpctpc[7] = {0., 5.,  10., 20.,
                                 40.,  60., 100.1};

	 const Int_t iTrackBin_TPCTPC[6] = {32, 12, 12, 6, 72, 20};
     fHistReconstTrack = new AliTHn("fHistReconstTrack", "fHistReconstTrack", nCFSteps, 6, iTrackBin_TPCTPC);
     fHistReconstTrack->SetBinLimits(0, binning_deta_TPCTPC);
     fHistReconstTrack->SetBinLimits(1, binning_pt_assoc);
     fHistReconstTrack->SetBinLimits(2, binning_pt_lead);
     fHistReconstTrack->SetBinLimits(3, binning_cent_tpctpc);
     fHistReconstTrack->SetBinLimits(4, binning_dphi);
     fHistReconstTrack->SetBinLimits(5, -10,10);
     fHistReconstTrack->SetVarTitle(0, "#Delta#eta");
     fHistReconstTrack->SetVarTitle(1, "p_{T} GeV/c");
     fHistReconstTrack->SetVarTitle(2, "leading p_{T} GeV/c");
     fHistReconstTrack->SetVarTitle(3, "centrality");
     fHistReconstTrack->SetVarTitle(4, "#Delta#phi");
     fHistReconstTrack->SetVarTitle(5, "Vz");
     fHistReconstTrackMix =  new AliTHn("fHistReconstTrackMix", "fHistReconstTrackMix", nCFSteps, 6, iTrackBin_TPCTPC);
     fHistReconstTrackMix->SetBinLimits(0, binning_deta_TPCTPC);
     fHistReconstTrackMix->SetBinLimits(1, binning_pt_assoc);
     fHistReconstTrackMix->SetBinLimits(2, binning_pt_lead);
     fHistReconstTrackMix->SetBinLimits(3, binning_cent_tpctpc);
     fHistReconstTrackMix->SetBinLimits(4, binning_dphi);
     fHistReconstTrackMix->SetBinLimits(5, -10,10);
     fHistReconstTrackMix->SetVarTitle(0, "#Delta#eta");
     fHistReconstTrackMix->SetVarTitle(1, "p_{T} GeV/c");
     fHistReconstTrackMix->SetVarTitle(2, "leading p_{T} GeV/c");
     fHistReconstTrackMix->SetVarTitle(3, "centrality");
     fHistReconstTrackMix->SetVarTitle(4, "#Delta#phi");
     fHistReconstTrackMix->SetVarTitle(5, "Vz");
   }else if (fAnaMode=="TPCFMD" || fAnaMode=="TPCFMDC"){
       Double_t binning_detaFMDTPC[49]={
		 -5.7,-5.6,-5.5,-5.4,-5.3,-5.2,-5.1,-5.0,
		 -4.9,-4.8,-4.7,-4.6,-4.5,-4.4,-4.3,-4.2,-4.1,-4.,
		 -3.9,-3.8,-3.7,-3.6,-3.5,-3.4,-3.3,-3.2,-3.1,-3.,
		 -2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.,
		 -1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.,
		 -0.9};
	   Double_t binning_detaFMDCTPC[34]={
		 0.9,
		 1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,
		 2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,
		 3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,
		 4.0,4.1,4.2};
       Double_t binning_pt_fmdtpc[2]={0,4};
       //      const Int_t iTrackBin_tpcfmd[6]={48,11,32,15,72,12};
	   Int_t ndetatpcfmd;
	   Int_t nfmdbin;
	   if(fAnaMode=="TPCFMD") {
		 ndetatpcfmd=48;
		 nfmdbin=32;
	   }else if(fAnaMode=="TPCFMDC") {
		 ndetatpcfmd=33;
		 nfmdbin=17;
	   }
	   const Double_t binning_cent_fmdfmd[7]={0.,5.,10.,20.,40.,60.,100.1};
	   const Int_t iTrackBin_tpcfmd[7]={ndetatpcfmd,1,nfmdbin,6,72,10,4};
	   //	   const Int_t iTrackBin_tpcfmdc[7]={33,1,17,11,72,20,1};
	   fHistReconstTrack = new AliTHn("fHistReconstTrack", "fHistReconstTrack", nCFSteps, 7, iTrackBin_tpcfmd);
	   fHistReconstTrackMix= new AliTHn("fHistReconstTrackMix", "fHistReconstTrackMix", nCFSteps, 7,iTrackBin_tpcfmd);
	   fHistReconstTrack1 = new AliTHn("fHistReconstTrack1", "fHistReconstTrack1", nCFSteps, 7, iTrackBin_tpcfmd);
	   fHistReconstTrackMix1= new AliTHn("fHistReconstTrackMix1", "fHistReconstTrackMix1", nCFSteps, 7,iTrackBin_tpcfmd);
	   if(fAnaMode=="TPCFMD") {
		 fHistReconstTrack->SetBinLimits(0,binning_detaFMDTPC);
		 fHistReconstTrack->SetBinLimits(2,binning_etafmd);
		 //Mixed Events
		 fHistReconstTrackMix->SetBinLimits(0,binning_detaFMDTPC);
		 fHistReconstTrackMix->SetBinLimits(2,binning_etafmd);
		 fHistReconstTrack1->SetBinLimits(0,binning_detaFMDTPC);
		 fHistReconstTrack1->SetBinLimits(2,binning_etafmd);
		 //Mixed Events
		 fHistReconstTrackMix1->SetBinLimits(0,binning_detaFMDTPC);
		 fHistReconstTrackMix1->SetBinLimits(2,binning_etafmd);
	   } else if(fAnaMode=="TPCFMDC") { 
		 fHistReconstTrack->SetBinLimits(0,binning_detaFMDCTPC);
		 fHistReconstTrack->SetBinLimits(2,binning_etafmdc);
		 //Mixed Events
		 fHistReconstTrackMix->SetBinLimits(0,binning_detaFMDCTPC);
		 fHistReconstTrackMix->SetBinLimits(2,binning_etafmdc);
		 fHistReconstTrack1->SetBinLimits(0,binning_detaFMDCTPC);
		 fHistReconstTrack1->SetBinLimits(2,binning_etafmdc);
		 //Mixed Events
		 fHistReconstTrackMix1->SetBinLimits(0,binning_detaFMDCTPC);
		 fHistReconstTrackMix1->SetBinLimits(2,binning_etafmdc);

	   }
	   fHistReconstTrack->SetBinLimits(1,binning_pt_fmdtpc);
	   fHistReconstTrack->SetBinLimits(3,binning_cent_fmdfmd);
	   fHistReconstTrack->SetBinLimits(4,binning_dphi);
	   fHistReconstTrack->SetBinLimits(5,-10.,10.);
	   fHistReconstTrack->SetBinLimits(6,-0.8,0.8);
	   fHistReconstTrack->SetVarTitle(0,"#Delta#eta");
	   fHistReconstTrack->SetVarTitle(1,"p_{T} GeV/c");
	   fHistReconstTrack->SetVarTitle(2,"FMD Eta");
	   fHistReconstTrack->SetVarTitle(3,"centrality");
	   fHistReconstTrack->SetVarTitle(4,"#Delta#phi");
	   fHistReconstTrack->SetVarTitle(5,"z vertex");
	   fHistReconstTrack->SetVarTitle(6,"TPC phi");
	   fHistReconstTrack1->SetBinLimits(1,binning_pt_fmdtpc);
	   fHistReconstTrack1->SetBinLimits(3,binning_cent_fmdfmd);
	   fHistReconstTrack1->SetBinLimits(4,binning_dphi);
	   fHistReconstTrack1->SetBinLimits(5,-10.,10.);
	   fHistReconstTrack1->SetBinLimits(6,-0.8,0.8);
	   fHistReconstTrack1->SetVarTitle(0,"#Delta#eta");
	   fHistReconstTrack1->SetVarTitle(1,"p_{T} GeV/c");
	   fHistReconstTrack1->SetVarTitle(2,"FMD Eta");
	   fHistReconstTrack1->SetVarTitle(3,"centrality");
	   fHistReconstTrack1->SetVarTitle(4,"#Delta#phi");
	   fHistReconstTrack1->SetVarTitle(5,"z vertex");
	   fHistReconstTrack1->SetVarTitle(6,"TPC phi");
	   
	   fHistReconstTrackMix->SetBinLimits(1,binning_pt_fmdtpc);
	   fHistReconstTrackMix->SetBinLimits(3,binning_cent_fmdfmd);
	   fHistReconstTrackMix->SetBinLimits(4,binning_dphi);
	   fHistReconstTrackMix->SetBinLimits(5,-10.,10.);
	   fHistReconstTrackMix->SetBinLimits(6,-0.8,0.8);
	   fHistReconstTrackMix->SetVarTitle(0,"#Delta#eta");
	   fHistReconstTrackMix->SetVarTitle(1,"p_{T} GeV/c");
	   fHistReconstTrackMix->SetVarTitle(2,"FMD Eta");
	   fHistReconstTrackMix->SetVarTitle(3,"centrality");
	   fHistReconstTrackMix->SetVarTitle(4,"#Delta#phi");
	   fHistReconstTrackMix->SetVarTitle(5,"z vertex");
	   fHistReconstTrackMix->SetVarTitle(6,"TPC phi");
	   fHistReconstTrackMix1->SetBinLimits(1,binning_pt_fmdtpc);
	   fHistReconstTrackMix1->SetBinLimits(3,binning_cent_fmdfmd);
	   fHistReconstTrackMix1->SetBinLimits(4,binning_dphi);
	   fHistReconstTrackMix1->SetBinLimits(5,-10.,10.);
	   fHistReconstTrackMix1->SetBinLimits(6,-0.8,0.8);
	   fHistReconstTrackMix1->SetVarTitle(0,"#Delta#eta");
	   fHistReconstTrackMix1->SetVarTitle(1,"p_{T} GeV/c");
	   fHistReconstTrackMix1->SetVarTitle(2,"FMD Eta");
	   fHistReconstTrackMix1->SetVarTitle(3,"centrality");
	   fHistReconstTrackMix1->SetVarTitle(4,"#Delta#phi");
	   fHistReconstTrackMix1->SetVarTitle(5,"z vertex");
	   fHistReconstTrackMix1->SetVarTitle(6,"TPC phi");
	      fOutputList1->Add(fHistReconstTrack1);
	      fOutputList1->Add(fHistReconstTrackMix1);
   
   }else if(fAnaMode=="FMDFMD"){
     const Int_t nTrackVars_fmdfmd = 6;
     const Double_t binning_cent_fmdfmd[7]={0.,5.,10.,20.,40.,60.,100.1};
     const Int_t iTrackBin_fmdfmd[6]={49,17,32,6,20,20};
     fHistReconstTrack= new AliTHn("fHistReconstTrack", "fHistReconstTrack", nCFSteps, nTrackVars_fmdfmd,iTrackBin_fmdfmd);
     fHistReconstTrack->SetBinLimits(0,3.425,8.325);
     //    fHistReconstTrack->SetBinLimits(0,3.525,8.325);
     fHistReconstTrack->SetBinLimits(1,binning_etafmdc);
     fHistReconstTrack->SetBinLimits(2,binning_etafmd);
     fHistReconstTrack->SetBinLimits(3,binning_cent_fmdfmd);
     //   fHistReconstTrack->SetBinLimits(4,-0.551*TMath::Pi(),1.449*TMath::Pi());
     fHistReconstTrack->SetBinLimits(4,-0.55*TMath::Pi(),1.45*TMath::Pi());
     fHistReconstTrack->SetBinLimits(5,-10,10);
     fHistReconstTrack->SetVarTitle(0,"#Delta#eta");
     fHistReconstTrack->SetVarTitle(1,"FMD(Asso) Eta");
     fHistReconstTrack->SetVarTitle(2,"FMD(Trigger) Eta");
     fHistReconstTrack->SetVarTitle(3,"centrality");
     fHistReconstTrack->SetVarTitle(4,"#Delta#phi");
     fHistReconstTrack->SetVarTitle(5,"z vertex");
     fHistReconstTrackMix= new AliTHn("fHistReconstTrackMix", "fHistReconstTrackMix", nCFSteps, nTrackVars_fmdfmd,iTrackBin_fmdfmd);
     fHistReconstTrackMix->SetBinLimits(0,3.425,8.325);
     //fHistReconstTrackMix->SetBinLimits(0,3.525,8.325);
     fHistReconstTrackMix->SetBinLimits(1,binning_etafmdc);
     fHistReconstTrackMix->SetBinLimits(2,binning_etafmd);
     fHistReconstTrackMix->SetBinLimits(3,binning_cent_fmdfmd);
     //    fHistReconstTrackMix->SetBinLimits(4,-0.551*TMath::Pi(),1.449*TMath::Pi());   
     fHistReconstTrackMix->SetBinLimits(4,-0.55*TMath::Pi(),1.45*TMath::Pi());
     fHistReconstTrackMix->SetBinLimits(5,-10,10);
     fHistReconstTrackMix->SetVarTitle(0,"#Delta#eta");
     fHistReconstTrackMix->SetVarTitle(1,"FMD(Asso) Eta");
     fHistReconstTrackMix->SetVarTitle(2,"FMD(Trigger) Eta");
     fHistReconstTrackMix->SetVarTitle(3,"centrality");
     fHistReconstTrackMix->SetVarTitle(4,"#Delta#phi");
     fHistReconstTrackMix->SetVarTitle(5,"z vertex");
    }else if(fAnaMode=="ITSFMD" || fAnaMode=="ITSFMDC"){
       Double_t binning_detaFMDITS[66]={
		 -6.6,-6.5,-6.4,-6.3,-6.2,-6.1,-6.0,
		 -5.9,-5.8,-5.7,-5.6,-5.5,-5.4,-5.3,-5.2,-5.1,-5.0,
		 -4.9,-4.8,-4.7,-4.6,-4.5,-4.4,-4.3,-4.2,-4.1,-4.,
		 -3.9,-3.8,-3.7,-3.6,-3.5,-3.4,-3.3,-3.2,-3.1,-3.,
		 -2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.,
		 -1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.,
		 -0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1};
	   Double_t binning_detaFMDCITS[50]={
		 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,
		 1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,
		 2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,
		 3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,
		 4.0,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9,
		 5.0};
	   Double_t binning_dphi_itsfmd[37] = {
		 -1.570796,  -1.396263,  -1.221730, 
		 -1.047198,  -0.872665,  -0.698132, 
		 -0.523599,  -0.349066,  -0.174533,  
		 0.0,         0.174533,   0.349066,  
		 0.523599,    0.698132,   0.872665, 
		 1.047198,    1.221730,   1.396263, 
		 1.570796,    1.745329,   1.919862,  
		 2.094395,    2.268928,   2.443461,  
		 2.617994,    2.792527,   2.967060,  
		 3.141593,    3.316126,   3.490659,  
		 3.665191,    3.839724,   4.014257,  
		 4.188790,    4.363323,   4.537856,  
		 4.712389};
	   Double_t binning_cent_its[8]={0.,5.,10.,20.,40.,60.,80.,100.};
	   Int_t nbinitsfmddeltaeta=9999;
	   Int_t nbinetafmd=999;
	   if(fAnaMode=="ITSFMD"){
		 nbinitsfmddeltaeta=65;
		 nbinetafmd=32;
	   }else if(fAnaMode=="ITSFMDC"){
		 nbinitsfmddeltaeta=49;
		 nbinetafmd=17;
	   }
       const Int_t iTrackBin_tpcfmd[6]={nbinitsfmddeltaeta,7,nbinetafmd,36,20,6};
	   Double_t binning_zvx_fmdits[2]={-10,10};
	   //	   Double_t binning_itseta[19]={-1.7, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.,0.2,  0.4,  0.6,  0.8,  1.0,  1.2,  1.4,  1.6,  1.7};
	   Double_t binning_itseta[7]={-1.7,-1.2,-0.6,0.,0.6,1.2,1.7};

     fHistReconstTrack = new AliTHn("fHistReconstTrack", "fHistReconstTrack", nCFSteps, 6, iTrackBin_tpcfmd);
     if(fAnaMode=="ITSFMD")  {
	   fHistReconstTrack->SetBinLimits(0,binning_detaFMDITS);
	   fHistReconstTrack->SetBinLimits(2,binning_etafmd);
	 }else if(fAnaMode=="ITSFMDC") {
	   fHistReconstTrack->SetBinLimits(0,binning_detaFMDCITS);
	   fHistReconstTrack->SetBinLimits(2,binning_etafmdc);
	 }
     fHistReconstTrack->SetBinLimits(1,binning_cent_its);
     fHistReconstTrack->SetBinLimits(3,binning_dphi_itsfmd);
	 //     fHistReconstTrack->SetBinLimits(5,binning_zvx_fmdits);
     fHistReconstTrack->SetBinLimits(4,-10.,10.);
     fHistReconstTrack->SetBinLimits(5,binning_itseta);
     fHistReconstTrack->SetVarTitle(0,"#Delta#eta");
     fHistReconstTrack->SetVarTitle(2,"FMD Eta");
     fHistReconstTrack->SetVarTitle(1,"centrality");
     fHistReconstTrack->SetVarTitle(3,"#Delta#phi");
     fHistReconstTrack->SetVarTitle(4,"z vertex");
     fHistReconstTrack->SetVarTitle(5,"ITS Eta");

     fHistReconstTrackMix= new AliTHn("fHistReconstTrackMix", "fHistReconstTrackMix", nCFSteps, 6,iTrackBin_tpcfmd);
     if(fAnaMode=="ITSFMD") {
	   fHistReconstTrackMix->SetBinLimits(0,binning_detaFMDITS);
	   fHistReconstTrackMix->SetBinLimits(2,binning_etafmd);
	 }else if(fAnaMode=="ITSFMDC") {
	   fHistReconstTrackMix->SetBinLimits(0,binning_detaFMDCITS);
	   fHistReconstTrackMix->SetBinLimits(2,binning_etafmdc);
	 }
     fHistReconstTrackMix->SetBinLimits(1,binning_cent_its);
     fHistReconstTrackMix->SetBinLimits(3,binning_dphi_itsfmd);
	 //     fHistReconstTrackMix->SetBinLimits(5,binning_zvx_fmdits);
     fHistReconstTrackMix->SetBinLimits(4,-10.,10.);
     fHistReconstTrackMix->SetBinLimits(5,binning_itseta);
     fHistReconstTrackMix->SetVarTitle(0,"#Delta#eta");
     fHistReconstTrackMix->SetVarTitle(1,"centrality");
	 fHistReconstTrackMix->SetVarTitle(2,"FMD Eta");
     fHistReconstTrackMix->SetVarTitle(3,"#Delta#phi");
     fHistReconstTrackMix->SetVarTitle(4,"z vertex");
     fHistReconstTrackMix->SetVarTitle(5,"ITS Eta");
   }
   fOutputList1->Add(fHistReconstTrack);
   fOutputList1->Add(fHistReconstTrackMix);
}

void AliAnalysisTaskSEpPbCorrelationsForward::UserExec(Option_t *) {
  AliAnalysisManager *mgr        = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *inEvMain = (AliInputEventHandler *)(mgr->GetInputEventHandler());
  if (!inEvMain)    return;
  //  fPIDResponse = inEvMain->GetPIDResponse();
  //if (!fPIDResponse)    return;
  


  if(!fIsAOD){
	mctruth = (AliMCEventHandler*)(mgr->GetMCtruthEventHandler());
	if(!mctruth)       return;
	if (!mctruth->InitOk()) return;
	if (!mctruth->TreeK()) return;
	mctruth->CreateLabelMap();
	mcEvent=mctruth->MCEvent();//AliMCEvent
	mcEvent->InitEvent();
	mcEvent->PreReadAll();
  }
  
  if(fIsAOD){
	//    fEvent = dynamic_cast<AliAODEvent *>(inEvMain->GetEvent());
    fEvent =AliForwardUtil::GetAODEvent(this);
    if (!fEvent) {
      AliWarning("ERROR: fEvent not available \n");
      return;
    }
  }else{
    fESD=dynamic_cast<AliESDEvent*>(inEvMain->GetEvent());
    if (!fESD) {
      AliWarning("ERROR: fESD not available \n");
      return;
    }
  }
  
  inputEvent=fEvent;
  if(!inputEvent) inputEvent=fESD;
  fHist_Stat->Fill(0);
    
  if (!fEventCuts.AcceptEvent(inputEvent)) {
  PostData(1, fOutputList);
  PostData(2, fOutputList1);
  PostData(3, fOutputList2);
  return;
  }
  
  multSelection =    (AliMultSelection *)inputEvent->FindListObject("MultSelection");
  //multSelection =    (AliMultSelection *)fEvent->FindListObject("MultSelection");
  if(!multSelection) {
     PostData(1, fOutputList);
     PostData(2, fOutputList1);
     PostData(3, fOutputList2);
	 Printf("No multselection");
     return;
  }
  fHist_Stat->Fill(1);      

  UInt_t maskIsSelected = inEvMain->IsEventSelected();
  Bool_t isSelected     = kFALSE;
  isSelected = ((maskIsSelected & AliVEvent::kINT7) == AliVEvent::kINT7);//Both for data and MC
  if (!isSelected)  return;
  fHist_Stat->Fill(2);
  
  // Primary Vllllertex
  /*
  lPrimaryBestVtx = inputEvent->GetPrimaryVertex();
  if (!lPrimaryBestVtx)    return;
  Int_t nTracksPrim = lPrimaryBestVtx->GetNContributors();
  if (nTracksPrim < 1)    return;

  if ((TMath::Abs(lPrimaryBestVtx->GetZ())) >= fZVertex)    return;
  tPrimaryVtxPosition[0] = lPrimaryBestVtx->GetX();
  tPrimaryVtxPosition[1] = lPrimaryBestVtx->GetY();
  tPrimaryVtxPosition[2] = lPrimaryBestVtx->GetZ();
  fHistzvertex->Fill(tPrimaryVtxPosition[2]);
  fPrimaryZVtx = lPrimaryBestVtx->GetZ();
  fHist_Stat->Fill(3);
  */
 
  lPrimaryBestVtx = fEvent->GetPrimaryVertex();
  if ((TMath::Abs(lPrimaryBestVtx->GetZ())) >= fZVertex)   {
	PostData(1, fOutputList);
	PostData(2, fOutputList1);
	PostData(3, fOutputList2);
	return;
   }
  tPrimaryVtxPosition[0] = lPrimaryBestVtx->GetX();
  tPrimaryVtxPosition[1] = lPrimaryBestVtx->GetY();
  tPrimaryVtxPosition[2] = lPrimaryBestVtx->GetZ();
  
   fPrimaryZVtx = lPrimaryBestVtx->GetZ();
   fHist_Stat->Fill(3);
   
   
   bSign = 0.;
   bSign = (InputEvent()->GetMagneticField() > 0) ? 1 : -1;

   if(fcollisiontype=="pPb" || fcollisiontype=="PbPb"){
    if(frun2){
      //AliMultSelection *multSelection =    (AliMultSelection *)fEvent->FindListObject("MultSelection");
      lCentrality = multSelection->GetMultiplicityPercentile(fCentType);
      Int_t qual = multSelection->GetEvSelCode();
      if (qual == 199)  lCentrality = -999;
    } else{
      AliCentrality *centobj = 0;
      //centobj = fEvent->GetCentrality();
      centobj = inputEvent->GetCentrality();
      lCentrality = centobj->GetCentralityPercentile(fCentType);
      if(!centobj) lCentrality=-1.;
    }
    if (lCentrality < 0. || lCentrality > 100. - 0.0000001)   return;
    Double_t *CentBins = fCentBins;
    poolmin = CentBins[0];
    poolmax = CentBins[fNCentBins];
    fHist_Stat->Fill(4);
   }else{
    //    AliMultSelection *multSelection =    (AliMultSelection *)fEvent->FindListObject("MultSelection");
    // if (!multSelection) {
	///  AliWarning ("AliMultSelection could not be found in the aod event list of objects");
    //  }
  }
   
   /*
  AliAnalysisUtils *fUtils=new AliAnalysisUtils;
  //if(fcollisiontype=="pp") if(fUtils->IsPileUpSPD(fEvent)) return;
  if(fUtils->IsPileUpMV(inputEvent)) return;
  //if(fUtils->IsPileUpSPD(fEvent)) return;
  //  if(fEvent->IsPileupFromSPD(5,0.8,3.,2.,5.)) return;
  fHist_Stat->Fill(5);
  // SPD vertex selection
  const AliVVertex* vtxSPD = dynamic_cast<const AliVVertex*>(inputEvent->GetPrimaryVertexSPD());
  Double_t dMaxResol = 0.25; // suggested from DPG
  Double_t cov[6] = {0};
  vtxSPD->GetCovarianceMatrix(cov);
  Double_t zRes = TMath::Sqrt(cov[5]);
  if ( vtxSPD->IsFromVertexerZ() && (zRes > dMaxResol)) return;
  fHist_Stat->Fill(6);
  //if(fNEntries==2) return;
  //  if(fcollisiontype=="pPb" ||  fHistCentrality->Fill(lCentrality);
  */
  
  fHistCentrality->Fill(lCentrality);
  MakeAna();
  fNEntries++;
  PostData(1, fOutputList);
  PostData(2, fOutputList1);
  PostData(3, fOutputList2);
}

void AliAnalysisTaskSEpPbCorrelationsForward::Terminate(Option_t *) {
  //  AliInfo(Form("Number of Correlation
  Printf("Entries======================%f",fNEntries);
  if (fPoolMgr)    delete fPoolMgr;   // PoolMgr->ClearPools();
  if (fPoolMgr1)    delete fPoolMgr1; // fPoolMgr1->ClearPools();
}

void AliAnalysisTaskSEpPbCorrelationsForward::MakeAna() {

  TObjArray *selectedTracksLeading = new TObjArray;
  selectedTracksLeading->SetOwner(kTRUE);
  TObjArray *selectedTracksAssociated = new TObjArray;
  selectedTracksAssociated->SetOwner(kTRUE);
    
  TObjArray* selectedTracksMC1=new TObjArray;
  selectedTracksMC1->SetOwner(kTRUE);
  TObjArray* selectedTracksMC2=new TObjArray;
  selectedTracksMC2->SetOwner(kTRUE);

  // Leading Particle
  fvzero = fEvent->GetVZEROData();
  if(fAnaMode=="TPCV0A"||fAnaMode=="TPCV0C"||fAnaMode=="V0AV0C"){
  Double_t eta_min;
  Double_t eta_max;
  Double_t eta_ave;
  Double_t phi_vzero;
  Double_t mult_vzero;
  Double_t vzeroqa[3];
  Double_t mult_vzero_eq;
  for (Int_t imod = 0; imod < 64; imod++) {
    eta_min = fvzero->GetVZEROEtaMin(imod);
    eta_max = fvzero->GetVZEROEtaMax(imod);
    phi_vzero = fvzero->GetVZEROAvgPhi(imod);
    mult_vzero = fvzero->GetMultiplicity(imod);
    mult_vzero_eq = fEvent->GetVZEROEqMultiplicity(imod);
    eta_ave = (eta_min + eta_max) / 2.;
    fHist_vzeromult->Fill(imod, mult_vzero);
    fHist_vzeromultEqweighted->Fill(imod, mult_vzero_eq);
    fHist2dmult->Fill(imod, mult_vzero_eq, mult_vzero);
    vzeroqa[0] = eta_ave;
    vzeroqa[1] = phi_vzero;
    vzeroqa[2] = lCentrality;
    if (fQA)   fHistVZERO->Fill(vzeroqa, 0, (Double_t)mult_vzero_eq);
     if(imod>31) {
	   if(fAnaMode=="TPCV0A") selectedTracksAssociated->Add(new AliAssociatedTrackYS(-999,eta_ave,phi_vzero,-999,-999,-999,-999,-999,mult_vzero_eq));
	   if(fAnaMode=="V0AV0C")selectedTracksLeading->Add(new AliAssociatedTrackYS(-999,eta_ave,phi_vzero,-999,-999,-999,-999,-999,mult_vzero_eq));
	   //selectedTrackV0A->Add(new AliAssociatedVZEROYS(mult_vzero_eq,eta_ave,phi_vzero,0.0,0,0));  
	 }else if(imod<32) {
	   if(fAnaMode=="TPCV0C" ||fAnaMode=="V0AV0C")selectedTracksAssociated->Add(new AliAssociatedTrackYS(-999,eta_ave,phi_vzero,-999,-999,-999,-999,-999,mult_vzero_eq));
	 }
  }
  }


if(fAnaMode=="TPCFMD" || fAnaMode=="TPCFMDC" || fAnaMode=="FMDFMD"){
  Float_t nFMD_fwd_hits=0;
  Float_t nFMD_bwd_hits=0;
  AliAODForwardMult*aodForward=static_cast<AliAODForwardMult*>(fEvent->FindListObject("Forward"));
   // Shape of d2Ndetadphi: 200, -4, 6, 20, 0, 2pi
  //   Int_t ivzbin=frefvz->GetXaxis()->FindBin(fPrimaryZVtx);
   const TH2D& d2Ndetadphi = aodForward->GetHistogram();
   TH1*hphiacceptance=aodForward->GetPhiAcceptance();
   Int_t nEta = d2Ndetadphi.GetXaxis()->GetNbins();
   Int_t nPhi = d2Ndetadphi.GetYaxis()->GetNbins();
   Double_t pt = 0;
   for (Int_t iEta = 1; iEta <= nEta; iEta++) {
	 Int_t valid = Int_t(d2Ndetadphi.GetBinContent(iEta, 0));
	 if (!valid) {
	   continue;
	 }
	 Float_t eta = d2Ndetadphi.GetXaxis()->GetBinCenter(iEta);
	 Float_t phiacc=hphiacceptance->GetBinContent(iEta);
	 fhistfmdphiacc->Fill(eta,lCentrality,phiacc);
	 
	 for (Int_t iPhi = 1; iPhi <= nPhi; iPhi++) {
	   // Bin content is most likely number of particles!
	   Float_t phi = d2Ndetadphi.GetYaxis()->GetBinCenter(iPhi);
	   
	   Float_t mostProbableN = d2Ndetadphi.GetBinContent(iEta, iPhi);
	   fh2_FMD_acceptance->Fill(eta,tPrimaryVtxPosition[2],mostProbableN);
	   //           Float_t corrfactor=fhcorr[ivzbin-1]->GetBinContent(iEta,iPhi);
	   if (mostProbableN > 0) {
		 if(eta>0){
		   if(fAnaMode=="TPCFMD" || fAnaMode=="ITSFMD") selectedTracksAssociated->Add(new AliAssociatedTrackYS(-999,eta,phi,-999,-999,-999,-999,-999,mostProbableN));			
		   if(fAnaMode=="FMDFMD") selectedTracksLeading->Add(new AliAssociatedTrackYS(-999,eta,phi,-999,-999,-999,-999,-999,mostProbableN));			
		   //   selectedFMDArray1->Add(new AliAssociatedVZEROYS(mostProbableN,eta,phi,0,0,0));
		   nFMD_fwd_hits+=mostProbableN;
		 }else if(eta<0){
		   if(fAnaMode=="TPCFMDC" || fAnaMode=="ITSFMDC" ||fAnaMode=="FMDFMD") selectedTracksAssociated->Add(new AliAssociatedTrackYS(-999,eta,phi,-999,-999,-999,-999,-999,mostProbableN));  
		   //selectedFMDArray2->Add(new AliAssociatedVZEROYS(mostProbableN,eta,phi,0,0,0));
		   nFMD_bwd_hits+=mostProbableN;
		 }
		 Double_t cont[4]={eta,phi,lCentrality,tPrimaryVtxPosition[2]};
		 fhistfmd->Fill(cont,0,mostProbableN);
		 fh2_FMD_eta_phi->Fill(eta,phi,mostProbableN);
	   }
	 }
   }
   Float_t nV0A_hits = fvzero->GetMTotV0A();
   Float_t nV0C_hits = fvzero->GetMTotV0C();
   fFMDV0->Fill(nFMD_bwd_hits + nFMD_fwd_hits, nV0C_hits + nV0A_hits);
   fFMDV0A->Fill(nFMD_fwd_hits, nV0A_hits);
   fFMDV0C->Fill(nFMD_bwd_hits, nV0C_hits);

   if(fcollisiontype=="pPb"){
	 //	 if (nV0A_hits + nV0C_hits < 1.5*(nFMD_fwd_hits + nFMD_bwd_hits) - 20) return; //events cuts
   }else if(fcollisiontype=="PbPb"){
     if (nV0A_hits + nV0C_hits < 1.5*(nFMD_fwd_hits + nFMD_bwd_hits)) return; //events cuts
   }
   fFMDV0_post->Fill(nFMD_bwd_hits + nFMD_fwd_hits, nV0C_hits + nV0A_hits);
   fFMDV0A_post->Fill(nFMD_fwd_hits, nV0A_hits);
   fFMDV0C_post->Fill(nFMD_bwd_hits, nV0C_hits);
 }
 fHist_Stat->Fill(7);
 fHistzvertex->Fill(tPrimaryVtxPosition[2]);
 fHistCentrality_aftercut->Fill(lCentrality);
 //finish event selection, everything should be filled after this line
 if(fAnaMode=="TPCTPC"){
   if(fasso=="hadron") selectedTracksAssociated = GetAcceptedTracksLeading(fEvent,kFALSE);
   else if (fasso == "PID")    selectedTracksAssociated = GetAcceptedTracksPID(fEvent);
 }
 
 if(fAnaMode=="TPCFMD" || fAnaMode=="TPCTPC" || fAnaMode=="TPCFMDC"){
   selectedTracksLeading = GetAcceptedTracksLeading(fEvent,kTRUE);
 }
 /*
 // AliAODTracklets *tracklets = ((AliAODEvent*)fEvent)->GetTracklets();
 AliVMultiplicity *tracklets = ((AliAODEvent*)fEvent)->GetTracklets();
 if (!tracklets) return;
 Int_t nTracklets = tracklets->GetNumberOfTracklets();
 
 for (Int_t i = 0; i < nTracklets; i++) {
   Double_t dphi  = tracklets->GetDeltaPhi(i);

   //   if (TMath::Abs(dphi) * 1000 > 5) {
   //     continue;
   //   }

   Double_t theta = tracklets->GetTheta(i);
   Double_t etaits   = -TMath::Log(TMath::Tan(theta/2));
   // Drop everything outside of -1.7 < eta 1.7 to avoid overlas with the FMD

   //if (etaits < -1.7 || etaits > 1.7) {
   //	 continue;
   //   }
   
   Double_t phiits   = tracklets->GetPhi(i);
   if (phiits<0) phiits+=TMath::TwoPi();
   if (phiits>TMath::TwoPi()) phiits-=TMath::TwoPi();
   
   Double_t phiitscorr = phiits+ tracklets->GetDeltaPhi(i)*39./34.;//correction dphi*39./34. (Dphi in rad)	
   if (phiitscorr<0) phiitscorr+=TMath::TwoPi();
   if (phiitscorr>TMath::TwoPi()) phiitscorr-=TMath::TwoPi();
   
   //	   fh2_ITS_acceptance->Fill(etaits,tPrimaryVtxPosition[2]);
   
   Int_t label1 = tracklets->GetLabel(i,0);
   Int_t label2 = tracklets->GetLabel(i,1);
	//	if(label1!=label2)cout<<i<<" "<<label1<<" "<<label2<<endl;
   
   AliVParticle* particle = fMCEvent->GetTrack(label1);
   if (!particle) 	  continue;
   
   Short_t charge  = particle->Charge();
   Float_t ptMC    = particle->Pt();
   Float_t etaMC   = particle->Eta();
   Float_t phiMC   = particle->Phi();
   Float_t pdg     = particle->PdgCode();
   Bool_t primary  = particle->InheritsFrom("AliAODMCParticle") ? ((AliAODMCParticle*) particle)->IsPhysicalPrimary() : fMCEvent->IsPhysicalPrimary(label1);
   Bool_t itsdphicut=TMath::Abs(dphi) * 1000 <5.;

   Double_t itsqa[4]={etaits,phiitscorr,lCentrality,fPrimaryZVtx};
   if(itsdphicut) fhistits->Fill(itsqa);
   
   fhistdphirec[0]->Fill(ptMC,1./TMath::Abs(dphi*1000));
   fhistitsdeltaphi[0]->Fill(abs(dphi*1000));
   fhistitsrefdeltaphi[0]->Fill(ptMC,1000.*(phiits-phiMC));
   fhistitsrefdeltaphicorr[0]->Fill(ptMC,1000.*(phiitscorr-phiMC));
   fhistitsdeltarefphideltaphimc[0]->Fill(1000*dphi,1000.*(phiits-phiMC));
   fhistitsdeltarefphideltaphimc[1]->Fill(1000*dphi,1000.*(phiitscorr-phiMC));
   if(itsdphicut) 	{
	 fhistitsrefdeltaphiaftercut[0]->Fill(ptMC,1000.*(phiits-phiMC));
	 fhistitsrefdeltaphicorraftercut[0]->Fill(ptMC,1000.*(phiitscorr-phiMC));
	 fhistitsrefdeltaetaaftercut[0]->Fill(ptMC,etaits-etaMC);
	 fhistdphirec[1]->Fill(ptMC,1./TMath::Abs(dphi*1000));
   }
	
   if(label1!=label2) {
	 fhistitsdeltaphi[1]->Fill(abs(dphi*1000));
	 fhistitsrefdeltaphi[1]->Fill(ptMC,1000.*(phiits-phiMC));
	 fhistitsrefdeltaphicorr[2]->Fill(ptMC,1000.*(phiitscorr-phiMC));
	 if(itsdphicut) {
	   fhistitsrefdeltaphiaftercut[1]->Fill(ptMC,1000.*(phiits-phiMC));
	   fhistitsrefdeltaphicorraftercut[1]->Fill(ptMC,1000.*(phiitscorr-phiMC));
	   fhistitsrefdeltaetaaftercut[1]->Fill(ptMC,etaits-etaMC);
	  }
   }
   
   if(label1==label2){
	 if(primary){
	   fhistitsdeltaphi[2]->Fill(abs(dphi*1000));
	   fhistitsrefdeltaphi[2]->Fill(ptMC,1000.*(phiits-phiMC));
	   fhistitsrefdeltaphicorr[2]->Fill(ptMC,1000.*(phiitscorr-phiMC));
	   if(itsdphicut) {
		 fhistitsrefdeltaphiaftercut[2]->Fill(ptMC,1000.*(phiits-phiMC));
		 fhistitsrefdeltaphicorraftercut[2]->Fill(ptMC,1000.*(phiitscorr-phiMC));
		 fhistitsrefdeltaetaaftercut[2]->Fill(ptMC,etaits-etaMC);
	   }
	 }
	 if(!primary){
	   fhistitsrefdeltaphi[3]->Fill(ptMC,1000.*(phiits-phiMC));
	   fhistitsdeltaphi[3]->Fill(abs(dphi*1000));
	   fhistitsrefdeltaphicorr[3]->Fill(ptMC,1000.*(phiitscorr-phiMC));
	   if(itsdphicut) 	{
		 fhistitsrefdeltaphiaftercut[3]->Fill(ptMC,1000.*(phiits-phiMC));
		 fhistitsrefdeltaphicorraftercut[3]->Fill(ptMC,1000.*(phiitscorr-phiMC));
		 fhistitsrefdeltaetaaftercut[3]->Fill(ptMC,etaits-etaMC);
	   }
	 }
   }
 }
 */
 
 //MC
  Int_t pdgcode=0;
  Double_t conmcprim[5];
  Bool_t TrIsPrim=kFALSE;
  Bool_t TrIsSecondMate=kFALSE;
  Bool_t TrIsSecondWeak=kFALSE;
  Bool_t TrIsOthers=kFALSE;
  Double_t mcTrackEta=-999.;
  Double_t mcTrackPt=-999.;
  Double_t mcTrackPhi=-999.;
  Bool_t TrCharge=kFALSE;
  
  if(!fDataType){
    if(fIsAOD){
      AliAODMCHeader* aodMCheader=(AliAODMCHeader*)fEvent->FindListObject(AliAODMCHeader::StdBranchName());
      TClonesArray *mcArray = (TClonesArray*)fEvent->FindListObject(AliAODMCParticle::StdBranchName());
      if(!mcArray){
        Printf("No MC particle branch found");
        return;
      }
	  
      Int_t nMCAllTracks = mcArray->GetEntriesFast();
	  Int_t hoge1=0;
	  for (Int_t i = 0; i < nMCAllTracks; i++){
		AliAODMCParticle *mcTrack = (AliAODMCParticle*)mcArray->At(i);
		if (!mcTrack) {
		  Error("ReadEventAODMC", "Could not receive particle %d", i);
		  continue;
		}
		
		TrIsPrim=mcTrack->IsPhysicalPrimary();
		TrIsSecondMate=mcTrack->IsSecondaryFromMaterial();
		TrIsSecondWeak=mcTrack->IsSecondaryFromWeakDecay();
		TrIsOthers=!TrIsPrim && !TrIsSecondMate && !TrIsSecondWeak;
		
		/*
		  while(!mcTrack->IsPhysicalPrimary()){
		  if(mcTrack->GetMother()<0) break;
		  mcTrack=(AliAODMCParticle*)mcArray->At(((AliAODMCParticle * )mcTrack)->GetMother());
		  if(!mcTrack) break;
		  }
		*/
		mcTrackEta = mcTrack->Eta();
		mcTrackPt  = mcTrack->Pt();
		mcTrackPhi = mcTrack->Phi();
		TrCharge=mcTrack->Charge()!=0;
		pdgcode=TMath::Abs(mcTrack->PdgCode());
		if(!TrCharge)        continue;
		
		/*
		  Int_t daughterlabel1=mcTrack->GetFirstDaughter();
		  if(daughterlabel1>0){
		  AliAODMCParticle*d0=(AliAODMCParticle*)mcArray->At(daughterlabel1);
		  }
		  Int_t motherlabel1 = mcTrack->GetMother();
		  if(motherlabel1>0){
			 AliAODMCParticle*m0=(AliAODMCParticle*)mcArray->At(motherlabel1);
			 }
		*/
		conmcprim[0]=mcTrackPt;
		conmcprim[1]=mcTrackEta;
		conmcprim[2]=mcTrackPhi;
		conmcprim[3]=lCentrality;
		conmcprim[4]=fPrimaryZVtx;
		
		if(TrIsPrim){
		  fhistmcprim->Fill(conmcprim,0);//primay charged partilce distribution(no mother particle)
		  fhmcprimvzeta->Fill(mcTrackEta,mcTrack->Zv());
		  fhmcprimpdgcode->Fill(pdgcode);
		  fh2_FMD_eta_phi_prim->Fill(mcTrackEta,mcTrackPhi);
		  //		  fhistmcprimfinal->Fill(conmcprim,0);
		  Double_t mcTrackEta1=mcTrackEta;
		  if(fAnaMode=="TPCTPC") {
			if(mcTrackEta<-0.8 || mcTrackEta>0.8) continue;
			selectedTracksMC1->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1));         
			selectedTracksMC2->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1));  			
		  }else{
			if(fAnaMode=="TPCFMD"||fAnaMode=="TPCFMDC"){
			  if(mcTrackEta>-0.8 && mcTrackEta<0.8) selectedTracksMC1->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1));
			}else if(fAnaMode=="FMDFMD")	  {
			  if(mcTrackEta>1.7 && mcTrackEta<4.9) selectedTracksMC1->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1)); 			}	
 			if(fAnaMode=="TPCFMD"){
			  if(mcTrackEta>1.7  && mcTrackEta<4.9)	    selectedTracksMC2->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1));  
			}	else if(fAnaMode=="TPCFMDC" ||fAnaMode=="FMDFMD"){
			  if(mcTrackEta>-3.4  && mcTrackEta<-1.7)	    selectedTracksMC2->Add(new AliAssociatedTrackYS(mcTrack->Charge(),mcTrackEta,mcTrack->Phi(),mcTrack->Pt(),mcTrack->GetLabel(),-999,-999,0, 1));  
			}
		  }			
		}	  
	  }
	}else{
      /*
      /////
      int np=mcEvent->GetNumberOfTracks();
      int nprim=mcEvent->GetNumberOfPrimaries();
      int j=0;
      int error=0;
      for(int i=0;i<np;i++){
	if(mctruth->IsParticleSelected(i)){
	  
	  if(mctruth->GetNewLabel(i)!=j){
          error++;
	  }
	  
      j++;
    }
  }
//  cout<<"# of IsSelected Particles="<<j<<" : number of label error="<<error<<" nprim="<<nprim<<" np="<<np<<endl;

      Int_t hoge2=0;
      for (Int_t ip = 0; ip < np; ip++){
	AliMCParticle* mcpart = (AliMCParticle*) mcEvent->GetTrack(ip);
	TParticle* part = mcpart->Particle();
	Float_t xv = part->Vx();
	Float_t yv = part->Vy();
	Float_t zv = part->Vz();
	Float_t rv = TMath::Sqrt(xv * xv + yv * yv);

	Bool_t write = kFALSE;
	if (ip < nprim) {
	  // Select the primary event
	  
	  write = kTRUE;
	}else if (part->GetUniqueID() == kPDecay) {
	  // Particles from decay
	  // Check that the decay chain ends at a primary particle
	  AliMCParticle* mother = mcpart;
	  Int_t imo = mcpart->GetMother();
	  while((imo >= nprim) && (mother->Particle()->GetUniqueID() == kPDecay)) {
	    mother =  (AliMCParticle*) mcEvent->GetTrack(imo);
	    imo =  mother->GetMother();
	  }
	  // Select according to pseudorapidity and production point of primary ancestor
	  if (imo < nprim){write = kTRUE;
	    
	  }
	  // if(!Select(((AliMCParticle*) mcE->GetTrack(imo))->Particle(), rv, zv))write = kFALSE; // selection on eta and \
	  phi of the mother
	} else if (part->GetUniqueID() == kPPair) {
	  // Now look for pair production
	  Int_t imo = mcpart->GetMother();
	  if (imo < nprim) {
	    // Select, if the gamma is a primary
	    write = kTRUE;
	    
	  } else {
	    // Check if the gamma comes from the decay chain of a primary particle
	    AliMCParticle* mother =  (AliMCParticle*) mcEvent->GetTrack(imo);
	    imo = mother->GetMother();
	    while((imo >= nprim) && (mother->Particle()->GetUniqueID() == kPDecay)) {
	      mother =   (AliMCParticle*) mcEvent->GetTrack(imo);
	      imo =  mother->GetMother();
	    }
	    // Select according to pseudorapidity and production point
        //if (imo < nprim && Select(mother->Particle(), rv, zv))
	    if (imo < nprim){
            hoge2++;
          Float_t eta=((TParticle*)mother->Particle())->Eta();
          Bool_t select=kFALSE;
          if (TMath::Abs(eta) < 2.5 && rv < 170) select=kTRUE;
          if(eta > -4.2 && eta < -2.3 && zv > -500.) select=kTRUE;

          if(select)write = kTRUE;
        }
      }
    }
    if (write) {
     //if(mcH)mcH->SelectParticle(ip);
    // hoge2++;
   }
 }
      */
 //cout<<fNEntries<<" "<<"number of setparticles=="<<hoge2<<endl;
      //  AliStack*rstack=mcEvent->Stack();

      //cout<<fNEntries<<" "<<mcEvent->GetNumberOfTracks()<<" "<<rstack->GetNtrack()<<endl;
  //  cout<<fNEntries<<" "<<mcEvent->GetNumberOfTracks()<<" "<<mcEvent->GetNumberOfPrimaries()<<" "<<rstack->GetNtrack()<<" "<<((TTree*)(rstack->TreeK()))->GetEntries()<<endl;
      /*
    Double_t  nTracks_vzero=0;
    Int_t nTrCount=0;
    Int_t nTrRefs=0;
    Int_t nV0A = 0;
    Int_t nV0C = 0;
    Double_t conmcprim[4];
    Int_t hoge=0;
    for(Int_t iTracks=0;iTracks<mcEvent->GetNumberOfTracks();iTracks++){
        AliMCParticle* track = (AliMCParticle*)mcEvent->GetTrack(iTracks);
        TParticle *rParticle=mcEvent->Particle(iTracks);
      if (!track) {
        Error("ReadEventMC", "Could not receive particle %d", iTracks);
        continue;
      }
      pdgcode = TMath::Abs(track->PdgCode());
      TrIsPrim=mcEvent->IsPhysicalPrimary(iTracks);
      TrIsSecondMate=mcEvent->IsSecondaryFromMaterial(iTracks);
      TrIsSecondWeak=mcEvent->IsSecondaryFromWeakDecay(iTracks);
      TrIsOthers=!TrIsPrim && !TrIsSecondMate && !TrIsSecondWeak;

      //if(mctruth->IsParticleSelected(iTracks)) hoge++;
      //cout<<fNEntries<<" "<<iTracks<<" "<<pdgcode<<" "<<<<" "<<TrIsPrim<<" "<<TrIsSecondMate<<" "<<TrIsSecondWeak<<endl;
    //  if(TrIsPrim){
    //  cout<<fNEntries<<" "<<iTracks<<" "<<track->PdgCode()<<endl;
      //rstack->DumpPart(iTracks);
    //  }


      mcTrackEta = track->Eta();
      mcTrackPt  = track->Pt();
      mcTrackPhi = track->Phi();
      TrCharge=track->Charge()!=0;
      pdgcode = TMath::Abs(track->PdgCode());
      if(!TrCharge)        continue;
      Int_t daughterindex1=track->GetFirstDaughter();
      if(daughterindex1>0){
        AliMCParticle* dparticle=(AliMCParticle*)mcEvent->GetTrack(daughterindex1);
      }
      Int_t motherIndex1 = track->GetMother();
      if(motherIndex1>0){
          TParticle *mParticle1=mcEvent->Particle(motherIndex1);
      }
      conmcprim[0]=mcTrackPt;
      conmcprim[1]=mcTrackEta;
      conmcprim[2]=mcTrackPhi;
      conmcprim[3]=lCentrality;
      conmcprim[4]=fPrimaryZVtx;
      if(TrIsPrim && motherIndex1<0) {
         fhistmcprim->Fill(conmcprim,0);//primay charged partilce distribution(no mother particle)
         fhmcprimvzeta->Fill(mcTrackEta,track->Zv());
         fhmcprimpdgcode->Fill(pdgcode);
         fh2_FMD_eta_phi_prim->Fill(mcTrackEta,mcTrackPhi);
       }

       //rstack-> DumpLoadedStack();//Dump particle(Too heavy, don't forget comment out before job submit)
       //rstack->DumpPart(iTracks);

     }
*/
  //   cout<<fNEntries<<" "<<"number of physicsprimary==="<<hoge<<endl;
      // cout<<fNEntries<<" "<<mcEvent->GetNumberOfTracks()<<" "<<hoge<<" "<<mcEvent->GetNumberOfTracks()-hoge<<endl;
/*
      Int_t nref=0;
      Int_t nfmdhits=0;
      for (Int_t iref = 0; iref < track->GetNumberOfTrackReferences(); iref++) {
         AliTrackReference *ref = track->GetTrackReference(iref);
         if (ref->DetectorId() != 12) continue;// Select FMD
         nfmdhits++;
         Float_t r = TMath::Sqrt(ref->X()*ref->X()+ref->Y()*ref->Y());
         Float_t z=ref->Z();
      //   Float_t zv=TMath::Abs(z-rParticle->Vz());
         Float_t zv=TMath::Abs(z-tPrimaryVtxPosition[2]);
         if(z<0) zv=-1.*zv;
         Double_t theta=TMath::ATan2(r,zv);
         Double_t etav= -1.*TMath::Log(TMath::Tan(theta/2.));
         Double_t phiv = TMath::ATan2(ref->Y(),ref->X());
         if(phiv<0) phiv=phiv+2*TMath::Pi();
         if(TrIsPrim) {
           fh2_FMD_acceptance_prim->Fill(etav,rParticle->Vz());
           //fh2_FMD_eta_phi_prim->Fill(etav,phiv);
         }
         if(TrIsPrim && motherIndex1<0) {
           fhrefetaFMD[0]->Fill(etav);
           fhrefphiFMD[0]->Fill(phiv);
         } else if(TrIsSecondMate) {
           fhrefetaFMD[1]->Fill(etav);
           fhrefphiFMD[1]->Fill(phiv);
         } else if(TrIsSecondWeak) {
           fhrefetaFMD[2]->Fill(etav);
           fhrefphiFMD[2]->Fill(phiv);
         }else if(TrIsOthers) {
           fhrefetaFMD[3]->Fill(etav);
           fhrefphiFMD[3]->Fill(phiv);
         }
       }
     }
*/
   }

  }
  //primary track used for tpc tracks
  /*
  
  FillCorrelationTracks(lCentrality,selectedTracksMC1,selectedTracksMC2,fHistTriggerTrack1,fHistReconstTrack1,kFALSE,0.02,0.8,bSign,0);
  FillCorrelationTracksMixingprim(lCentrality,lPrimaryBestVtx->GetZ(),poolmax,poolmin,selectedTracksMC1,selectedTracksMC2,fHistTriggerTrackMix,fHistReconstTrackMix1,kFALSE,0.02,0.8,bSign,0);
  
  FillCorrelationTracks(lCentrality,selectedTracksMC1,selectedTracksAssociated,fHistTriggerTrack,fHistReconstTrack,kFALSE,0.02,0.8,bSign,0);
  FillCorrelationTracksMixing(lCentrality,lPrimaryBestVtx->GetZ(),poolmax,poolmin,selectedTracksMC1,selectedTracksAssociated,fHistTriggerTrackMix,fHistReconstTrackMix,kFALSE,0.02,0.8,bSign,0);
  */
  //reconstructed tracks are used
  FillCorrelationTracks(lCentrality,selectedTracksLeading,selectedTracksMC2,fHistTriggerTrack1,fHistReconstTrack1,kFALSE,0.02,0.8,bSign,0);
  FillCorrelationTracksMixingprim(lCentrality,lPrimaryBestVtx->GetZ(),poolmax,poolmin,selectedTracksLeading,selectedTracksMC2,fHistTriggerTrackMix,fHistReconstTrackMix1,kFALSE,0.02,0.8,bSign,0);
  
  FillCorrelationTracks(lCentrality,selectedTracksLeading,selectedTracksAssociated,fHistTriggerTrack,fHistReconstTrack,kFALSE,0.02,0.8,bSign,0);
  FillCorrelationTracksMixing(lCentrality,lPrimaryBestVtx->GetZ(),poolmax,poolmin,selectedTracksLeading,selectedTracksAssociated,fHistTriggerTrackMix,fHistReconstTrackMix,kFALSE,0.02,0.8,bSign,0);
    
  selectedTracksLeading->Clear();
  delete selectedTracksLeading;
  selectedTracksAssociated->Clear();
  delete selectedTracksAssociated;
  selectedTracksMC1->Clear();
  delete selectedTracksMC1;
  selectedTracksMC2->Clear();
  delete selectedTracksMC2;
  
}

TObjArray* AliAnalysisTaskSEpPbCorrelationsForward::GetFMDhitsYS(Bool_t Aside){
  TObjArray *tracks1 = new TObjArray;
    tracks1->SetOwner(kTRUE);

    AliAODForwardMult* aodForward =static_cast<AliAODForwardMult*>(fEvent->FindListObject("ForwardMC"));
	//cout<<aodForward->IsSecondaryCorrected()<<endl;
	//cout<<aodForward->GetCentrality()<<" "<<lCentrality<<endl;
	//cout<<aodForward->IsAcceptanceCorrected()<<endl;
	//cout<<aodForward->IsVertexBiasCorrected() <<endl;
	//cout<<aodForward->IsMergingEfficiencyCorrected()<<endl;

    //if(!aodForward) cout<<"no aodforadmult"<<endl;
	// Shape of d2Ndetadphi: 200, -4, 6, 20, 0, 2pi
      const TH2D& d2Ndetadphi = aodForward->GetHistogram();

      Int_t nEta = d2Ndetadphi.GetXaxis()->GetNbins();
      Int_t nPhi = d2Ndetadphi.GetYaxis()->GetNbins();
      //AliAnalysisTaskValidation::Tracks ret_vector;
      // FMD has no pt resolution!
      Double_t pt = 0;

      for (Int_t iEta = 1; iEta <= nEta; iEta++) {
        Int_t valid = Int_t(d2Ndetadphi.GetBinContent(iEta, 0));
        if (!valid) {
           // No data expected for this eta
          continue;
        }
        Float_t eta = d2Ndetadphi.GetXaxis()->GetBinCenter(iEta);
        for (Int_t iPhi = 1; iPhi <= nPhi; iPhi++) {
          // Bin content is most likely number of particles!
          Float_t mostProbableN = d2Ndetadphi.GetBinContent(iEta, iPhi);
          if (mostProbableN > 0) {
            Float_t phi = d2Ndetadphi.GetYaxis()->GetBinCenter(iPhi);
            //ret_vector.push_back(AliAnalysisTaskValidation::Track(eta, phi, pt, mostProbableN));
          //  fh2_FMD_acceptance->Fill(eta,tPrimaryVtxPosition[2]);
        ///    fh2_FMD_eta_phi->Fill(eta,phi,mostProbableN);

            if(Aside){
              if(eta<0) continue;
             } else{
              if(eta>0) continue;
            }

			//            tracks1->Add(new AliAssociatedVZEROYS(mostProbableN,eta,phi,0,0,0));
            tracks1->Add(new AliAssociatedTrackYS(0,eta,phi,0,0,0,0,0,mostProbableN));
            Double_t cont[4]={eta,phi,lCentrality,tPrimaryVtxPosition[2]};
            //            cout<<eta<<" "<<phi<<" "<<lCentrality<<" "<<tPrimaryVtxPosition[2]<<endl;

			//			if( abs(tPrimaryVtxPosition[2])<1) {
            fhistfmd->Fill(cont,0,mostProbableN);
            fh2_FMD_acceptance->Fill(eta,tPrimaryVtxPosition[2],mostProbableN);
            fh2_FMD_eta_phi->Fill(eta,phi,mostProbableN);
			//}

          }
        }
      }



    /*
      TObjArray *tracks1 = new TObjArray;
      tracks1->SetOwner(kTRUE);
      Int_t i=0;
    for (auto const &track: ret_vector) {
           //cout<<i<<" "<<track.eta<<" "<<track.phi<<" "<<track.weight<<endl;
           if(Aside==kTRUE){
            if(track.eta>0) tracks1->Add(new AliAssociatedVZEROYS(track.eta,track.phi,track.weight,0,0,0));
            } else{
            if(track.eta<0) tracks1->Add(new AliAssociatedVZEROYS(track.eta,track.phi,track.weight,0,0,0));
          }
           i++;
    }
    */
        return tracks1;
    //  std::random_device rd;
    //    std::default_random_engine engine{rd()};
    //    std::shuffle(std::begin(ret_vector), std::end(ret_vector), engine);

    //    return ret_vector;
  }

TObjArray *AliAnalysisTaskSEpPbCorrelationsForward::GetAcceptedTracksLeading(AliAODEvent *event,Bool_t leading) {
  TObjArray *tracks = new TObjArray;
  tracks->SetOwner(kTRUE);
  Int_t nTracks = event->GetNumberOfTracks();
  Double_t pidqa[5];
  Double_t  conmcprim[5];
  TClonesArray *mcArray = (TClonesArray*)fEvent->FindListObject(AliAODMCParticle::StdBranchName());
  for (Int_t i = 0; i < nTracks; i++) {
    AliAODTrack *fTrack = dynamic_cast<AliAODTrack *>(event->GetTrack(i));
    //AliVTrack *fTrack = dynamic_cast<AliVTrack *>(event->GetTrack(i));
    if (!fTrack)      continue;
  //  if (!IsAcceptedTrackLeading(fTrack))      continue;
    if (!IsAcceptedTrack(fTrack))      continue;
    //if(!fIsAOD)if(!fTrackCuts->AcceptTrack((AliESDTrack)*fTrack)) continue;
    if (fTrack->Charge() == 0)      continue;
    if(leading){
     pidqa[0]=fTrack->Pt();
     pidqa[1]=fTrack->Eta();
     //     pidqa[2]=RangePhi(fTrack->Phi());
     pidqa[2]=fTrack->Phi();
     pidqa[3]=lCentrality;
     pidqa[4]=fPrimaryZVtx;
     fHistLeadQA->Fill(pidqa,0);
	 /*
	 if(!fDataType) {
       Int_t a=0;
       Int_t label=abs(fTrack->GetLabel());
       AliAODMCParticle*mcTrack=(AliAODMCParticle *)mcArray->At(label);
       if(!mcTrack){
         continue;
       }
       while(!mcTrack->IsPhysicalPrimary()){
         if(mcTrack->GetMother()<0) break;
         mcTrack=(AliAODMCParticle*)mcArray->At(((AliAODMCParticle * )mcTrack)->GetMother());
         if(!mcTrack) break;
       }
       Double_t mcTrackEta = mcTrack->Eta();
       Double_t mcTrackPt  = mcTrack->Pt();
       Double_t mcTrackPhi = mcTrack->Phi();
       Int_t  pdgcode=TMath::Abs(mcTrack->PdgCode());      
      

       conmcprim[0]=mcTrackPt;
       conmcprim[1]=mcTrackEta;
       conmcprim[2]=mcTrackPhi;
       conmcprim[3]=lCentrality;
       conmcprim[4]=fPrimaryZVtx;
       
     }*/
    }
    
    Int_t SpAsso=0;
    tracks->Add(new AliAssociatedTrackYS(fTrack->Charge(), fTrack->Eta(), fTrack->Phi(), fTrack->Pt(), fTrack->GetID(), -999, -999, SpAsso, 1));
  }
  return tracks;
}

TObjArray *AliAnalysisTaskSEpPbCorrelationsForward::GetAcceptedTracksPID(AliAODEvent *fAOD) {
  TObjArray *tracks = new TObjArray;
  tracks->SetOwner(kTRUE);
  Int_t nTracks = fAOD->GetNumberOfTracks();
  Double_t pidqa[4];
  for (Int_t i = 0; i < nTracks; i++) {
    AliAODTrack *aodTrack = dynamic_cast<AliAODTrack *>(fAOD->GetTrack(i));
    Int_t SpPID=-999;
    if (!aodTrack)  continue;
    if (!IsAcceptedTrack(aodTrack))    continue;
    if (aodTrack->Charge() == 0)      continue;
    Double_t nSigmaKaonTPC = fPIDResponse->NumberOfSigmasTPC(aodTrack, AliPID::kKaon);
    Double_t nSigmaPionTPC = fPIDResponse->NumberOfSigmasTPC(aodTrack, AliPID::kPion);
    Double_t nSigmaProtonTPC = fPIDResponse->NumberOfSigmasTPC(aodTrack, AliPID::kProton);
    Double_t nSigmaKaonTOF = fPIDResponse->NumberOfSigmasTOF(aodTrack, AliPID::kKaon);
    Double_t nSigmaPionTOF = fPIDResponse->NumberOfSigmasTOF(aodTrack, AliPID::kPion);
    Double_t nSigmaProtonTOF = fPIDResponse->NumberOfSigmasTOF(aodTrack, AliPID::kProton);
    fHistNsig[0]->Fill(aodTrack->Pt(),nSigmaPionTPC);
    fHistNsig[1]->Fill(aodTrack->Pt(),nSigmaKaonTPC);
    fHistNsig[2]->Fill(aodTrack->Pt(),nSigmaProtonTPC);
    fHistNsig[3]->Fill(aodTrack->Pt(),nSigmaPionTOF);
    fHistNsig[4]->Fill(aodTrack->Pt(),nSigmaKaonTOF);
    fHistNsig[5]->Fill(aodTrack->Pt(),nSigmaProtonTOF);
    fHistNsigcorr[3]->Fill(nSigmaPionTPC,nSigmaPionTOF);
    fHistNsigcorr[4]->Fill(nSigmaKaonTPC,nSigmaKaonTOF);
    fHistNsigcorr[5]->Fill(nSigmaProtonTPC,nSigmaProtonTOF);

    Double_t d2nsigmakaon =  nSigmaKaonTPC * nSigmaKaonTPC + nSigmaKaonTOF * nSigmaKaonTOF;
    Double_t d2nsigmapion =  nSigmaPionTPC * nSigmaPionTPC + nSigmaPionTOF * nSigmaPionTOF;
    Double_t d2nsigmaproton =  nSigmaProtonTPC * nSigmaProtonTPC + nSigmaProtonTOF * nSigmaProtonTOF;

    Bool_t fPIDTOF = kTRUE;
    if (fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF, fAOD->GetTrack(i)) == 0)      fPIDTOF = kFALSE;
    else      fPIDTOF = kTRUE;

    Double_t nSigmaKaonTOFTPC;
    Double_t nSigmaPionTOFTPC;
    Double_t nSigmaProtonTOFTPC;

    if (fPIDTOF && aodTrack->Pt() > 0.5) {
      nSigmaKaonTOFTPC = TMath::Sqrt(d2nsigmakaon);
      nSigmaPionTOFTPC = TMath::Sqrt(d2nsigmapion);
      nSigmaProtonTOFTPC = TMath::Sqrt(d2nsigmaproton);
    } else {
      nSigmaKaonTOFTPC = TMath::Abs(nSigmaKaonTPC);
      nSigmaPionTOFTPC = TMath::Abs(nSigmaPionTPC);
      nSigmaProtonTOFTPC = TMath::Abs(nSigmaProtonTPC);
    }
    if ((nSigmaKaonTOFTPC < fMaxnSigmaTPCTOF) &&  (nSigmaKaonTOFTPC < nSigmaPionTOFTPC) &&   (nSigmaKaonTOFTPC < nSigmaProtonTOFTPC)) SpPID = 0;
    if ((nSigmaPionTOFTPC < fMaxnSigmaTPCTOF) &&  (nSigmaPionTOFTPC < nSigmaKaonTOFTPC) &&   (nSigmaPionTOFTPC < nSigmaProtonTOFTPC)) SpPID = 1;
    if ((nSigmaProtonTOFTPC < fMaxnSigmaTPCTOF) &&  (nSigmaProtonTOFTPC < nSigmaKaonTOFTPC) &&   (nSigmaProtonTOFTPC < nSigmaPionTOFTPC)) SpPID = 2;

    pidqa[0]=aodTrack->Pt();
    pidqa[1]=aodTrack->Eta();
    pidqa[2]=RangePhi(aodTrack->Phi());
    pidqa[3]=lCentrality;
    if(SpPID<0) continue;
    if(fQA) fHistPIDQA->Fill(pidqa, SpPID);
    if(SpPID==1) fHistNsigcorr[0]->Fill(nSigmaPionTPC,nSigmaPionTOF);
    if(SpPID==0) fHistNsigcorr[1]->Fill(nSigmaKaonTPC,nSigmaKaonTOF);
    if(SpPID==2) fHistNsigcorr[2]->Fill(nSigmaProtonTPC,nSigmaProtonTOF);

    tracks->Add(new AliAssociatedTrackYS(aodTrack->Charge(), aodTrack->Eta(), aodTrack->Phi(), aodTrack->Pt(), aodTrack->GetID(), -999, -999, SpPID, 1));
  }
  return tracks;
}


Bool_t AliAnalysisTaskSEpPbCorrelationsForward::IsAcceptedTrackLeading(const AliVTrack *fTrack) {
  if (!fTrack)  return kFALSE;
  if(fIsAOD){
    if (!((AliAODTrack*)fTrack)->TestFilterMask(BIT(5)))  return kFALSE;
  }
    /*
    if (!fTrack->IsOn(AliVTrack::kTPCrefit)) return kFALSE;
    Float_t nCrossedRowsTPC =fTrack->GetTPCClusterInfo(2,1);
    if (nCrossedRowsTPC < 70) return kFALSE;
    Int_t findable=fTrack->GetTPCNclsF();
    if (findable <= 0)return kFALSE;
    if (nCrossedRowsTPC/findable < 0.8) return kFALSE;
  */
  if (fTrack->Pt() < fPtMin) return kFALSE;
  if (fTrack->Pt() > 4.0) return kFALSE;
  
  if (TMath::Abs(fTrack->Eta()) > fEtaMax)  return kFALSE;
  return kTRUE;
}


Bool_t AliAnalysisTaskSEpPbCorrelationsForward::IsAcceptedTrack(const AliAODTrack *aodTrack) {
  if (!aodTrack)
    return kFALSE;
  //  if(!aodTrack->TestFilterMask(BIT(5))) return kFALSE; // standard cut with
  //  tight DCA cut
  if (!aodTrack->TestFilterMask(BIT(5)))
    return kFALSE; // only tpc cut
  /*
  if (!aodTrack->IsOn(AliAODTrack::kTPCrefit)) return kFALSE;
  Float_t nCrossedRowsTPC =aodTrack->GetTPCClusterInfo(2,1);
  if (nCrossedRowsTPC < 70) return kFALSE;
  Int_t findable=aodTrack->GetTPCNclsF();
  if (findable <= 0)return kFALSE;
  if (nCrossedRowsTPC/findable < 0.8) return kFALSE;
  */
  if (aodTrack->Pt() < fPtMin)
    return kFALSE;
  if (TMath::Abs(aodTrack->Eta()) > fEtaMax)
    return kFALSE;
  return kTRUE;
}

void AliAnalysisTaskSEpPbCorrelationsForward::FillCorrelationTracks( Double_t centrality, TObjArray *triggerArray, TObjArray *selectedTrackArray,AliTHn *triggerHist, AliTHn *associateHist, Bool_t twoTrackEfficiencyCut, Float_t twoTrackEfficiencyCutValue, Float_t fTwoTrackCutMinRadius,Float_t bSign, Int_t step) {
  if (!triggerHist || !associateHist)    return;
  if(fAnaMode=="TPCTPC"){
      Double_t binscontTrig[2];
      Double_t binscont[6];
      for (Int_t i = 0; i < triggerArray->GetEntriesFast(); i++) {
      AliAssociatedTrackYS *trigger = (AliAssociatedTrackYS *)triggerArray->At(i);
      if (!trigger)    continue;
      Float_t triggerPt = trigger->Pt();
      Float_t triggerEta = trigger->Eta();
      Float_t triggerPhi = trigger->Phi();
      Int_t trigFirstID = trigger->GetIDFirstDaughter();
      Int_t trigSecondID = trigger->GetIDSecondDaughter();
      Int_t trigID = trigger->GetID();
      binscontTrig[0] = triggerPt;
      binscontTrig[1] = centrality;
      triggerHist->Fill(binscontTrig, 0);
      for (Int_t j = 0; j < selectedTrackArray->GetEntriesFast(); j++) {
        AliAssociatedTrackYS *associate =   (AliAssociatedTrackYS*)selectedTrackArray->At(j);
        if (!associate)        continue;
        Int_t AssoFirstID = associate->GetIDFirstDaughter();
        Int_t AssoSecondID = associate->GetIDSecondDaughter();
        if (fasso == "hadron" || fasso=="PID") {
          if (triggerPt <= associate->Pt())          continue;
          if (trigID == associate->GetID())          continue;
        }
        binscont[0] = triggerEta - associate->Eta();
        binscont[1] = associate->Pt();
        binscont[2] = triggerPt;
        binscont[3] = centrality;
        binscont[4] = RangePhi(triggerPhi - associate->Phi());
        binscont[5] = fPrimaryZVtx;
		Int_t SpAsso = associate->WhichCandidate();
        if (fasso == "V0" || fasso == "Phi" || fasso == "Cascade" ||  (fasso == "PID")) {
          if (SpAsso < 0)          continue;
          associateHist->Fill(binscont, SpAsso);
        }else if(fasso=="hadron"){
          associateHist->Fill(binscont, 0);
        }
      }
    }
  }else if (fAnaMode=="TPCV0A" || fAnaMode=="TPCV0C" || fAnaMode=="TPCFMD" || fAnaMode=="TPCFMDC"){
    Double_t binscontTrig[4];
    Double_t binscont[7];
    for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
      AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
      if(!trigger)continue;
      Float_t  triggerEta  = trigger->Eta();
      Float_t  triggerPhi  = trigger->Phi();
      Float_t  triggerPt  = trigger->Pt();
	  binscontTrig[0]=triggerPt;
      binscontTrig[1]=centrality;
      binscontTrig[2]=fPrimaryZVtx;
      binscontTrig[3]=triggerEta;
      Int_t SpAsso= trigger->WhichCandidate();
      triggerHist->Fill(binscontTrig,0);
      for (Int_t j=0; j<selectedTrackArray->GetEntriesFast(); j++){
        AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) selectedTrackArray->At(j);
        if(!associate)continue;
        Float_t associatemultiplicity=associate->Multiplicity();
        Float_t assophi=associate->Phi();
        Float_t assoeta=associate->Eta();
        binscont[0]=triggerEta-associate->Eta();
        binscont[1]=triggerPt;
        binscont[2]=associate->Eta();
        binscont[3]=centrality;
        binscont[4]=RangePhi(triggerPhi-associate->Phi());
        binscont[5]=fPrimaryZVtx;
        binscont[6]=triggerEta;
		if (fasso == "V0" || fasso == "Phi" || fasso == "Cascade" ||  (fasso == "PID")) {
          if (SpAsso < 0)          continue;
          associateHist->Fill(binscont, SpAsso,(Double_t)associate->Multiplicity());
        }else if(fasso=="hadron"){
          associateHist->Fill(binscont, 0, (Double_t)associate->Multiplicity());
        }
      }
    }
  }else if (fAnaMode=="ITSFMD" || fAnaMode=="ITSFMDC"){
    Double_t binscontTrig[4];
    Double_t binscont[6];
    //    cout<<triggerArray->GetEntriesFast()<<" "<<selectedTrackArray->GetEntriesFast()<<" "<<triggerArray->GetEntriesFast()*selectedTrackArray->GetEntriesFast()<<endl;
    for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
      AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
      if(!trigger)continue;
      Float_t  triggerEta  = trigger->Eta();
      Float_t  triggerPhi  = trigger->Phi();
      Float_t  triggerPt  = 0.5;
	  binscontTrig[0]=triggerPt;
      binscontTrig[1]=centrality;
      binscontTrig[2]=fPrimaryZVtx;
      binscontTrig[3]=triggerEta;
      Int_t SpAsso= trigger->WhichCandidate();
      triggerHist->Fill(binscontTrig,SpAsso);
      for (Int_t j=0; j<selectedTrackArray->GetEntriesFast(); j++){
        AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) selectedTrackArray->At(j);
        if(!associate)continue;
        Float_t associatemultiplicity=associate->Multiplicity();
        Float_t assophi=associate->Phi();
        Float_t assoeta=associate->Eta();
        binscont[0]=triggerEta-associate->Eta();
	binscont[1]=centrality;	
	binscont[2]=associate->Eta();
	binscont[3]=RangePhi(triggerPhi-associate->Phi());
        binscont[4]=fPrimaryZVtx;
        binscont[5]=triggerEta;
	associateHist->Fill(binscont, 0, (Double_t)associate->Multiplicity());
      }
    }
 } else if(fAnaMode=="FMDFMD"){
    Double_t binscontTrig[3];
    Double_t binscont[6];
    for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
      AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
      if(!trigger)continue;
      Float_t  triggerMultiplicity= trigger->Multiplicity();
      //if(triggerMultiplicity<1) cout<<"triggerMultiplicity=="<<triggerMultiplicity<<endl;
      Float_t  triggerEta  = trigger->Eta();
      Float_t  triggerPhi  = trigger->Phi();
      binscontTrig[0]=centrality;
      binscontTrig[1]=triggerEta;
      binscontTrig[2]=fPrimaryZVtx;
      triggerHist->Fill(binscontTrig,0,(Double_t)triggerMultiplicity);
      for (Int_t j=0; j<selectedTrackArray->GetEntriesFast(); j++){
        AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) selectedTrackArray->At(j);
        if(!associate)continue;
        Float_t assophi=associate->Phi();
        Float_t assoeta=associate->Eta();
        Float_t mult=associate->Multiplicity()*triggerMultiplicity;
        binscont[0]=triggerEta-associate->Eta();
        binscont[1]=associate->Eta();
        binscont[2]=triggerEta;
        binscont[3]=centrality;
        binscont[4]=RangePhi_FMD(triggerPhi-associate->Phi());
        binscont[5]=fPrimaryZVtx;

        Float_t dphivzero=triggerPhi-associate->Phi();
        if(triggerPhi==assophi && triggerEta==assoeta) continue;
        associateHist->Fill(binscont,0,(Double_t)mult);
      }
    }
  }else if(fAnaMode=="SECA"||fAnaMode=="SECC"){
    Double_t binscontTrig[3];
    Double_t binscont[5];
    for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
      AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
      if(!trigger)continue;
      Float_t  triggerMultiplicity= trigger->Multiplicity();
      Float_t  triggerEta  = trigger->Eta();
      Float_t  triggerPhi  = trigger->Phi();
      binscontTrig[0]=centrality;
      binscontTrig[1]=triggerEta;
      binscontTrig[2]=fPrimaryZVtx;
      triggerHist->Fill(binscontTrig,0,(Double_t)triggerMultiplicity);
      for (Int_t j=0; j<selectedTrackArray->GetEntriesFast(); j++){
        AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) selectedTrackArray->At(j);
        if(!associate)continue;
        Float_t assophi=associate->Phi();
        Float_t assoeta=associate->Eta();
        Float_t mult=associate->Multiplicity()*triggerMultiplicity;
        binscont[0]=triggerEta-associate->Eta();
        binscont[1]=triggerEta;
        binscont[2]=centrality;
        binscont[3]=RangePhi_FMD(triggerPhi-associate->Phi());
        binscont[4]=fPrimaryZVtx;
        //        if(triggerEta>2.7 && triggerEta<2.8)cout<<triggerEta<<" "<<associate->Eta()<<" "<<binscont[0]<<endl;
        Float_t dphivzero=triggerPhi-associate->Phi();
        if(triggerPhi==assophi && triggerEta==assoeta) continue;
        associateHist->Fill(binscont,0,(Double_t)mult);
      }
    }
  }else if(fAnaMode=="V0AV0C"){
    Double_t  binscont1[4];
    Double_t  binscontTrig1[3];
    for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
      AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
      if(!trigger)continue;
      Double_t  triggerMultiplicity= trigger->Multiplicity();
      Double_t  triggerEta  = trigger->Eta();
      Double_t  triggerPhi  = trigger->Phi();
      binscontTrig1[0]=centrality;
      binscontTrig1[1]=triggerEta;
      binscontTrig1[2]=triggerPhi;
      triggerHist->Fill(binscontTrig1,0,(Double_t)triggerMultiplicity);
      for (Int_t j=0; j<selectedTrackArray->GetEntriesFast(); j++){
        AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) selectedTrackArray->At(j);
        if(!associate)continue;
        Double_t associatemultiplicity=associate->Multiplicity();
        Double_t assophi=associate->Phi();
        Double_t assoeta=associate->Eta();
        binscont1[0]=associate->Eta();
        binscont1[1]=triggerEta;
        binscont1[2]=centrality;
        binscont1[3]=RangePhi2(triggerPhi-associate->Phi());
        if(triggerPhi==assophi && triggerEta==assoeta) continue;
        Double_t dphivzero=triggerPhi-associate->Phi();
        associateHist->Fill(binscont1,0,(Double_t)triggerMultiplicity*associatemultiplicity);
      }
    }
  }
}

void AliAnalysisTaskSEpPbCorrelationsForward::FillCorrelationTracksMixing(Double_t centrality, Double_t pvxMix, Double_t poolmax, Double_t poolmin,    TObjArray *triggerArray, TObjArray *selectedTrackArray, AliTHn *triggerHist,    AliTHn *associateHist, Bool_t twoTrackEfficiencyCut,    Float_t twoTrackEfficiencyCutValue, Float_t fTwoTrackCutMinRadius,    Float_t bSign, Int_t step) {
  if (!triggerHist || !associateHist) return;
  Double_t counterMix = 0;
  AliEventPool *pool = fPoolMgr->GetEventPool(centrality, pvxMix);
  if (!pool)    AliFatal(Form("No pool found for centrality = %f, zVtx = %f", centrality,
  pvxMix));
  //    cout<<pool->NTracksInPool()<<" "<<fPoolMinNTracks<<" "<<pool->GetCurrentNEvents()<<" "<<fMinEventsToMix<<endl;
  if (pool->IsReady() || pool->NTracksInPool() > fPoolMinNTracks ||  pool->GetCurrentNEvents() > fMinEventsToMix) {
     Int_t nMix = pool->GetCurrentNEvents();
    for (Int_t jMix = 0; jMix < nMix; jMix++) {  
      TObjArray *mixEvents = pool->GetEvent(jMix);
      if(fAnaMode=="TPCTPC"){
        Double_t binscontTrig[2];
        Double_t binscont[6];
      
        for (Int_t i = 0; i < triggerArray->GetEntriesFast(); i++) {
          AliAssociatedTrackYS *trig = (AliAssociatedTrackYS *)triggerArray->At(i);
          if (!trig)          continue;
          Double_t triggerPhi = trig->Phi();
          Double_t triggerEta = trig->Eta();
          Double_t triggerPt = trig->Pt();
          counterMix++;
          binscontTrig[0] = triggerPt;
          binscontTrig[1] = centrality;
          triggerHist->Fill(binscontTrig, step);
          for (Int_t j = 0; j < mixEvents->GetEntriesFast(); j++) {
            AliAssociatedTrackYS *associate =  (AliAssociatedTrackYS *)mixEvents->At(j);
            if (!associate) continue;
            /*
            if(triggerEta<=0){
            if(associate->Eta()<=0) continue;
          }else if(triggerEta>0){
          if(associate->Eta()>0) continue;
        }
        */
        binscont[0] = triggerEta - associate->Eta();
        binscont[1] = associate->Pt();
        binscont[2] = triggerPt;
        binscont[3] = centrality;
        binscont[4] = RangePhi(triggerPhi - associate->Phi());
        binscont[5] = pvxMix;
        Int_t SpAsso = associate->WhichCandidate();
        if (fasso == "V0" || fasso == "Phi" || fasso == "Cascade" || (fasso == "PID")) {
          if (SpAsso < 0)   continue;
          associateHist->Fill(binscont, SpAsso, 1. / (Double_t)nMix);
        }else if(fasso=="hadron"){
          associateHist->Fill(binscont, 0,1./(Double_t)nMix);
        }
      }
    }
      }else if(fAnaMode=="TPCV0A" || fAnaMode=="TPCV0C" || fAnaMode=="TPCFMD" || fAnaMode=="TPCFMDC"){
        Double_t binscontTrig[2];
        Double_t binscont[7];
        for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
          AliAssociatedTrackYS* trigger =(AliAssociatedTrackYS*) triggerArray->At(i);
          if(!trigger)continue;
          Double_t triggerPt   = trigger->Pt();
          Double_t triggerEta  = trigger->Eta();
          Double_t triggerPhi  = trigger->Phi();
		  Int_t SpAsso=trigger->WhichCandidate();
          counterMix++;
          binscontTrig[0]=triggerPt;
          binscontTrig[1]=centrality;
          triggerHist->Fill(binscontTrig,SpAsso);
          for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
            AliAssociatedTrackYS* associate=(AliAssociatedTrackYS*)  mixEvents->At(j);
            if(!associate)continue;
            binscont[0]=triggerEta-associate->Eta();
            binscont[1]=triggerPt;
            binscont[2]=associate->Eta();
            binscont[3]=centrality;
            binscont[4]=RangePhi(triggerPhi-associate->Phi());
            binscont[5]=pvxMix;
            binscont[6]=triggerEta;
            if (fasso == "V0" || fasso == "Phi" || fasso == "Cascade" || (fasso == "PID")) {
              if (SpAsso < 0)   continue;
              associateHist->Fill(binscont, SpAsso, (Double_t)associate->Multiplicity()/(Double_t)nMix);
            }else if(fasso=="hadron"){
              associateHist->Fill(binscont, 0,(Double_t)associate->Multiplicity()/(Double_t)nMix);
            }
          }
        }
      }else if(fAnaMode=="ITSFMD" || fAnaMode=="ITSFMDC"){
        Double_t binscontTrig[2];
        Double_t binscont[6];
        for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
          AliAssociatedTrackYS* trigger =(AliAssociatedTrackYS*) triggerArray->At(i);
          if(!trigger)continue;
          Double_t triggerPt   = 0.5;
          Double_t triggerEta  = trigger->Eta();
          Double_t triggerPhi  = trigger->Phi();
		  Int_t SpAsso=trigger->WhichCandidate();
          counterMix++;
          binscontTrig[0]=triggerPt;
          binscontTrig[1]=centrality;
          triggerHist->Fill(binscontTrig,SpAsso);
          for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
            AliAssociatedTrackYS* associate=(AliAssociatedTrackYS*)  mixEvents->At(j);
			if(!associate)continue;
            binscont[0]=triggerEta-associate->Eta();
            binscont[1]=centrality;
            binscont[2]=associate->Eta();
            binscont[3]=RangePhi(triggerPhi-associate->Phi());
            binscont[4]=pvxMix;
            binscont[5]=triggerEta;
			//			if(-1.35>binscont[3])cout<<binscont[3]<<" "<<triggerPhi<<" "<<associate->Phi()<<endl;
			associateHist->Fill(binscont, 0,(Double_t)associate->Multiplicity()/(Double_t)nMix);
		  }
        }
      }else if(fAnaMode=="FMDFMD"){
        Double_t binscontTrig[2];
        Double_t binscont[6];
        for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
          AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
        if(!trigger)continue;
        Float_t triggerMultiplicity= trigger->Multiplicity();
        Float_t  triggerEta  = trigger->Eta();
        Float_t  triggerPhi  = trigger->Phi();
        counterMix++;
        binscontTrig[0]=centrality;
        binscontTrig[1]=triggerEta;
        triggerHist->Fill(binscontTrig,step,(Double_t)triggerMultiplicity);
        for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
          AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) mixEvents->At(j);
          if(!associate)continue;
          //          Double_t associatemultiplicity=associate->Multiplicity();
          Float_t assophi=associate->Phi();
          Float_t assoeta=associate->Eta();
          Float_t mult=triggerMultiplicity*associate->Multiplicity();
          binscont[0]=triggerEta-associate->Eta();
          binscont[1]=associate->Eta();
          binscont[2]=triggerEta;
          binscont[3]=centrality;
          binscont[4]=RangePhi_FMD(triggerPhi-associate->Phi());
          binscont[5]=pvxMix;
          // if(triggerPhi==assophi && triggerEta==assoeta) continue;
          //  associateHist->Fill(binscont,0,(Double_t)triggerMultiplicity*associatemultiplicity/(Double_t)nMix);
          associateHist->Fill(binscont,0,mult/(Double_t)nMix);
        }
        }
      }else if(fAnaMode=="SECA"||fAnaMode=="SECC"){
        Double_t binscontTrig[2];
        Double_t binscont[5];
        for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
          AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
        if(!trigger)continue;
        Float_t triggerMultiplicity= trigger->Multiplicity();
        Float_t  triggerEta  = trigger->Eta();
        Float_t  triggerPhi  = trigger->Phi();
        counterMix++;
        binscontTrig[0]=centrality;
        binscontTrig[1]=triggerEta;
        triggerHist->Fill(binscontTrig,step,(Double_t)triggerMultiplicity);
        for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
          AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) mixEvents->At(j);
          if(!associate)continue;
          //          Double_t associatemultiplicity=associate->Multiplicity();
          Float_t assophi=associate->Phi();
          Float_t assoeta=associate->Eta();
          Float_t mult=triggerMultiplicity*associate->Multiplicity();
          binscont[0]=triggerEta-associate->Eta();
          binscont[1]=triggerEta;
          binscont[2]=centrality;
          binscont[3]=RangePhi_FMD(triggerPhi-associate->Phi());
          binscont[4]=pvxMix;
          //     if(triggerPhi==assophi && triggerEta==assoeta) continue;
          //  associateHist->Fill(binscont,0,(Double_t)triggerMultiplicity*associatemultiplicity/(Double_t)nMix);
          associateHist->Fill(binscont,0,mult/(Double_t)nMix);
        }
        }
        

	  }else if (fAnaMode=="V0AV0C"){
      Double_t  binscont1[4];
      Double_t  binscontTrig1[3];
      for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
        AliAssociatedTrackYS* trigger = (AliAssociatedTrackYS*) triggerArray->At(i);
        if(!trigger)continue;
        Double_t  triggerMultiplicity= trigger->Multiplicity();
        Double_t  triggerEta  = trigger->Eta();
        Double_t  triggerPhi  = trigger->Phi();
        counterMix++;
        binscontTrig1[0]=centrality;
        binscontTrig1[1]=triggerEta;
        binscontTrig1[2]=triggerPhi;
        triggerHist->Fill(binscontTrig1,step,(Double_t)triggerMultiplicity);
        for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
          AliAssociatedTrackYS* associate = (AliAssociatedTrackYS*) mixEvents->At(j);
          if(!associate)continue;
          Double_t associatemultiplicity=associate->Multiplicity();
          Double_t assophi=associate->Phi();
          Double_t assoeta=associate->Eta();
          binscont1[0]=associate->Eta();
          binscont1[1]=triggerEta;
          binscont1[2]=centrality;
          binscont1[3]=RangePhi2(triggerPhi-associate->Phi());
          if(triggerPhi==assophi && triggerEta==assoeta) continue;
          associateHist->Fill(binscont1,0,(Double_t)triggerMultiplicity*associatemultiplicity/(Double_t)nMix);
        }
      }
    }
  }
}

  TObjArray *tracksClone = new TObjArray;
  tracksClone->SetOwner(kTRUE);
  //  cout<<selectedTrackArray->GetEntriesFast()<<endl;
  for (Int_t i = 0; i < selectedTrackArray->GetEntriesFast(); i++) {
    AliAssociatedTrackYS *particle =  (AliAssociatedTrackYS *)selectedTrackArray->At(i);
    tracksClone->Add(new AliAssociatedTrackYS(
      particle->Charge(), particle->Eta(), particle->Phi(), particle->Pt(),
      particle->GetID(), particle->GetIDFirstDaughter(),
      particle->GetIDSecondDaughter(), particle->WhichCandidate(),
      particle->Multiplicity()));
    }
   pool->UpdatePool(tracksClone);
}



void AliAnalysisTaskSEpPbCorrelationsForward::FillCorrelationTracksMixingprim(Double_t centrality, Double_t pvxMix, Double_t poolmax, Double_t poolmin,    TObjArray *triggerArray, TObjArray *selectedTrackArray, AliTHn *triggerHist,    AliTHn *associateHist, Bool_t twoTrackEfficiencyCut,    Float_t twoTrackEfficiencyCutValue, Float_t fTwoTrackCutMinRadius,    Float_t bSign, Int_t step) {
  if (!triggerHist || !associateHist) return;
  Double_t counterMix = 0;
  AliEventPool *pool1 = fPoolMgr1->GetEventPool(centrality, pvxMix);
  if (!pool1)  AliFatal(Form("No pool found for centrality = %f, zVtx = %f", centrality,
			    pvxMix));
  Double_t binscontTrig[3];
  Double_t binscont[7];
  if (pool1->IsReady() || pool1->NTracksInPool() > fPoolMinNTracks || pool1->GetCurrentNEvents() > fMinEventsToMix) {
    Int_t nMix = pool1->GetCurrentNEvents();
    for (Int_t jMix = 0; jMix < nMix; jMix++) {
      TObjArray *mixEvents = pool1->GetEvent(jMix);
      for(Int_t i=0;i<triggerArray->GetEntriesFast();i++){
	AliAssociatedTrackYS* trigger =(AliAssociatedTrackYS*) triggerArray->At(i);
	if(!trigger)continue;
	Double_t triggerPt   = trigger->Pt();
	Double_t triggerEta  = trigger->Eta();
	Double_t triggerPhi  = trigger->Phi();
	counterMix++;
	binscontTrig[0]=triggerPt;
	binscontTrig[1]=centrality;
	binscontTrig[2]=fPrimaryZVtx;
	triggerHist->Fill(binscontTrig,0);
	for (Int_t j=0; j<mixEvents->GetEntriesFast(); j++){
	  AliAssociatedTrackYS* associate=(AliAssociatedTrackYS*)  mixEvents->At(j);
	  if(!associate)continue;
	  binscont[0]=triggerEta-associate->Eta();
	  binscont[1]=triggerPt;
	  binscont[2]=associate->Eta();
	  binscont[3]=centrality;
	  binscont[4]=RangePhi(triggerPhi-associate->Phi());
	  binscont[5]=fPrimaryZVtx;
	  binscont[6]=triggerEta;
	  //	  cout<<" i am here!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	  Int_t candidate=associate->WhichCandidate();
          associateHist->Fill(binscont, 0,1./(Double_t)nMix);
	}
      }
    }
  }
  TObjArray *tracksClone = new TObjArray;
  tracksClone->SetOwner(kTRUE);
  for (Int_t i=0; i<selectedTrackArray->GetEntriesFast(); i++)
    {
      AliAssociatedTrackYS* particle = (AliAssociatedTrackYS*) selectedTrackArray->At(i);
      tracksClone->Add(new AliAssociatedTrackYS(
      particle->Charge(), particle->Eta(), particle->Phi(), particle->Pt(),
      particle->GetID(), particle->GetIDFirstDaughter(),
      particle->GetIDSecondDaughter(), particle->WhichCandidate(),
      particle->Multiplicity()));
    }
  pool1->UpdatePool(tracksClone);
  
}
Double_t AliAnalysisTaskSEpPbCorrelationsForward::RangePhi(Double_t DPhi) {
  if (DPhi < -TMath::Pi() / 2)   DPhi += 2 * TMath::Pi();
  if (DPhi > 3 * TMath::Pi() / 2) DPhi -= 2*TMath::Pi();
  return DPhi;
}

Double_t AliAnalysisTaskSEpPbCorrelationsForward::RangePhi_FMD(Double_t DPhi) {
  //if (DPhi < (-TMath::Pi() / 2 -0.0001))   DPhi += 2 * TMath::Pi();
  //if (DPhi > (3 * TMath::Pi() / 2-0.0001)) DPhi -= 2*TMath::Pi();
  DPhi = TMath::ATan2(TMath::Sin(DPhi), TMath::Cos(DPhi));
  if (DPhi < (-0.5*TMath::Pi()-0.0001))    DPhi += 2 * TMath::Pi();
  return DPhi;
}

Double_t AliAnalysisTaskSEpPbCorrelationsForward::RangePhi2(Double_t DPhi) {
  DPhi = TMath::ATan2(TMath::Sin(DPhi), TMath::Cos(DPhi));
  if (DPhi < -1.178097)    DPhi += 2 * TMath::Pi();
  return DPhi;
}
