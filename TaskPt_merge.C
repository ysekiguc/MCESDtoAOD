void TaskPt_merge(const char *dir, Int_t stage=0)
{
// Automatically generated merging macro executed in grid subjobs

   TStopwatch timer;
   timer.Start();

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

// Analysis source to be compiled at runtime (if any)
   gROOT->ProcessLine(".L AliAnalysisTaskSEpPbCorrelationsForward.cxx+g");

// Connect to AliEn
   if (!TGrid::Connect("alien://")) return;
// Set temporary merging directory to current one
   gSystem->Setenv("TMPDIR", gSystem->pwd());

// Set temporary compilation directory to current one
   gSystem->SetBuildDir(gSystem->pwd(), kTRUE);

   TString outputDir = dir;
   TString outputFiles = "EventStat_temp.root,AnalysisResults.root,pyxsec_hists.root";
   TString mergeExcludes = "AliAOD.root EventStat_temp.root ";
   TObjArray *list = outputFiles.Tokenize(",");
   TIter *iter = new TIter(list);
   TObjString *str;
   TString outputFile;
   Bool_t merged = kTRUE;
   AliAnalysisManager *mgr = AliAnalysisAlien::LoadAnalysisManager("TaskPt.root");
   if (!mgr) {
      printf("ERROR: Analysis manager could not be extracted from file ");
      return;
   }
   while((str=(TObjString*)iter->Next())) {
      outputFile = str->GetString();
      if (outputFile.Contains("*")) continue;
      Int_t index = outputFile.Index("@");
      if (index > 0) outputFile.Remove(index);
      // Skip already merged outputs
      if (!gSystem->AccessPathName(outputFile)) {
         printf("Output file <%s> found. Not merging again.\n",outputFile.Data());
         continue;
      }
      if (mergeExcludes.Contains(outputFile.Data())) continue;
      merged = AliAnalysisAlien::MergeOutput(outputFile, outputDir, 100, stage);
      if (!merged) {
         printf("ERROR: Cannot merge %s\n", outputFile.Data());
         return;
      }
   }
   // all outputs merged, validate
   ofstream out;
   out.open("outputs_valid", ios::out);
   out.close();
   // read the analysis manager from file
   if (!outputDir.Contains("Stage")) return;
   mgr->SetRunFromPath(mgr->GetRunFromAlienPath(dir));
   mgr->SetSkipTerminate(kFALSE);
   mgr->PrintStatus();
   AliLog::SetGlobalLogLevel(AliLog::kError);
   TTree *tree = NULL;
   mgr->StartAnalysis("gridterminate", tree);
}

