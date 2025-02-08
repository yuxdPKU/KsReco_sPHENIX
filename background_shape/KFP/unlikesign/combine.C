void combine()
{

    TChain* chain = new TChain("DecayTree");
    chain->Add("./root/*52911*.root");

    TFile* file = new TFile("all52911.root","recreate");
    TTree* tree = chain->CopyTree("","",100000000);
    file->cd();
    tree->Write();
    file->Close();

}
