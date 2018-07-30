#include <iostream> 
#include <vector> 
#include <stdlib.h>
#include <utility> 

#include <boost/optional.hpp> 
#include <TFile.h> 
#include <TChain.h> 
#include <TH1.h> 
#include <TLorentzVector.h> 
#include <TTree.h> 

using namespace std; 
bool sort_pt (TLorentzVector i, TLorentzVector j){return (i.Pt()<j.Pt());}

int main(){
  TFile *in = TFile::Open("/msu/data/t3work2/ivanp/jahred_tiny/data15.root"); 
  TFile *out = TFile::Open("/msu/data/t3work9/voetberg/tmva_input/17feat_realdata.root", "RECREATE"); 
  cout<<"Opened files"<<endl; 
  //===============================================
  TChain* chain = (TChain*)in->Get("mini"); 
  //===============================================
  //Variables 
  constexpr size_t max=10; 
  Float_t m_yy, dy_yy; 
  Int_t photon_n, jet_n; 
  Float_t jet_pt[max], jet_eta[max], jet_phi[max], jet_m[max]; 
  Float_t photon_pt[max], photon_eta[max], photon_phi[max], photon_m[max]; 
  //==============================================
  //Chains
  chain->SetBranchAddress("m_yy", &m_yy); 
  chain->SetBranchAddress("dy_yy", &dy_yy); 

  chain->SetBranchAddress("photon_n", &photon_n); 
  chain->SetBranchAddress("photon_pt", photon_pt); 
  chain->SetBranchAddress("photon_eta", photon_eta); 
  chain->SetBranchAddress("photon_phi", photon_phi); 
  chain->SetBranchAddress("photon_m", photon_m); 

  chain->SetBranchAddress("jet_n", &jet_n); 
  chain->SetBranchAddress("jet_pt", jet_pt); 
  chain->SetBranchAddress("jet_eta", jet_eta); 
  chain->SetBranchAddress("jet_phi", jet_phi); 
  chain->SetBranchAddress("jet_m", jet_m); 
  
  cout<<"Set Variables"<<endl; 
  //==============================================
  //Tree
  Float_t s, yydr, y1dr, y2dr, ptratio, y1y, y2y, jety, yydy; 
  Float_t y1E, y1pt, y2E, y2pt, yyE, yypt, jetE, jetpt; 

  TTree* tree = new TTree("tree", "tree"); 
  tree->SetDirectory(out); 

  tree->Branch("s", &s);                                                       
  tree->Branch("yydr", &yydr);                                                 
  tree->Branch("y1dr", &y1dr);                                                 
  tree->Branch("y2dr", &y2dr);                                                 
  tree->Branch("ptratio", &ptratio);                                           
  tree->Branch("y1y", &y1y);                                                   
  tree->Branch("y2y", &y2y);                                                   
  tree->Branch("jety", &jety);                                                 
  tree->Branch("yydy", &yydy);                                                 
  tree->Branch("y1E", &y1E);                                                   
  tree->Branch("y1pt", &y1pt);                                                 
  tree->Branch("y2E", &y2E);                                                   
  tree->Branch("y2pt", &y2pt);                                                 
  tree->Branch("yyE", &yyE);                                                   
  tree->Branch("yypt", &yypt);                                                 
  tree->Branch("jetE", &jetE);                                                 
  tree->Branch("jetpt", &jetpt);

  cout<<"Set Branches"<<endl;
  //==============================================
  const Long64_t entries = chain->GetEntries(); 
  //==============================================
  for (long j=0; j<entries; ++j){
    chain->GetEntry(j); 
    TLorentzVector yy, y1, y2, jet, t_jet, t_y; 
    vector<TLorentzVector> jets, photons; 

 //   if (photon_pt[0]>photon_pt[1]){
 //     y1.SetPtEtaPhiM(photon_pt[0],photon_eta[0], photon_phi[0], photon_m[0]); 
 //     y2.SetPtEtaPhiM(photon_pt[1],photon_eta[1], photon_phi[1], photon_m[1]);
 //   }
 //   else{
 //     y2.SetPtEtaPhiM(photon_pt[0],photon_eta[0], photon_phi[0], photon_m[0]); 
 //     y1.SetPtEtaPhiM(photon_pt[1],photon_eta[1], photon_phi[1], photon_m[1]);
 //   }

    for (int i=0; i<photon_n; ++i){
      t_y.SetPtEtaPhiM(photon_pt[i],photon_eta[i], photon_phi[i], photon_m[i]); 
      photons.emplace_back(t_y); 
    } 

    if (jet_n !=0){
      for (int i=0; i<jet_n; ++i){  
        t_jet.SetPtEtaPhiM(jet_pt[i], jet_eta[i], jet_phi[i], jet_m[i]); 
        jets.emplace_back(t_jet);
      }
      sort(jets.begin(), jets.end(), sort_pt); 
      jet=jets[0];   
    } 
    else{
      jet.SetPtEtaPhiM(jet_pt[0], jet_eta[0], jet_phi[0], jet_m[0]);
    }
    

    //Need to sort
    sort(photons.begin(), photons.end(), sort_pt); 

    y1=photons[0]; 
    y2=photons[1];
    yy=y1+y2;  


    //Mass Cut
    //Trying for no signal events    
    bool select = !((m_yy>121.)&&(m_yy<129.));                         
    //Rapidity Cut                                                              
    select &= (abs(y2.Rapidity())<2.4);                                    
    select &= (abs(y1.Rapidity())<2.4);   
    //PseudoRapidity Cut                                                        
    select &= (abs(y1.PseudoRapidity())<2.37);                             
    select &= !(1.37<abs(y1.PseudoRapidity()) && abs(y1.PseudoRapidity())<1.52);
    select &= (abs(y2.PseudoRapidity())<2.37);                             
    select &= !(1.37<abs(y2.PseudoRapidity()) && abs(y2.PseudoRapidity())<1.52);
    //Pt Cut                                                                    
    select &= (y1.Pt()>.35*yy.M());                                     
    select &= (y2.Pt()>.25*yy.M());                                     
    //Delta R                                                                   
    select &= (y2.DeltaR(jet)>.4); 
    //Jet Cuts
    for (int i=0; i<jet_n; ++i ){ 
      select &= (jets[i].Pt()>30); 
      select &= (jets[i].Rapidity()<4.4); 
    }  

    if (select){
      s = (yy+jet).M(); 
      yydr = abs(yy.DeltaR(jet)); 
      y1dr = abs(y1.DeltaR(jet)); 
      y2dr = abs(y2.DeltaR(jet));
      ptratio = abs(y1.Pt())/abs(y2.Pt()); 
      y1y = abs(y1.Rapidity());
      y2y = abs(y2.Rapidity());
      jety = abs(jet.Rapidity());
      yydy = dy_yy; 

      yyE = abs(yy.E()); 
      y1E = abs(y1.E());
      y2E = abs(y2.E());
      jetE = abs(jet.E());
    
      yypt = abs(yy.Pt()); 
      y1pt = abs(y1.Pt());
      y2pt = abs(y2.Pt());
      jetpt = abs(jet.Pt());
    
      tree->Fill(); 
    }
  }
  out->Write(); 
  out->Close(); 
}
