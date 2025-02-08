#include <filesystem>
#include <sPhenixStyle.C>

void EvtDisplay2D_zoomin(int M_runNumber = 53744, int M_eventNumber = 6, int M_trackid1 = 12, int M_trackid2 = 41){

  //SetsPhenixStyle();

  TFile* file_ksreco = new TFile("../clusters_seeds_53744-0-0.root_ks_tony.root");
  TTree* tree_ksreco = (TTree*) file_ksreco->Get("ntp_reco_info");
  Float_t eventNumber_ksreco, id1, id2;
  Float_t projected_pca_rel1_x, projected_pca_rel1_y, projected_pca_rel1_z;
  Float_t projected_pca_rel2_x, projected_pca_rel2_y, projected_pca_rel2_z;
  Float_t vertex_x, vertex_y, vertex_z;
  Float_t cosThetaReco, projected_pathlength_x, projected_pathlength_y, projected_pathlength_z;
  Float_t projected_pair_dca;
  tree_ksreco->SetBranchAddress("eventNumber",&eventNumber_ksreco);
  tree_ksreco->SetBranchAddress("id1",&id1);
  tree_ksreco->SetBranchAddress("id2",&id2);
  tree_ksreco->SetBranchAddress("projected_pca_rel1_x",&projected_pca_rel1_x);
  tree_ksreco->SetBranchAddress("projected_pca_rel1_y",&projected_pca_rel1_y);
  tree_ksreco->SetBranchAddress("projected_pca_rel1_z",&projected_pca_rel1_z);
  tree_ksreco->SetBranchAddress("projected_pca_rel2_x",&projected_pca_rel2_x);
  tree_ksreco->SetBranchAddress("projected_pca_rel2_y",&projected_pca_rel2_y);
  tree_ksreco->SetBranchAddress("projected_pca_rel2_z",&projected_pca_rel2_z);
  tree_ksreco->SetBranchAddress("vertex_x",&vertex_x);
  tree_ksreco->SetBranchAddress("vertex_y",&vertex_y);
  tree_ksreco->SetBranchAddress("vertex_z",&vertex_z);
  tree_ksreco->SetBranchAddress("cosThetaReco",&cosThetaReco);
  tree_ksreco->SetBranchAddress("projected_pathlength_x",&projected_pathlength_x);
  tree_ksreco->SetBranchAddress("projected_pathlength_y",&projected_pathlength_y);
  tree_ksreco->SetBranchAddress("projected_pathlength_z",&projected_pathlength_z);
  tree_ksreco->SetBranchAddress("projected_pair_dca",&projected_pair_dca);

  float svx_ksreco, svy_ksreco, svz_ksreco;
  float pvx_ksreco, pvy_ksreco, pvz_ksreco;
  float dira_ksreco, dL_ksreco, pairdca_ksreco;
  int nevent_ksreco = tree_ksreco->GetEntries();
  for (int i=0; i<nevent_ksreco; i++)
  {
    tree_ksreco->GetEntry(i);
    if ((int)eventNumber_ksreco!=M_eventNumber) continue;
    if ( !((int)id1==M_trackid1 && (int)id2==M_trackid2) && !((int)id1==M_trackid2 && (int)id2==M_trackid1) ) continue;
    svx_ksreco = (projected_pca_rel1_x+projected_pca_rel2_x)/2;
    svy_ksreco = (projected_pca_rel1_y+projected_pca_rel2_y)/2;
    svz_ksreco = (projected_pca_rel1_z+projected_pca_rel2_z)/2;
    pvx_ksreco = vertex_x;
    pvy_ksreco = vertex_y;
    pvz_ksreco = vertex_z;
    dira_ksreco = cosThetaReco;
    dL_ksreco = sqrt(pow(projected_pathlength_x,2)+pow(projected_pathlength_y,2)+pow(projected_pathlength_z,2));
    pairdca_ksreco = projected_pair_dca;
  }
  std::vector<float> vec_svx_ksreco; vec_svx_ksreco.clear();
  std::vector<float> vec_svy_ksreco; vec_svy_ksreco.clear();
  std::vector<float> vec_svz_ksreco; vec_svz_ksreco.clear();
  std::vector<float> vec_svr_ksreco; vec_svr_ksreco.clear();
  std::vector<float> vec_pvx_ksreco; vec_pvx_ksreco.clear();
  std::vector<float> vec_pvy_ksreco; vec_pvy_ksreco.clear();
  std::vector<float> vec_pvz_ksreco; vec_pvz_ksreco.clear();
  std::vector<float> vec_pvr_ksreco; vec_pvr_ksreco.clear();
  vec_svx_ksreco.push_back(svx_ksreco);
  vec_svy_ksreco.push_back(svy_ksreco);
  vec_svz_ksreco.push_back(svz_ksreco);
  vec_svr_ksreco.push_back(sqrt(pow(svx_ksreco,2)+pow(svy_ksreco,2)));
  vec_pvx_ksreco.push_back(pvx_ksreco);
  vec_pvy_ksreco.push_back(pvy_ksreco);
  vec_pvz_ksreco.push_back(pvz_ksreco);
  vec_pvr_ksreco.push_back(sqrt(pow(pvx_ksreco,2)+pow(pvy_ksreco,2)));

  TFile* file_kfp = new TFile("../clusters_seeds_53744-0-0.root_ks_kfp.root");
  TTree* tree_kfp = (TTree*) file_kfp->Get("DecayTree");
  Int_t eventNumber_kfp, track_1_track_ID, track_2_track_ID;
  Float_t K_S0_SV_x, K_S0_SV_y, K_S0_SV_z;
  Float_t primary_vertex_x, primary_vertex_y, primary_vertex_z;
  Float_t K_S0_DIRA, K_S0_decayLength, track_1_track_2_DCA;
  tree_kfp->SetBranchAddress("eventNumber",&eventNumber_kfp);
  tree_kfp->SetBranchAddress("track_1_track_ID",&track_1_track_ID);
  tree_kfp->SetBranchAddress("track_2_track_ID",&track_2_track_ID);
  tree_kfp->SetBranchAddress("K_S0_SV_x",&K_S0_SV_x);
  tree_kfp->SetBranchAddress("K_S0_SV_y",&K_S0_SV_y);
  tree_kfp->SetBranchAddress("K_S0_SV_z",&K_S0_SV_z);
  tree_kfp->SetBranchAddress("primary_vertex_x",&primary_vertex_x);
  tree_kfp->SetBranchAddress("primary_vertex_y",&primary_vertex_y);
  tree_kfp->SetBranchAddress("primary_vertex_z",&primary_vertex_z);
  tree_kfp->SetBranchAddress("K_S0_DIRA",&K_S0_DIRA);
  tree_kfp->SetBranchAddress("K_S0_decayLength",&K_S0_decayLength);
  tree_kfp->SetBranchAddress("track_1_track_2_DCA",&track_1_track_2_DCA);

  float svx_kfp, svy_kfp, svz_kfp;
  float pvx_kfp, pvy_kfp, pvz_kfp;
  float dira_kfp, dL_kfp, pairdca_kfp;
  int nevent_kfp = tree_kfp->GetEntries();
  for (int i=0; i<nevent_kfp; i++)
  {
    tree_kfp->GetEntry(i);
    if (eventNumber_kfp!=M_eventNumber) continue;
    if ( !(track_1_track_ID==M_trackid1 && track_2_track_ID==M_trackid2) && !(track_1_track_ID==M_trackid2 && track_2_track_ID==M_trackid1) ) continue;
    svx_kfp = K_S0_SV_x;
    svy_kfp = K_S0_SV_y;
    svz_kfp = K_S0_SV_z;
    pvx_kfp = primary_vertex_x;
    pvy_kfp = primary_vertex_y;
    pvz_kfp = primary_vertex_z;
    dira_kfp = K_S0_DIRA;
    dL_kfp = K_S0_decayLength;
    pairdca_kfp = track_1_track_2_DCA;
  }
  std::vector<float> vec_svx_kfp; vec_svx_kfp.clear();
  std::vector<float> vec_svy_kfp; vec_svy_kfp.clear();
  std::vector<float> vec_svz_kfp; vec_svz_kfp.clear();
  std::vector<float> vec_svr_kfp; vec_svr_kfp.clear();
  std::vector<float> vec_pvx_kfp; vec_pvx_kfp.clear();
  std::vector<float> vec_pvy_kfp; vec_pvy_kfp.clear();
  std::vector<float> vec_pvz_kfp; vec_pvz_kfp.clear();
  std::vector<float> vec_pvr_kfp; vec_pvr_kfp.clear();
  vec_svx_kfp.push_back(svx_kfp);
  vec_svy_kfp.push_back(svy_kfp);
  vec_svz_kfp.push_back(svz_kfp);
  vec_svr_kfp.push_back(sqrt(pow(svx_kfp,2)+pow(svy_kfp,2)));
  vec_pvx_kfp.push_back(pvx_kfp);
  vec_pvy_kfp.push_back(pvy_kfp);
  vec_pvz_kfp.push_back(pvz_kfp);
  vec_pvr_kfp.push_back(sqrt(pow(pvx_kfp,2)+pow(pvy_kfp,2)));

  TFile* file_resid = new TFile("../clusters_seeds_53744-0-0.root_resid.root");
  TTree* tree_resid = (TTree*) file_resid->Get("residualtree");
  Int_t eventNumber_resid, trackid;
  std::vector<float> *clusgx=0, *clusgy=0, *clusgz=0;
  tree_resid->SetBranchAddress("eventNumber",&eventNumber_resid);
  tree_resid->SetBranchAddress("trackid",&trackid);
  tree_resid->SetBranchAddress("clusgx",&clusgx);
  tree_resid->SetBranchAddress("clusgy",&clusgy);
  tree_resid->SetBranchAddress("clusgz",&clusgz);

  std::vector<float> vec_clus_gx_track1; vec_clus_gx_track1.clear();
  std::vector<float> vec_clus_gy_track1; vec_clus_gy_track1.clear();
  std::vector<float> vec_clus_gz_track1; vec_clus_gz_track1.clear();
  std::vector<float> vec_clus_gr_track1; vec_clus_gr_track1.clear();
  std::vector<float> vec_clus_gx_track2; vec_clus_gx_track2.clear();
  std::vector<float> vec_clus_gy_track2; vec_clus_gy_track2.clear();
  std::vector<float> vec_clus_gz_track2; vec_clus_gz_track2.clear();
  std::vector<float> vec_clus_gr_track2; vec_clus_gr_track2.clear();

  int nevent_resid = tree_resid->GetEntries();
  for (int i=0; i<nevent_resid; i++)
  {
    tree_resid->GetEntry(i);
    if (eventNumber_resid!=M_eventNumber) continue;
    if (trackid==M_trackid1)
    {
      for (int j=0; j<(clusgx->size()); j++)
      {
        vec_clus_gx_track1.push_back(clusgx->at(j));
        vec_clus_gy_track1.push_back(clusgy->at(j));
        vec_clus_gz_track1.push_back(clusgz->at(j));
        vec_clus_gr_track1.push_back( sqrt(pow(clusgx->at(j),2)+pow(clusgy->at(j),2)) );
      }
    }
    else if (trackid==M_trackid2)
    {
      for (int j=0; j<(clusgx->size()); j++)
      {
        vec_clus_gx_track2.push_back(clusgx->at(j));
        vec_clus_gy_track2.push_back(clusgy->at(j));
        vec_clus_gz_track2.push_back(clusgz->at(j));
        vec_clus_gr_track2.push_back( sqrt(pow(clusgx->at(j),2)+pow(clusgy->at(j),2)) );
      }   
    }
  }

  TCanvas *can_xy = new TCanvas(Form("can_xy"), "can_xy", 800, 800);
  can_xy->SetLeftMargin(0.15);
  can_xy->SetRightMargin(0.05);
  can_xy->cd();

  TGraph *gr_clus_track1_xy = new TGraph(vec_clus_gx_track1.size(), vec_clus_gx_track1.data(), vec_clus_gy_track1.data());
  gr_clus_track1_xy->SetTitle(Form("Run %d Event %d TrackID %d&%d;X [cm];Y [cm]",M_runNumber,M_eventNumber,M_trackid1,M_trackid2));
  gr_clus_track1_xy->GetXaxis()->SetLimits(-10, 10);
  gr_clus_track1_xy->SetMinimum(-10);
  gr_clus_track1_xy->SetMaximum(10);
  gr_clus_track1_xy->SetMarkerSize(0.5);
  gr_clus_track1_xy->SetMarkerStyle(20);
  gr_clus_track1_xy->SetMarkerColor(kBlack);
  gr_clus_track1_xy->Draw("AP");

  TGraph *gr_clus_track2_xy = new TGraph(vec_clus_gx_track2.size(), vec_clus_gx_track2.data(), vec_clus_gy_track2.data());
  gr_clus_track2_xy->SetMarkerSize(0.5);
  gr_clus_track2_xy->SetMarkerStyle(20);
  gr_clus_track2_xy->SetMarkerColor(kBlack);
  gr_clus_track2_xy->Draw("P,same");

  TGraph *gr_sv_ksreco_xy = new TGraph(vec_svx_ksreco.size(), vec_svx_ksreco.data(), vec_svy_ksreco.data());
  gr_sv_ksreco_xy->SetMarkerSize(1.0);
  gr_sv_ksreco_xy->SetMarkerColor(kBlue);
  gr_sv_ksreco_xy->Draw("P*,same");

  TGraph *gr_sv_kfp_xy = new TGraph(vec_svx_kfp.size(), vec_svx_kfp.data(), vec_svy_kfp.data());
  gr_sv_kfp_xy->SetMarkerSize(1.0);
  gr_sv_kfp_xy->SetMarkerColor(kRed);
  gr_sv_kfp_xy->Draw("P*,same");

  TGraph *gr_pv_ksreco_xy = new TGraph(vec_pvx_ksreco.size(), vec_pvx_ksreco.data(), vec_pvy_ksreco.data());
  gr_pv_ksreco_xy->SetMarkerSize(1.0);
  gr_pv_ksreco_xy->SetMarkerColor(kBlue);
  gr_pv_ksreco_xy->SetMarkerStyle(22);
  gr_pv_ksreco_xy->Draw("P,same");

  TGraph *gr_pv_kfp_xy = new TGraph(vec_pvx_kfp.size(), vec_pvx_kfp.data(), vec_pvy_kfp.data());
  gr_pv_kfp_xy->SetMarkerSize(1.0);
  gr_pv_kfp_xy->SetMarkerColor(kRed);
  gr_pv_kfp_xy->SetMarkerStyle(22);
  gr_pv_kfp_xy->Draw("P,same");

  TLegend *legend_xy = new TLegend(0.2, 0.7, 0.6, 0.9);
  legend_xy->SetTextSize(0.04);
  legend_xy->AddEntry(gr_clus_track1_xy, "Clusters on track", "p");
  legend_xy->AddEntry(gr_sv_ksreco_xy, "KsReco SV", "p");
  legend_xy->AddEntry(gr_sv_kfp_xy, "KFP SV", "p");
  legend_xy->AddEntry(gr_pv_ksreco_xy, "KsReco PV", "p");
  legend_xy->AddEntry(gr_pv_kfp_xy, "KFP PV", "p");
  legend_xy->Draw("same");

  TPaveText *pt = new TPaveText(0.6,0.7,0.9,0.9, "NDC");
  pt->SetFillColor(0);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);
  pt->AddText(Form("DIRA: %.3f (KFP); %.3f (KsReco)",dira_kfp,dira_ksreco));
  pt->AddText(Form("DL: %.3f (KFP); %.3f (KsReco)",dL_kfp,dL_ksreco));
  pt->AddText(Form("PairDCA: %.3f (KFP); %.3f (KsReco)",pairdca_kfp,pairdca_ksreco));
  pt->Draw("same");

  can_xy->Update();
  can_xy->SaveAs(Form("figure/EvtDisplay_run%d_event%d_trkid_%d_%d_xy_zoomin.pdf",M_runNumber,M_eventNumber,M_trackid1,M_trackid2));




  TCanvas *can_rz = new TCanvas(Form("can_rz"), "can_rz", 800, 800);
  can_rz->SetLeftMargin(0.15);
  can_rz->SetRightMargin(0.05);
  can_rz->cd();

  TGraph *gr_clus_track1_rz = new TGraph(vec_clus_gz_track1.size(), vec_clus_gz_track1.data(), vec_clus_gr_track1.data());
  gr_clus_track1_rz->SetTitle(Form("Run %d Event %d TrackID %d&%d;Z [cm];R [cm]",M_runNumber,M_eventNumber,M_trackid1,M_trackid2));
  gr_clus_track1_rz->GetXaxis()->SetLimits(-20, 20);
  gr_clus_track1_rz->SetMinimum(0);
  gr_clus_track1_rz->SetMaximum(10);
  gr_clus_track1_rz->SetMarkerSize(0.5);
  gr_clus_track1_rz->SetMarkerStyle(20);
  gr_clus_track1_rz->SetMarkerColor(kBlack);
  gr_clus_track1_rz->Draw("AP");

  TGraph *gr_clus_track2_rz = new TGraph(vec_clus_gz_track2.size(), vec_clus_gz_track2.data(), vec_clus_gr_track2.data());
  gr_clus_track2_rz->SetMarkerSize(0.5);
  gr_clus_track2_rz->SetMarkerStyle(20);
  gr_clus_track2_rz->SetMarkerColor(kBlack);
  gr_clus_track2_rz->Draw("P,same");

  TGraph *gr_sv_ksreco_rz = new TGraph(vec_svz_ksreco.size(), vec_svz_ksreco.data(), vec_svr_ksreco.data());
  gr_sv_ksreco_rz->SetMarkerSize(1.0);
  gr_sv_ksreco_rz->SetMarkerColor(kBlue);
  gr_sv_ksreco_rz->Draw("P*,same");

  TGraph *gr_sv_kfp_rz = new TGraph(vec_svz_kfp.size(), vec_svz_kfp.data(), vec_svr_kfp.data());
  gr_sv_kfp_rz->SetMarkerSize(1.0);
  gr_sv_kfp_rz->SetMarkerColor(kRed);
  gr_sv_kfp_rz->Draw("P*,same");

  TGraph *gr_pv_ksreco_rz = new TGraph(vec_pvz_ksreco.size(), vec_pvz_ksreco.data(), vec_pvr_ksreco.data());
  gr_pv_ksreco_rz->SetMarkerSize(1.0);
  gr_pv_ksreco_rz->SetMarkerColor(kBlue);
  gr_pv_ksreco_rz->SetMarkerStyle(22);
  gr_pv_ksreco_rz->Draw("P,same");

  TGraph *gr_pv_kfp_rz = new TGraph(vec_pvz_kfp.size(), vec_pvz_kfp.data(), vec_pvr_kfp.data());
  gr_pv_kfp_rz->SetMarkerSize(1.0);
  gr_pv_kfp_rz->SetMarkerColor(kRed);
  gr_pv_kfp_rz->SetMarkerStyle(22);
  gr_pv_kfp_rz->Draw("P,same");

  TLegend *legend_rz = new TLegend(0.2, 0.7, 0.6, 0.9);
  legend_rz->SetTextSize(0.04);
  legend_rz->AddEntry(gr_clus_track1_rz, "Clusters on track", "p");
  legend_rz->AddEntry(gr_sv_ksreco_rz, "KsReco SV", "p");
  legend_rz->AddEntry(gr_sv_kfp_rz, "KFP SV", "p");
  legend_rz->AddEntry(gr_pv_ksreco_rz, "KsReco PV", "p");
  legend_rz->AddEntry(gr_pv_kfp_rz, "KFP PV", "p");
  legend_rz->Draw("same");

  pt->Draw("same");

  can_rz->Update();
  can_rz->SaveAs(Form("figure/EvtDisplay_run%d_event%d_trkid_%d_%d_rz_zoomin.pdf",M_runNumber,M_eventNumber,M_trackid1,M_trackid2));

}
