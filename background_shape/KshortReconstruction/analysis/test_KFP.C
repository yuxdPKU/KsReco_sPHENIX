#include <TFile.h>
#include <TNtuple.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TF1.h>
#include <TChain.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLine.h>
#include <iostream>
#include <fstream>
#include <string>

void test_KFP()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

  //TCut trcut("track_1_MVTX_nHits>0 && track_1_INTT_nHits>0 && track_2_MVTX_nHits>0 && track_2_INTT_nHits>0 && track_1_IP_xy<0.05 && track_2_IP_xy<0.05 && track_1_pT>0.2 && track_2_pT>0.2 && K_S0_DIRA>0.88 && pow(K_S0_decayLength,2)>0.2 && track_1_track_2_DCA<0.5");
  //TCut trcut("track_1_MVTX_nHits>0 && track_1_INTT_nHits>0 && track_2_MVTX_nHits>0 && track_2_INTT_nHits>0 && track_1_pT>0.2 && track_2_pT>0.2 && K_S0_DIRA>0.88 && pow(K_S0_decayLength,2)>0.2 && track_1_track_2_DCA<0.5");
  TCut trcut("track_1_MVTX_nHits>0 && track_1_INTT_nHits>1 && track_2_MVTX_nHits>0 && track_2_INTT_nHits>1 && track_1_pT>0.2 && track_2_pT>0.2 && K_S0_DIRA>0.88 && pow(K_S0_decayLength,2)>0.2 && track_1_track_2_DCA<0.5");

  // likesign charge1==charge2
  // unlikesign charge1!=charge2
  TChain *ntp_likesign = new TChain("DecayTree");
  TChain *ntp_unlikesign = new TChain("DecayTree");

  //int run = 52911;
  int run = 53744;

  ntp_likesign->Add(Form("/sphenix/u/xyu3/workarea/PhotonConv/KsNew/background_shape/KshortReconstruction/likesign/all%d_kfp.root",run));
  ntp_unlikesign->Add(Form("/sphenix/u/xyu3/workarea/PhotonConv/KsNew/background_shape/KshortReconstruction/unlikesign/all%d_kfp.root",run));

  TCanvas *c2 = new TCanvas("c2","mass",50,50,400,600);
  c2->Divide(1,2);

  c2->cd(1);
 
  TH1D *hm = new TH1D("hm","invariant mass",60,0.4, 0.6);
  ntp_unlikesign->Draw("K_S0_mass>>hm",trcut);
 
  int binlo = hm->FindBin(0.43);
  int binhi = hm->FindBin(0.54);

  TH1D *hmbg = new TH1D("hmbg","mass background",60,0.4, 0.6);
  ntp_likesign->Draw("K_S0_mass>>hmbg", trcut);

  std::cout <<  "fg " <<  hm->Integral(binlo, binhi) << " bgd " << hmbg->Integral(binlo, binhi) << " net signal " <<  hm->Integral(binlo, binhi) -  hmbg->Integral(binlo, binhi) << std::endl;

  hm->GetXaxis()->SetTitle("invariant mass (GeV)");
  hmbg->SetLineColor(kRed);
  hmbg->SetLineWidth(2.0);
  hm->SetLineWidth(2.0);

  hm->SetMaximum(hm->GetMaximum() > hmbg->GetMaximum() ? hm->GetMaximum() : hmbg->GetMaximum());
  hm->SetMinimum(hm->GetMinimum() < hmbg->GetMinimum() ? hm->GetMinimum() : hmbg->GetMinimum());

  //hm->Scale(hmbg->Integral()/hm->Integral());
 
  hm->DrawCopy();  
  hmbg->DrawCopy("same");  

  TLegend *leg = new TLegend(0.55, 0.7, 0.85, 0.85, "", "NDC");
  leg->AddEntry(hm,"Unlike sign","l");
  leg->AddEntry(hmbg,"Like sign","l");
  leg->Draw();
 
  c2->cd(2);
 
  hm->Sumw2();
  hmbg->Sumw2();
  hm->Add(hmbg,-1);
  hm->SetLineColor(kBlack);
  hm->SetMarkerStyle(20);
  hm->SetMarkerSize(0.3);
  hm->SetLineWidth(1.0);
  hm->SetTitle("net invariant mass");
  hm->DrawCopy("p");

  TF1 *f = new TF1("f","[0]+gaus(1)",0.4,0.6);
  f->SetParameters(0.0, 100.0, 0.49, 0.1);
  hm->Fit("f","R");
  f->Draw("same");

  TF1 * fg = new TF1("fg","gaus(0)", 0.4, 0.6);
  fg->SetParameters(f->GetParameter(1), f->GetParameter(2), f->GetParameter(3));
  // multiply value/bin by bins/GeV to  get counts from function integral
  std::cout << " fit signal integral " << fg->Integral(0.43, 0.54) * (60 / 0.2) << std::endl;

  TF1 * fe = new TF1("fe","[0]", 0.4, 0.6);
  fe->SetParameter(0, f->GetParameter(0));
  std::cout << " fit background integral " << fe->Integral(0.43, 0.54) * (60/ 0.2) << std::endl;

  TLine* one = new TLine(0.4, 0.0, 0.6, 0.0);
  one->Draw();

  c2->Update();
  c2->SaveAs("figure/plot2_mass_KFP.pdf");

}
