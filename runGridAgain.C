{
  char *gridMode="full";
  Bool_t iAODAnalysis =kFALSE;
  Bool_t useMC =kTRUE;
  Bool_t frun2=kTRUE;

  //Load common libraries
  gSystem->Load("libCore.so");
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libSTEER");
  gSystem->Load("libRAWDatabase");
  gSystem->Load("libRAWDatarec");
  gSystem->Load("libHist");
  gSystem->Load("libRIO");
  //gSystem->Load("libEveDet");
  //gSystem->Load("libCDB");
  //gSystem->Load("libESDfilter");
  gSystem->Load("libAOD");
  gSystem->Load("libESD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libCORRFW");
  gSystem->Load("libADbase");
  gSystem->Load("libADrec");
  gSystem->Load("libADsim");
  gSystem->Load("libOADB");

  //ANALYSIS
  gSystem->Load("libVZERObase.so");
  gSystem->Load("libVZEROrec.so");
  gSystem->Load("libVZEROsim.so");
  gSystem->Load("libANALYSIScalib.so");
  gSystem->Load("libEventMixing.so");
  gSystem->Load("libTender.so");
  gSystem->Load("libTenderSupplies.so");

  // PWG
  gSystem->Load("libPWGTools.so");
  gSystem->Load("libPWGGlauber.so");
  gSystem->Load("libPWGflowBase.so");
  gSystem->Load("libPWGflowTasks.so");
  gSystem->Load("libPWGmuon.so");
  gSystem->Load("libPWGmuondep.so");
  gSystem->Load("libPWGCaloTrackCorrBase.so");
  //PWGCF
  gSystem->Load("libPWGCFCorrelationsBase.so");
  gSystem->Load("libPWGCFCorrelationsDPhi.so"); //comment out on 19.12.2017
  //gSystem->Load("libPWGCFCorrelationsJCORRAN.so");
  gSystem->Load("libPWGCFChaoticity.so");
  //  gSystem->Load("libPWGCFFEMTOSCOPYAOD.so");
  // gSystem->Load("libPWGCFfemtoscopy.so");

  gSystem->Load("libPWGCFfemtoscopyUser.so");

  gSystem->Load("libPWGCFunicor.so");
  gSystem->Load("libPWGCFebye.so");
  gSystem->Load("libPWGCFflowCME.so");
  gSystem->Load("libPWGCFK0Analysis.so");
  gSystem->Load("libPWGCFflowBW.so");

  gSystem->Load("libPWGflowBase");
  gSystem->Load("libPWGflowTasks");

  gSystem->Load("libTPCbase.so");
  gSystem->Load("libTPCrec.so");
  gSystem->Load("libTPCsim.so");
  gSystem->Load("libTPCupgrade.so");
  gSystem->Load("libTPCutil.so");

  gSystem->Load("libACORDEbase.so");

  gSystem->Load("libPWGLFforward2");
  

  //  gSystem->Load("libESD.so");
  //gSystem->Load("libMinuit");

  // Use AliRoot includes to compile our task
  gROOT->ProcessLine(".include $ROOTSYS/include");
  gROOT->ProcessLine(".include $ALICE_ROOT/include");
  gROOT->ProcessLine(".include $ALICE_PHYSICS");
  gROOT->ProcessLine(".include $ALICE_PHYSICS/lib");
  gROOT->ProcessLine(".include $ALICE_ROOT/lib");

  gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include  -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_ROOT/OADB -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWG -I$ALICE_PHYSICS/PWGCF -I$ALICE_ROOT/PWGHF -I$ALICE_ROOT/PWGHF/base -I$ALICE_ROOT/PWGHF/vertexingHF -I$ALICE_ROOT/PWG/FLOW/Base -I$ALICE_ROOT/PWG/FLOW/Tasks -I$ALICE_ROOT/PWGDQ/dielectron -I$ALICE_ROOT/AD/macros -I/$ALICE_ROOT/STAT/Macros -I$ALICE_ROOT/VZERO -I$ALICE_PHYSICS/OADB -g");

  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");
  // Create and configure the alien handler plugin
  TChain*chain;
  if(gridMode!=""){
    gROOT->ProcessLine(".L CreateAlienHandler.C");
    AliAnalysisGrid *alienHandler = CreateAlienHandler(useMC,iAODAnalysis);
    mgr->SetGridHandler(alienHandler);
  }else{
    if(useMC){
      if(iAODAnalysis){
          chain=new TChain("aodTree");
	//       chain->Add("/Users/sekiguchi/alicework/alice_data/2017/LHC17f2b_fast/265309/001/AliAOD.root");//DPMJET
       chain->Add("/home/sekiguchi/work/local_alicework/MCESDanalysis/ali_data/AliAOD.root");//DPMJET

      }else{
          chain=new TChain("esdTree");
		//		chain->Add("/home/sekiguchi/work/local_alicework/alice_data/2017/LHC17f2b_fast/265309/001/AliESDs.root");//DPMJET
		chain->Add("~/workalice/alice_data/2017/LHC17f2a_fast_fix/265309/001/AliESDs.root");//EPOS

		
		/*
		chain->Add("/home/sekiguchi/work/local_alicework/MCESDanalysis/ali_data_PbPb/001/AliESDs.root");//AMPT of PbPb
		chain->Add("/home/sekiguchi/work/local_alicework/MCESDanalysis/ali_data_PbPb/002/AliESDs.root");//AMPT of PbPb
		*/
      }

    }else{
      if(!iAODAnalysis){
        TChain *chain =new TChain("esdTree");
        //	chain->Add("../testdata/AliESDs_12g_pass2_188359.root");
	//    chain->Add("input/2012/LHC12g/000188359/ESDs/pass2/12000188359001.10/AliESDs.root");
	//	chain->Add("../../tmp/AliESDs.root");
	//chain->Add("$HOME/alicework/alice_data/2013/LHC13c/000195529/ESDs/pass2/13000195529000.10/AliESDs.root");


  chain->Add("$HOME/alicework/alice_data/2016/LHC16q/000265309/pass1_CENT_wSDD/16000265309019.100/AliESDs.root");
}
else{
  TChain *chain =new TChain("aodTree");
	//chain->Add("../../alice_data/2013/LHC13c/000195529/ESDs/pass2/AOD/001/AliAOD.root");
	//chain->Add("../../alice_data/2013/LHC13b/000195344/ESDs/pass3/AOD/001/AliAOD.root");
	//		chain->Add("$HOME/alicework/alice_data/2016/LHC16q/000265309/pass1_CENT_wSDD/AOD/001/AliAOD.root");
	//	chain->Add("$HOME/alicework/alice_data/2016/LHC16q/000265309/pass1_CENT_wSDD/16000265309019.100/AliAOD.root");
	//
	//chain->Add("$HOME/alicework/alice_data/2016/LHC16q/000265309/pass1_FAST/001/AliAOD.root");
	chain->Add("$HOME/alicework/alice_data/2016/LHC16q/000265309/pass1_FAST/002/AliAOD.root");
	//	chain->Add("$HOME/alicework/alice_data/2015/LHC15n/000244340/pass4/AOD/001/AliAOD.root");

      }
    }

  }

//  Create the analysis manager
  // if AOD , AliMCEventHander is not necessary
  // if (useMC && !iAODAnalysis) {


  if(useMC){
      AliMCEventHandler* mcHandler = new AliMCEventHandler();
     mgr->SetMCtruthEventHandler(mcHandler);
     mcHandler->SetReadTR(useMC);
  }

   if(!iAODAnalysis){
     AliESDInputHandler* esdH = new AliESDInputHandler();
     mgr->SetInputEventHandler(esdH);
   }else{
     AliAODInputHandler *aodHandler = new AliAODInputHandler();
     mgr->SetInputEventHandler(aodHandler);
   }

 if(!iAODAnalysis){
	 AliAODHandler*aodoutputHandler=new AliAODHandler();
	 if(!aodoutputHandler) cout<<" NO AliAODHandler" <<endl;
	 aodoutputHandler->SetOutputFileName("AliAOD.root");
	 AliAnalysisManager::GetAnalysisManager()->SetOutputEventHandler(aodoutputHandler);
 }

 if (!iAODAnalysis) { 
   gROOT->ProcessLine(".L $ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
   AliTaskCDBconnect* cdb = AddTaskCDBconnect();
   AliCDBManager* cdbM = AliCDBManager::Instance();
   cdbM->SetDefaultStorageFromRun(265309);
 }
   gROOT->ProcessLine(".L $ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
   AddTaskPhysicsSelection(useMC,true);
 
  gROOT->ProcessLine(".L $ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  AliMultSelectionTask * taskMult = AddTaskMultSelection(); //
 if(!iAODAnalysis){
   // RSD filter for TPC tracks and more 

      gROOT->ProcessLine(".L $ALICE_ROOT/ANALYSIS/ESDfilter/macros/AddTaskESDFilter.C");
	  int run_flag = 1500;
      AliAnalysisTaskESDfilter *taskesdfilter = 
		AddTaskESDFilter(true, // Kinemticds tracks for MC 
						 false,         // write Muon AOD
						 false,               // write dimuon AOD 
						 false,               // usePhysicsSelection 
						 false,               // centrality OBSOLETE
						 false,                // enable TPS only tracks
						 false,               // disable cascades
						 false,               // disable kinks
						 run_flag,             // run flag (YY00)
						 3,                    // Muon mode
						 false,                // V0 filtering off
						 true,                 // Muon w/SPD tracklets
						 false,                // Not muon_calo (i.e.,full)
						 false);               // no add PCM V0s(?)
	  
	 gROOT->ProcessLine(".L $ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskForwardMult.C");
	 Bool_t   mc  = true; // false: real data, true: simulated data
	 ULong_t runNo = 195483;
	 UShort_t sys = 3; // 0: get from data, 1: pp, 2: AA 3: pA
	 UShort_t sNN = 5023; // 0: get from data, otherwise center of mass energy (per nucleon pair)
	 Short_t  fld = 5; // 0: get from data, otherwise L3 field in kG
	 AliAnalysisTask *task  = AddTaskForwardMult(mc, runNo, sys, sNN, fld);
	 }

 gROOT->ProcessLine(".L $ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
   //   AliAnalysisTaskPIDResponse *taskPID=AddTaskPIDResponse(kFALSE,kFALSE,kTRUE,1);
   AddTaskPIDResponse(useMC);
   
   //
   //  //   gROOT->LoadMacro("AddTaskPIDResponse.C");
   //  Bool_t isMC = kFALSE;
   //  Bool_t autoMCesd = kTRUE;
   //  Bool_t tuneOnData = kFALSE;
   //  Int_t recoPass = 2;
   //  Bool_t cachePID = kFALSE;
   //  Bool_t useTPCEtaCorrection = kTRUE;
   //  Bool_t useTPCMultiplicityCorrection = kFALSE;
   //   Int_t recoDataPass = 2;
   //  //  Int_t recoDataPass = 2;
   //
   //  AliAnalysisTaskSE *setupTask = AddTaskPIDResponse(isMC,autoMCesd,tuneOnData,recoPass,cachePID,"",useTPCEtaCorrection,useTPCMultiplicityCorrection,recoDataPass);


   gROOT->ProcessLine(".x AliAnalysisTaskSEpPbCorrelationsForward.cxx++g");
   gROOT->ProcessLine(".x AddTaskpPbCorrelationsForward.C");
   AliAnalysisTaskSEpPbCorrelationsForward* ana =AddTaskpPbCorrelationsForward();
   


   // Enable debug printouts
   mgr->SetDebugLevel(0);
   if (!mgr->InitAnalysis()) return;
   mgr->PrintStatus();
   // Start analysis in grid.
   if(gridMode!=""){
     mgr->StartAnalysis("grid");
   }
   else{
     mgr->StartAnalysis("local",chain);
   }
};
