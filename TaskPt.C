TChain* CreateChain(const char *xmlfile, const char *type="ESD");

const char *anatype = "ESD";

void TaskPt()
{
// Analysis using ESD/MC
// Automatically generated analysis steering macro executed in grid subjobs

   TStopwatch timer;
   timer.Start();

// connect to AliEn and make the chain
   if (!TGrid::Connect("alien://")) return;
// Set temporary merging directory to current one
   gSystem->Setenv("TMPDIR", gSystem->pwd());

// Set temporary compilation directory to current one
   gSystem->SetBuildDir(gSystem->pwd(), kTRUE);

// Reset existing include path and add current directory first in the search
   gSystem->SetIncludePath("-I.");
// load base root libraries
   gSystem->Load("libTree");
   gSystem->Load("libGeom");
   gSystem->Load("libVMC");
   gSystem->Load("libPhysics");

   gSystem->Load("libMinuit");

// Load analysis framework libraries
   gSystem->Load("libSTEERBase");
   gSystem->Load("libESD");
   gSystem->Load("libAOD");
   gSystem->Load("libANALYSIS");
   gSystem->Load("libANALYSISalice");
   gSystem->Load("libOADB");
   gSystem->Load("libCORRFW");

// include path
   TString intPath = gInterpreter->GetIncludePath();
   TObjArray *listpaths = intPath.Tokenize(" ");
   TIter nextpath(listpaths);
   TObjString *pname;
   while ((pname=(TObjString*)nextpath())) {
      TString current = pname->GetName();
      if (current.Contains("AliRoot") || current.Contains("ALICE_ROOT")) continue;
      gSystem->AddIncludePath(current);
   }
   if (listpaths) delete listpaths;
   gSystem->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include  -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_ROOT/OADB -I/$ALICE_PHYSICS/OADB -I$ALICE_ROOT/PWGHF -I$ALICE_ROOT/PWGHF/base -I$ALICE_ROOT/PWGHF/vertexingHF -I$ALICE_ROOT/PWG/FLOW/Base -I$ALICE_ROOT/PWG/FLOW/Tasks -I$ALICE_ROOT/PWGDQ/dielectron -I$ALICE_ROOT/PWGCF/Correlations/macros/underlyingevent -I$ALICE_ROOT/VZERO -I$ALICE_ROOT/PWG/Tools -I$ALICE_ROOT/PWGCF/Correlations/Base -I$ALICE_ROOT/PWGCF/Correlations/DPhi -I$ALICE_ROOT/PWGLF/FORWARD/analysis2  -I$ALICE_ROOT/OADB/COMMON    -g ");
   gROOT->ProcessLine(".include $ALICE_ROOT/include");
   printf("Include path: %s\n", gSystem->GetIncludePath());

// Add aditional AliRoot libraries
   gSystem->Load("libCore");
   gSystem->Load("libTree");
   gSystem->Load("libGeom");
   gSystem->Load("libVMC");
   gSystem->Load("libPhysics");
   gSystem->Load("libSTEERBase");
   gSystem->Load("libESD");
   gSystem->Load("libAOD");
   gSystem->Load("libANALYSIS");
   gSystem->Load("libANALYSISalice");
   gSystem->Load("libCORRFW");
   gSystem->Load("libPWGTools");
   gSystem->Load("libPWGGlauber");
   gSystem->Load("libPWGflowBase");
   gSystem->Load("libPWGCFCorrelationsBase");
   gSystem->Load("libPWGCFCorrelationsDPhi");
   gSystem->Load("libPWGCFCorrelationsJCORRAN");
   gSystem->Load("libPWGLFforward2");
   gSystem->Load("libOADB");
   gSystem->Load("libADbase");
   gSystem->Load("libADrec");
   gSystem->Load("libADsim");
   gSystem->Load("libVZERObase");
   gSystem->Load("libVZEROrec");
   gSystem->Load("libVZEROsim");

// analysis source to be compiled at runtime (if any)
   gROOT->ProcessLine(".L AliAnalysisTaskSEpPbCorrelationsForward.cxx+g");

// read the analysis manager from file
   AliAnalysisManager *mgr = AliAnalysisAlien::LoadAnalysisManager("TaskPt.root");
   if (!mgr) return;
   mgr->PrintStatus();
   AliLog::SetGlobalLogLevel(AliLog::kError);
   TChain *chain = CreateChain("wn.xml", anatype);

   mgr->StartAnalysis("localfile", chain, 1234567890, 0);
   timer.Stop();
   timer.Print();
}

//________________________________________________________________________________
TChain* CreateChain(const char *xmlfile, const char *type)
{
// Create a chain using url's from xml file
   TString filename;
   Int_t run = 0;
   TString treename = type;
   treename.ToLower();
   treename += "Tree";
   printf("***************************************\n");
   printf("    Getting chain of trees %s\n", treename.Data());
   printf("***************************************\n");
   TAlienCollection *coll = dynamic_cast<TAlienCollection *>(TAlienCollection::Open(xmlfile));
   if (!coll) {
      ::Error("CreateChain", "Cannot create an AliEn collection from %s", xmlfile);
      return NULL;
   }
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   TChain *chain = new TChain(treename);
   coll->Reset();
   while (coll->Next()) {
      filename = coll->GetTURL();
      if (mgr) {
         Int_t nrun = AliAnalysisManager::GetRunFromAlienPath(filename);
         if (nrun && nrun != run) {
            printf("### Run number detected from chain: %d\n", nrun);
            mgr->SetRunFromPath(nrun);
            run = nrun;
         }
      }
      chain->Add(filename);
   }
   if (!chain->GetNtrees()) {
      ::Error("CreateChain", "No tree found from collection %s", xmlfile);
      return NULL;
   }
   return chain;
}

