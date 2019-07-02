AliAnalysisGrid* CreateAlienHandler(Bool_t isMC,Bool_t fAOD){
  char *data     = "LHC17f2a_fast";
  char *datatype = "FAST"  ;//Fast,wSDD,woSDD
  Int_t method   =  1;
  // Check if user has a valid token, otherwise make one. This has limitations.
  // One can always follow the standard procedure of calling alien-token-init then
  // source /tmp/gclient_env_$UID in the current shell.

  // if (!AliAnalysisGrid::CreateToken()) return NULL;
  AliAnalysisAlien *plugin = new AliAnalysisAlien();
  plugin->SetOverwriteMode();
  plugin->SetMergeViaJDL(1);
  plugin->SetRunMode("full");
  //        plugin->SetRunMode("terminate");
  //  plugin->SetRunMode("test");
  
  //Set versions of used packages
  plugin->SetAPIVersion("V1.1x");
  //plugin->SetAliPhysicsVersion("vAN-20170628-1");
  //plugin->SetAliPhysicsVersion("vAN-20170722-1");//have issuse to deal with primary particle so it is removed.
  //plugin->SetAliPhysicsVersion("vAN-20170824-1");
  //plugin->SetAliPhysicsVersion("vAN-20171218-1");
  //plugin->SetAliPhysicsVersion("vAN-20180325-1");
  //  plugin->SetAliPhysicsVersion("vAN-20180708-1");
  //plugin->SetAliPhysicsVersion("vAN-20180803-1");
  //  plugin->SetAliPhysicsVersion("v5-08-13zc-01-cookdedx-1");
  // plugin->SetAliROOTVersion("v5-34-30-alice7");
  plugin->SetAliPhysicsVersion("vAN-20180919-1");
  
  // Declare input data to be processed.
  // Method 1: Create automatically XML collections using alien 'find' command.
  // Define production directory LFN
  
  if(!isMC){
    plugin->SetRunPrefix("000"); // IMPORTANT!
  }

  plugin->SetCheckCopy(kFALSE);
  if(!isMC){
  if(method==1){
    if(data=="LHC16q"){
      plugin->SetGridDataDir("/alice/data/2016/LHC16q");
      if(datatype=="wSDD")plugin->SetDataPattern("*pass1_CENT_wSDD/AOD/*/AliAOD.root");
      else if(datatype=="woSDD")plugin->SetDataPattern("*pass1_CENT_woSDD/AOD/*/AliAOD.root");
      else if(datatype=="FAST")plugin->SetDataPattern("*pass1_FAST/AOD/*/AliAOD.root");
      //plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265305); // remove because no ZDC
      // plugin->AddRunNumber(265308);  // remove because no ZDC
      plugin->AddRunNumber(265309);
      /*
      plugin->AddRunNumber(265332);
      plugin->AddRunNumber(265334);
      //      plugin->AddRunNumber(265335); // remove because of no TOF in data taking
      plugin->AddRunNumber(265336);
      plugin->AddRunNumber(265338);
      plugin->AddRunNumber(265339);
      plugin->AddRunNumber(265342);
      plugin->AddRunNumber(265343);
      plugin->AddRunNumber(265344);
      plugin->AddRunNumber(265377);
      plugin->AddRunNumber(265378);
      plugin->AddRunNumber(265381);
      plugin->AddRunNumber(265383);
      plugin->AddRunNumber(265384);
      plugin->AddRunNumber(265385);
      plugin->AddRunNumber(265387);
      plugin->AddRunNumber(265388);
      plugin->AddRunNumber(265419);
      plugin->AddRunNumber(265420);
      plugin->AddRunNumber(265421);
      plugin->AddRunNumber(265422);
      plugin->AddRunNumber(265424);
      plugin->AddRunNumber(265425);
      plugin->AddRunNumber(265426);
      plugin->AddRunNumber(265427);
      plugin->AddRunNumber(265435);
      plugin->AddRunNumber(265499);
      plugin->AddRunNumber(265500);
      plugin->AddRunNumber(265501);
      plugin->AddRunNumber(265521);
      plugin->AddRunNumber(265525);
      */
    }else if(data=="LHC15n"){
      plugin->SetGridDataDir("/alice/data/2015/LHC15n");
      plugin->SetDataPattern("*pass4/AOD/*/AliAOD.root");
      plugin->AddRunNumber(244340);
      plugin->AddRunNumber(244343);
      plugin->AddRunNumber(244351);
      plugin->AddRunNumber(244355);
      plugin->AddRunNumber(244359);
      plugin->AddRunNumber(244364);
      plugin->AddRunNumber(244377);
      plugin->AddRunNumber(244411);
      plugin->AddRunNumber(244416);
      plugin->AddRunNumber(244418);
      plugin->AddRunNumber(244421);
      plugin->AddRunNumber(244453);
      plugin->AddRunNumber(244456);
      plugin->AddRunNumber(244480);
      plugin->AddRunNumber(244481);
      plugin->AddRunNumber(244482);
      plugin->AddRunNumber(244483);
      plugin->AddRunNumber(244484);
      plugin->AddRunNumber(244531);
      plugin->AddRunNumber(244540);
      plugin->AddRunNumber(244542);
      plugin->AddRunNumber(244617);
      plugin->AddRunNumber(244618);
      plugin->AddRunNumber(244619);
      plugin->AddRunNumber(244626);
      plugin->AddRunNumber(244627);
      plugin->AddRunNumber(244628);
    }else if(data=="LHC13b2_p1"){
      plugin->SetGridDataDir("/alice/sim/2013/LHC13b2_efix_p1");
      plugin->SetDataPattern("*/AOD/AliAOD.root");
      plugin->AddRunNumber(195344);
      plugin->AddRunNumber(195346);
      plugin->AddRunNumber(195351);
      plugin->AddRunNumber(195389);
      plugin->AddRunNumber(195390);
      plugin->AddRunNumber(195391);
      plugin->AddRunNumber(195478);
      plugin->AddRunNumber(195479);
      plugin->AddRunNumber(195480);
      plugin->AddRunNumber(195481);
      plugin->AddRunNumber(195482);
      plugin->AddRunNumber(195483);
      plugin->AddRunNumber(195529);
      plugin->AddRunNumber(195531);
      plugin->AddRunNumber(195566);
      plugin->AddRunNumber(195567);
      plugin->AddRunNumber(195568);
      plugin->AddRunNumber(195592);
      plugin->AddRunNumber(195593);
      plugin->AddRunNumber(195596);
      plugin->AddRunNumber(195633);
      plugin->AddRunNumber(195635);
      plugin->AddRunNumber(195644);
      plugin->AddRunNumber(195673);
      plugin->AddRunNumber(195675);
      plugin->AddRunNumber(195677);
    }
  }else{
    //   plugin->SetOutputSingleFolder("output");
    //   plugin->SetOutputToRunNo();
    //   Method 2: Declare existing data files (raw collections, xml collections, root file)
    //   If no path mentioned data is supposed to be in the work directory (see SetGridWorkingDir())
    //   XML collections added via this method can be combined with the first method if
    //   the content is compatible (using or not tags)
    //   if(data=="LHC16q" && datatype=="wSDD") plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/xml/pass1_CENT_wSDD/000265305_000265525.xml");//old good run list
    if(data=="LHC16q" && datatype=="wSDD") plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/xml/pass1_CENT_wSDD/000265309_000265525.xml");//32 runs
    if(data=="LHC16q" && datatype=="woSDD") plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/xml/pass1_CENT_woSDD/000265309_000265525.xml");//32 runs
    if(data=="LHC16q" && datatype=="FAST") plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/xml/pass1_FAST/000265309_000265525.xml");//32 runs
    if(data=="LHC15n") plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/xml/LHC15n/pass4/000244340_000244628.xml");
    //   plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");
    //   Define alien work directory where all files will be copied. Relative to alien $HOME.
  }
}else{
  if(method==1){
    if(data=="LHC17f2b_fast") plugin->SetGridDataDir("/alice/sim/2017/LHC17f2b_fast");
    else if(data=="LHC17f2a_fast") plugin->SetGridDataDir("/alice/sim/2017/LHC17f2a_fast_fix");
    //	plugin->SetDataPattern("AOD/*/AliAOD.root");
    plugin->SetDataPattern("/*/AliESDs.root");
    
    plugin->AddRunNumber(265309);
    plugin->AddRunNumber(265332);
    plugin->AddRunNumber(265334);
	//      plugin->AddRunNumber(265335); // remove because of no TOF in data taking
	plugin->AddRunNumber(265336);
	plugin->AddRunNumber(265338);

    //plugin->AddRunNumber(265339);
    //plugin->AddRunNumber(265342);
      
    //    plugin->AddRunNumber(265343);
      /*
      plugin->AddRunNumber(265344);
      plugin->AddRunNumber(265377);

      plugin->AddRunNumber(265378);
      plugin->AddRunNumber(265381);
      plugin->AddRunNumber(265383);
      plugin->AddRunNumber(265384);
      plugin->AddRunNumber(265385);
      plugin->AddRunNumber(265387);
      plugin->AddRunNumber(265388);
      plugin->AddRunNumber(265419);
      plugin->AddRunNumber(265420);
      plugin->AddRunNumber(265421);
      plugin->AddRunNumber(265422);
      plugin->AddRunNumber(265424);
      plugin->AddRunNumber(265425);
      plugin->AddRunNumber(265426);
      plugin->AddRunNumber(265427);
      plugin->AddRunNumber(265435);
      plugin->AddRunNumber(265499);
      plugin->AddRunNumber(265500);
      plugin->AddRunNumber(265501);
      plugin->AddRunNumber(265521);
      plugin->AddRunNumber(265525);
      */

    }else if(method==2){

      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265309.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265332.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265334.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265336.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265338.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265339.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265342.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265343.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265344.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265377.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265378.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265381.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265383.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265384.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265385.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265387.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265388.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265419.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265420.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265421.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265422.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265424.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265425.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265426.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265427.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265435.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265499.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265500.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265501.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265521.xml");
      plugin->AddDataFile("/alice/cern.ch/user/y/ysekiguc/01122017_MC/265525.xml");
    }
}


  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_02102018_265309_vAN-20180919-1");
  ///  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_02102018_265332_vAN-20180919-1");
  //plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_02102018_265334_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_02102018_265336_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_02102018_265338_vAN-20180919-1");


  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMDC_02102018_265309_265336_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMDC_02102018_265338_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMDC_22112018_265339_vAN-20180919-1");
  // plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMDC_22112018_265342_vAN-20180919-1");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_22112018_265342_vAN-20180919-1");
  //plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_22112018_265339_vAN-20180919-1");

  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/ForwardMC/TPCFMD_102018_265309_265336_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/ForwardMC/TPCFMDC_102018_265309_265336_vAN-20180919-1");

    
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265309_vAN-20180919-1");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265332_vAN-20180919-1");
  //      plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265334_vAN-20180919-1");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265336_vAN-20180919-1");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265338_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265339_vAN-20180919-1");
  //  plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/NoMultiplicitycut/TPCFMD_17092018_265309_vAN-20180919-1_differentaliphysics");

  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_05022019_265309_265336_vAN-20180919-1_bootstrap");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMD_05022019_265309_265336_vAN-20180919-1_bootstrap_poolmintrack5000");
  //        plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/TPCFMDA_05022019_265309_265336_vAN-20180919-1_bootstrap_poolmintrack5000");

  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDA_01052019_265309_265336");
  //     plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDA_01052019_265309_265336_v1");

  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDC_01052019_265338");
  //     plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDA_01052019_265338");

  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDA_01052019_265339_265342");
  //        plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDC_01052019_265339_265342");

  //      plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDA_05052019_265309_265338");
  //    plugin->SetGridWorkingDir("EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDC_05052019_265309_265338");
  
  //  plugin->SetGridWorkingDir("01072019/EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDC_05052019_265309_265338");
  plugin->SetGridWorkingDir("01072019/EPOS_LHC17f2a/ESDtoAOD/tpctrack/TPCFMDC_02072019_265309_265338");

  // Declare alien output directory. Relative to working directory.
  plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
  plugin->SetUser("ysekiguc");
  // Declare the analysis source files names separated by blancs. To be compiled runtime
  // using ACLiC on the worker nodes.

  plugin->SetAnalysisSource("AliAnalysisTaskSEpPbCorrelationsForward.cxx");
  // Declare all libraries (other than the default ones for the framework. These will be
  // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.

  plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include  -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_ROOT/OADB -I/$ALICE_PHYSICS/OADB -I$ALICE_ROOT/PWGHF -I$ALICE_ROOT/PWGHF/base -I$ALICE_ROOT/PWGHF/vertexingHF -I$ALICE_ROOT/PWG/FLOW/Base -I$ALICE_ROOT/PWG/FLOW/Tasks -I$ALICE_ROOT/PWGDQ/dielectron -I$ALICE_ROOT/PWGCF/Correlations/macros/underlyingevent -I$ALICE_ROOT/VZERO -I$ALICE_ROOT/PWG/Tools -I$ALICE_ROOT/PWGCF/Correlations/Base -I$ALICE_ROOT/PWGCF/Correlations/DPhi -I$ALICE_ROOT/PWGLF/FORWARD/analysis2  -I$ALICE_ROOT/OADB/COMMON    -g");

   plugin->SetAdditionalLibs("libCore.so  libTree.so  libGeom.so  libVMC.so  libPhysics.so libSTEERBase.so libESD.so  libAOD.so  libANALYSIS.so  libANALYSISalice.so libCORRFW.so  libPWGTools.so  libPWGGlauber.so libPWGflowBase.so libPWGCFCorrelationsBase.so libPWGCFCorrelationsDPhi.so  libPWGCFCorrelationsJCORRAN.so libPWGLFforward2.so  libOADB.so  libADbase.so libADrec.so libADsim.so libVZERObase.so libVZEROrec.so  libVZEROsim.so AliAnalysisTaskSEpPbCorrelationsForward.h  AliAnalysisTaskSEpPbCorrelationsForward.cxx");

   // Declare the output file names separated by blancs.
   // (can be like: file.root or file.root@ALICE::Niham::File)
   plugin->SetDefaultOutputs("");
   plugin->SetMergeExcludes("AliAOD.root,EventStat_temp.root");
   // Optionally define the files to be archived.
   //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr@ALICE::NIHAM::File root_archive.zip:*.root@ALICE::NIHAM::File");
   //   plugin->SetOutputArchive("log_archive.zip:stdout,stderr");

   // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro("TaskPt.C");

   // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   if(data=="LHC16q" )plugin->SetNrunsPerMaster(35);
   else if(data=="LHC15n")plugin->SetNrunsPerMaster(35);
   //   else if(  data=="LHC17f2b_fast" ||data=="LHC17f2a_fast" )plugin->SetNrunsPerMaster(35);
   else if(data=="LHC17f2a_fast" )plugin->SetNrunsPerMaster(1);
   else plugin->SetNrunsPerMaster(1);

   if(isMC){
     //         plugin->SetSplitMaxInputFileNumber(5);
     plugin->SetSplitMaxInputFileNumber(200);
   }else{
     if(fAOD){
       //plugin->SetSplitMaxInputFileNumber(10);
       //       plugin->SetSplitMaxInputFileNumber(20);
       //       plugin->SetSplitMaxInputFileNumber(20);
        plugin->SetSplitMaxInputFileNumber(50);
       //       plugin->SetSplitMaxInputFileNumber(10);

     }else{
       plugin->SetSplitMaxInputFileNumber(100);
     }
   }

   // Optionally modify the executable name (default analysis.sh)
   plugin->SetExecutable("TaskPt.sh");

   // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
   //   plugin->SetMaxInitFailed(5);

   // Optionally resubmit threshold.
   //   plugin->SetMasterResubmitThreshold(90);

   // Optionally set time to live (default 30000 sec)
   plugin->SetTTL(30000);

   // Optionally set input format (default xml-single)
   plugin->SetInputFormat("xml-single");

   // Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskPt.jdl");

   // Optionally modify job price (default 1)
   plugin->SetPrice(1);

   // Optionally modify split mode (default 'se')
   plugin->SetSplitMode("se");

   return plugin;
}
