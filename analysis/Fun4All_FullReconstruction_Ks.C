/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <fun4all/Fun4AllUtils.h>
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <GlobalVariables.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <cdbobjects/CDBTTree.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackingqa/InttClusterQA.h>
#include <trackingqa/MicromegasClusterQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/TpcClusterQA.h>
#include <tpcqa/TpcRawHitQA.h>
#include <trackingqa/TpcSeedsQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>
//#include <trackingdiagnostics/KshortReconstruction.h>

#include <stdio.h>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wundefined-internal"

#include <kfparticle_sphenix/KFParticle_sPHENIX.h>
#include <kfparticle_sphenix/KshortReconstruction_local.h>

#pragma GCC diagnostic pop

void KsReco(std::string outfile = "KsReco.root");
void KFPReco_Ks(std::string module_name = "myKShortReco", std::string decaydescriptor = "K_S0 -> pi^+ pi^-", std::string outfile = "KFP.root");
void KFPReco_D0(std::string module_name = "myD0Reco", std::string decaydescriptor = "[D0 -> K^- pi^+]cc", std::string outfile = "KFP.root");

R__LOAD_LIBRARY(libkfparticle_sphenix.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
R__LOAD_LIBRARY(libtpcqa.so)
void Fun4All_FullReconstruction_Ks(
    const int nEvents = 10,
    const std::string tpcfilename = "DST_STREAMING_EVENT_run2pp_new_2024p002-00053222-00000.root",
    const std::string tpcdir = "/sphenix/lustre01/sphnxpro/physics/slurp/streaming/physics/new_2024p002/run_00053200_00053300/",
    const std::string outfilename = "clusters_seeds",
    const std::string outdir = "./root",
    const int runnumber = 53222,
    const int segment = 0,
    const int index = 0,
    const int stepsize = 10)
{
  std::string inputtpcRawHitFile = tpcdir + tpcfilename;

  std::pair<int, int>
      runseg = Fun4AllUtils::GetRunSegment(tpcfilename);
  //int runnumber = runseg.first;
  //int segment = runseg.second;

  string outDir = outdir + "/inReconstruction/" + to_string(runnumber) + "/";
  string makeDirectory = "mkdir -p " + outDir;
  system(makeDirectory.c_str());
  TString outfile = outDir + outfilename + "_" + runnumber + "-" + segment + "-" + index + ".root";
  std::cout<<"outfile "<<outfile<<std::endl;
  std::string theOutfile = outfile.Data();

  auto se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(inputtpcRawHitFile);
  // hitsin->AddFile(inputMbd);
  se->registerInputManager(hitsin);

  Global_Reco();

  TString ksfile = theOutfile + "_ks_ksreco.root";
  std::string ksstring(ksfile.Data());
  KsReco(ksstring);

  TString ks_kfp_likesign_outfile = theOutfile + "_ks_kfp_likesign.root";
  std::string ks_kfp_likesign_string(ks_kfp_likesign_outfile.Data());
  KFPReco_Ks("myKShortReco_KFP_likesign", "[K_S0 -> pi^+ pi^+]cc", ks_kfp_likesign_string);

  TString ks_kfp_unlikesign_outfile = theOutfile + "_ks_kfp_unlikesign.root";
  std::string ks_kfp_unlikesign_string(ks_kfp_unlikesign_outfile.Data());
  KFPReco_Ks("myKShortReco_KFP_unlikesign", "K_S0 -> pi^+ pi^-", ks_kfp_unlikesign_string);

  TString D0_kfp_likesign_outfile = theOutfile + "_D0_kfp_likesign.root";
  std::string D0_kfp_likesign_string(D0_kfp_likesign_outfile.Data());
  KFPReco_D0("myD0eco_KFP_likesign", "[D0 -> K^+ pi^+]cc", D0_kfp_likesign_string);

  TString D0_kfp_unlikesign_outfile = theOutfile + "_D0_kfp_unlikesign.root";
  std::string D0_kfp_unlikesign_string(D0_kfp_unlikesign_outfile.Data());
  KFPReco_D0("myD0eco_KFP_unlikesign", "[D0 -> K^- pi^+]cc", D0_kfp_unlikesign_string);

  se->skip(stepsize*index);
  se->run(nEvents);
  se->End();
  se->PrintTimer();

  ifstream file_ksreco(ksstring.c_str(), ios::binary | ios::ate);
  if (file_ksreco.good() && (file_ksreco.tellg() > 100))
  {
    string outputDirMove = outdir + "/Reconstructed/" + to_string(runnumber) + "/";
    string makeDirectoryMove = "mkdir -p " + outputDirMove;
    system(makeDirectoryMove.c_str());
    string moveOutput = "mv " + ksstring + " " + outputDirMove;
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  ifstream file_ks_kfp_likesign(ks_kfp_likesign_string.c_str(), ios::binary | ios::ate);
  if (file_ks_kfp_likesign.good() && (file_ks_kfp_likesign.tellg() > 100))
  {
    string outputDirMove = outdir + "/Reconstructed/" + to_string(runnumber) + "/";
    string makeDirectoryMove = "mkdir -p " + outputDirMove;
    system(makeDirectoryMove.c_str());
    string moveOutput = "mv " + ks_kfp_likesign_string + " " + outputDirMove;
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  ifstream file_ks_kfp_unlikesign(ks_kfp_unlikesign_string.c_str(), ios::binary | ios::ate);
  if (file_ks_kfp_unlikesign.good() && (file_ks_kfp_unlikesign.tellg() > 100))
  {
    string outputDirMove = outdir + "/Reconstructed/" + to_string(runnumber) + "/";
    string makeDirectoryMove = "mkdir -p " + outputDirMove;
    system(makeDirectoryMove.c_str());
    string moveOutput = "mv " + ks_kfp_unlikesign_string + " " + outputDirMove;
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  ifstream file_D0_kfp_likesign(D0_kfp_likesign_string.c_str(), ios::binary | ios::ate);
  if (file_D0_kfp_likesign.good() && (file_D0_kfp_likesign.tellg() > 100))
  {
    string outputDirMove = outdir + "/Reconstructed/" + to_string(runnumber) + "/";
    string makeDirectoryMove = "mkdir -p " + outputDirMove;
    system(makeDirectoryMove.c_str());
    string moveOutput = "mv " + D0_kfp_likesign_string + " " + outputDirMove;
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  ifstream file_D0_kfp_unlikesign(D0_kfp_unlikesign_string.c_str(), ios::binary | ios::ate);
  if (file_D0_kfp_unlikesign.good() && (file_D0_kfp_unlikesign.tellg() > 100))
  {
    string outputDirMove = outdir + "/Reconstructed/" + to_string(runnumber) + "/";
    string makeDirectoryMove = "mkdir -p " + outputDirMove;
    system(makeDirectoryMove.c_str());
    string moveOutput = "mv " + D0_kfp_unlikesign_string + " " + outputDirMove;
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}

void KsReco(std::string outfile = "KsReco.root")
{
  auto se = Fun4AllServer::instance();
  auto kshort=new KshortReconstruction_local();
  kshort->Verbosity(0);
  kshort->setTrackQualityCut(1000);
  kshort->setPtCut(0.2);
  kshort->setTrackPtCut(0.2);
  kshort->setPairDCACut(5.0);
  kshort->setRequireMVTX(true);
  kshort->setTrackDCACut(0.0);  // requires fabs(dca) > this
  kshort->set_output_file(outfile);
  se->registerSubsystem(kshort);
}

void KFPReco_Ks(std::string module_name = "myKShortReco", std::string decaydescriptor = "K_S0 -> pi^+ pi^-", std::string outfile = "KFP.root")
{
  auto se = Fun4AllServer::instance();
  //KFParticle setup
  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(module_name);
  kfparticle->Verbosity(0);
  kfparticle->setDecayDescriptor(decaydescriptor);

  //Basic node selection and configuration
  kfparticle->magFieldFile("FIELDMAP_TRACKING");
  kfparticle->getAllPVInfo(false);
  kfparticle->allowZeroMassTracks(true);
  kfparticle->getDetectorInfo(true);
  kfparticle->useFakePrimaryVertex(false);

  kfparticle->constrainToPrimaryVertex(true);
  kfparticle->setMotherIPchi2(FLT_MAX);
  kfparticle->setFlightDistancechi2(-1.);
  kfparticle->setMinDIRA(-1.1);
  kfparticle->setDecayLengthRange(0., FLT_MAX);
  kfparticle->setDecayTimeRange(-1*FLT_MAX, FLT_MAX);

  //Track parameters
  kfparticle->setMinMVTXhits(0);
  //kfparticle->setMinTPChits(20);
  kfparticle->setMinTPChits(0);
  kfparticle->setMinimumTrackPT(0.2);
  kfparticle->setMaximumTrackPTchi2(FLT_MAX);
  kfparticle->setMinimumTrackIPchi2(-1.);
  kfparticle->setMinimumTrackIP(-1.);
  //kfparticle->setMaximumTrackchi2nDOF(100.);
  kfparticle->setMaximumTrackchi2nDOF(FLT_MAX);

  //Vertex parameters
  //kfparticle->setMaximumVertexchi2nDOF(50);
  kfparticle->setMaximumVertexchi2nDOF(FLT_MAX);
  //kfparticle->setMaximumDaughterDCA(1.);
  kfparticle->setMaximumDaughterDCA(FLT_MAX);

  //Parent parameters
  kfparticle->setMotherPT(0);
  kfparticle->setMinimumMass(0.200);
  kfparticle->setMaximumMass(1.000);
  //kfparticle->setMaximumMotherVertexVolume(0.1);
  kfparticle->setMaximumMotherVertexVolume(FLT_MAX);

  kfparticle->setOutputName(outfile);

  se->registerSubsystem(kfparticle);
}

void KFPReco_D0(std::string module_name = "myD0Reco", std::string decaydescriptor = "[D0 -> K^- pi^+]cc", std::string outfile = "KFP.root")
{
  auto se = Fun4AllServer::instance();
  //KFParticle setup
  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX(module_name);
  kfparticle->Verbosity(0);
  kfparticle->setDecayDescriptor(decaydescriptor);

  //Basic node selection and configuration
  kfparticle->magFieldFile("FIELDMAP_TRACKING");
  kfparticle->getAllPVInfo(false);
  kfparticle->allowZeroMassTracks(true);
  kfparticle->getDetectorInfo(true);
  kfparticle->useFakePrimaryVertex(false);

  kfparticle->constrainToPrimaryVertex(true);
  kfparticle->setMotherIPchi2(FLT_MAX);
  kfparticle->setFlightDistancechi2(-1.);
  kfparticle->setMinDIRA(-1.1);
  kfparticle->setDecayLengthRange(0., FLT_MAX);
  kfparticle->setDecayTimeRange(-1*FLT_MAX, FLT_MAX);

  //Track parameters
  kfparticle->setMinMVTXhits(0);
  //kfparticle->setMinTPChits(20);
  kfparticle->setMinTPChits(0);
  kfparticle->setMinimumTrackPT(0.2);
  kfparticle->setMaximumTrackPTchi2(FLT_MAX);
  kfparticle->setMinimumTrackIPchi2(-1.);
  kfparticle->setMinimumTrackIP(-1.);
  //kfparticle->setMaximumTrackchi2nDOF(100.);
  kfparticle->setMaximumTrackchi2nDOF(FLT_MAX);

  //Vertex parameters
  //kfparticle->setMaximumVertexchi2nDOF(50);
  kfparticle->setMaximumVertexchi2nDOF(FLT_MAX);
  //kfparticle->setMaximumDaughterDCA(1.);
  kfparticle->setMaximumDaughterDCA(FLT_MAX);

  //Parent parameters
  kfparticle->setMotherPT(0);
  kfparticle->setMinimumMass(1.5);
  kfparticle->setMaximumMass(2.2);
  //kfparticle->setMaximumMotherVertexVolume(0.1);
  kfparticle->setMaximumMotherVertexVolume(FLT_MAX);

  kfparticle->setOutputName(outfile);

  se->registerSubsystem(kfparticle);
}
