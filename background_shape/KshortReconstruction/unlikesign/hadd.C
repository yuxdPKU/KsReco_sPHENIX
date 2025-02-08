void hadd()
{

  int run=53744;
  TChain chain("DecayTree");
  chain.Add(Form("./root/*%d*kfp.root",run));

  int compressionLevel=9;
  TFile output(Form("all%d_kfp_highcompress.root",run), "RECREATE", "", compressionLevel);

  chain.CloneTree(-1, "fast");

  output.Write();
  output.Close();

  std::cout << "Done!" << std::endl;
}
