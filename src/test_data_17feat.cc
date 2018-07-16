#include <iostream>                                                             
#include <vector>                                                               
#include <stdlib.h>                                                             
#include <utility>                                                              
#include <chrono>                                                               
                                                                                
#include <boost/optional.hpp>                                                   
#include <TFile.h>                                                              
#include <TChain.h>                                                             
#include <TH1.h>                                                                
#include <TLorentzVector.h>                                                     
#include <TTree.h>                                                              
#include <TRandom3.h>                                                           
                                                                                
#include "Higgs2diphoton.hh"                                                    
                                                                                
using namespace std;                                                            
                                                                                
Higgs2diphoton Hdecay;                                                          
                                                                                
int main(){                                                                     
  TFile *sig = TFile::Open("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_126.root");
  TFile *bg = TFile::Open("$TMPDIR/born4.root");                                
  //===============================================                             
  TFile* out = TFile::Open("17feat_pt21_24.root", "RECREATE");                     
                                                                                
  cout<<"Files opened"<<endl;                                                   
  //===============================================                             
  TChain* chain_b = (TChain*)bg->Get("t3");                                     
  TChain* chain_s = (TChain*)sig->Get("t3");                                    
   //===============================================                            
  constexpr size_t max = 10;                                                    
  Float_t px_s[max], py_s[max], pz_s[max], E_s[max];                            
  Int_t nparticle_s, kf_s[max];                                                 
  Double_t weight_s;                                                            
                                                                                
  Float_t px_b[max], py_b[max], pz_b[max], E_b[max];                            
  Int_t nparticle_b, kf_b[max];                                                 
  Double_t weight_b;
  //===============================================                             
  chain_s->SetBranchAddress("nparticle",&nparticle_s);                          
  chain_s->SetBranchAddress("kf",kf_s);                                         
  chain_s->SetBranchAddress("px",px_s);                                         
  chain_s->SetBranchAddress("py",py_s);                                         
  chain_s->SetBranchAddress("pz",pz_s);                                         
  chain_s->SetBranchAddress("E",E_s);                                           
  chain_s->SetBranchAddress("weight2",&weight_s);                               
                                                                                
  chain_b->SetBranchAddress("nparticle",&nparticle_b);                          
  chain_b->SetBranchAddress("kf",kf_b);                                         
  chain_b->SetBranchAddress("px",px_b);                                         
  chain_b->SetBranchAddress("py",py_b);                                         
  chain_b->SetBranchAddress("pz",pz_b);                                         
  chain_b->SetBranchAddress("E",E_b);                                           
  chain_b->SetBranchAddress("weight2",&weight_b);                               
                                                                                
                                                                                
  cout<<"Chains set"<<endl;                                                     
  //==============================================
  //ALL THE TREES                                                               
  Float_t s, yydr, y1dr, y2dr, ptratio, y1y, y2y,jety, yydy;                    
  Float_t y1E, y1pt, y2E, y2pt, yyE, yypt, jetE, jetpt;                         
  Float_t weights;                                                              
  //===============================================                             
  TTree* sig_d = new TTree("sig", "signal");                           
  TTree* bg_d = new TTree("bg", "background");                         
  sig_d->SetDirectory(out);                                                 
  bg_d->SetDirectory(out);                                                  
                                                                                
  //===============================================                             
  //Ouputs                                                                      
  //Signal                                                                      
  sig_d->Branch("s", &s);                                                   
  sig_d->Branch("yydr", &yydr);                                             
  sig_d->Branch("y1dr", &y1dr);                                             
  sig_d->Branch("y2dr", &y2dr);                                             
  sig_d->Branch("ptratio", &ptratio);                                       
  sig_d->Branch("y1y", &y1y);                                               
  sig_d->Branch("y2y", &y2y);                                               
  sig_d->Branch("jety", &jety);                                             
  sig_d->Branch("yydy", &yydy);                                             
  sig_d->Branch("y1E", &y1E);                                               
  sig_d->Branch("y1pt", &y1pt);                                             
  sig_d->Branch("y2E", &y2E);                                               
  sig_d->Branch("y2pt", &y2pt);                                             
  sig_d->Branch("yyE", &yyE);                                               
  sig_d->Branch("yypt", &yypt);                                             
  sig_d->Branch("jetE", &jetE);                                             
  sig_d->Branch("jetpt", &jetpt);                                           
                                                                                
  sig_d->Branch("weights", &weights); 
  //Background
  bg_d->Branch("s", &s);                                                   
  bg_d->Branch("yydr", &yydr);                                             
  bg_d->Branch("y1dr", &y1dr);                                             
  bg_d->Branch("y2dr", &y2dr);                                             
  bg_d->Branch("ptratio", &ptratio);                                       
  bg_d->Branch("y1y", &y1y);                                               
  bg_d->Branch("y2y", &y2y);                                               
  bg_d->Branch("jety", &jety);                                             
  bg_d->Branch("yydy", &yydy);                                             
  bg_d->Branch("y1E", &y1E);                                               
  bg_d->Branch("y1pt", &y1pt);                                             
  bg_d->Branch("y2E", &y2E);                                               
  bg_d->Branch("y2pt", &y2pt);                                             
  bg_d->Branch("yyE", &yyE);                                               
  bg_d->Branch("yypt", &yypt);                                             
  bg_d->Branch("jetE", &jetE);                                             
  bg_d->Branch("jetpt", &jetpt);                                           
                                                                                
  bg_d->Branch("weights", &weights);
 //==============================================
  const Long64_t entries_s = chain_s->GetEntries(); 
  const Long64_t entries_b = chain_b->GetEntries();
 //=============================================
 //Signal                                                                      
  for (long j=0; j<entries_s; ++j){                                             
                                                                                
    chain_s->GetEntry(j);                                                       
    TLorentzVector higgs, photon1, photon2, jet;                                
    vector<TLorentzVector> jets;                                                
    for (long i=0; i<nparticle_s; ++i){                                         
      if (kf_s[i]==25){                                                         
        higgs.SetPxPyPzE(px_s[i],py_s[i],pz_s[i],E_s[i]);                       
      }                                                                         
      else{                                                                     
        jets.emplace_back(px_s[i],py_s[i],pz_s[i],E_s[i]);                      
      }                                                                         
    }                                                                           
                                                                                
    //Decay higgs                                                               
    pair<TLorentzVector, TLorentzVector> diphoton = Hdecay(higgs);              
                                                                                
    //Sort Photons                                                                            
    if (diphoton.first.Pt()>diphoton.second.Pt()){                              
      photon1 = diphoton.first;                                                 
      photon2 = diphoton.second;                                                
    }                                                                           
    else{                                                                       
      photon1 = diphoton.second;                                                
      photon2 = diphoton.first;                                                 
    }                                                                           
                                                                                
    jet=jets[0]; 
  
    //Mass Cut                                                                  
    bool select = ((higgs.M()>121.)&&(higgs.M()<124.));                         
    //Rapidity Cut                                                              
    select &= (abs(photon2.Rapidity())<2.4);                                    
    select &= (abs(photon1.Rapidity())<2.4);                                    
    //PseudoRapidity Cut                                                        
    select &= (abs(photon1.PseudoRapidity())<2.37);                             
    select &= !(1.37<abs(photon1.PseudoRapidity()) && abs(photon1.PseudoRapidity())<1.52);
    select &= (abs(photon2.PseudoRapidity())<2.37);                             
    select &= !(1.37<abs(photon2.PseudoRapidity()) && abs(photon2.PseudoRapidity())<1.52);
    //Pt Cut                                                                    
    select &= (photon1.Pt()>.35*higgs.M());                                     
    select &= (photon2.Pt()>.25*higgs.M());                                     
    //Delta R                                                                   
    select &= (photon2.DeltaR(jet)>.4);                                         
                                                                                
    if (select){                                                                
      s = (abs((higgs+jet).M()));                                               
      yydr = (abs(higgs.DeltaR(jet)));                                          
      y1dr = (abs(photon1.DeltaR(jet)));                                        
      y2dr = (abs(photon2.DeltaR(jet)));                                        
      ptratio = (abs(photon1.Pt())/abs(photon2.Pt()));                          
      y1y = (abs(photon1.Rapidity()));                                          
      y2y = (abs(photon2.Rapidity()));                                          
      jety = (abs(jet.Rapidity()));                                             
      yydy = (abs(photon1.Rapidity()-photon2.Rapidity()));                      
                                                                                
      yyE = (abs(higgs.E()));                                                   
      y1E = (abs(photon1.E()));                                                 
      y2E = (abs(photon2.E()));                                                 
      jetE = (abs(jet.E()));                                                    
                                                                                
      yypt = (abs(higgs.Pt()));                                                 
      y1pt = abs(photon1.Pt());                                                 
      y2pt = abs(photon2.Pt());                                                 
      jetpt = abs(jet.Pt());                                                    
                                                                                
      weights = weight_s; 
      
      sig_d->Fill(); 
    }
  }
  //======================================================
  for (long k=0; k<entries_b; ++k){                                             
    chain_b->GetEntry(k);                                                       
                                                                             
    vector<TLorentzVector> photons;                                             
    TLorentzVector photon1, photon2, jet;                                       
    photons.clear();                                                            
                                                                                 
    //Assign particles                                                           
    for (long i=0; i<nparticle_b; ++i){                                          
      //if photon                                                                
      if (kf_b[i]== 22){                                                         
        photons.emplace_back(px_b[i], py_b[i], pz_b[i], E_b[i]);                 
      }                                                                          
      //if jet                                                                   
      else{                                                                      
        jet.SetPxPyPzE(px_b[i], py_b[i], pz_b[i], E_b[i]);                       
      }                                                                          
    }                                                                            
    //Setting photons                                                            
    //Sort by pt                                                                 
    if (photons[0].Pt()>photons[1].Pt()){                                        
      photon1 = photons[0];                                                      
      photon2 = photons[1];                                                      
    }                                                                            
    else{                                                                        
      photon1 = photons[1];                                                      
      photon2 = photons[0];                                                      
    }                                                                            
    TLorentzVector diphoton = photon1 + photon2;
    
    
    //Mass Cut                                                                   
    bool select = ((diphoton.M()>121.)&&(diphoton.M()<124.));                    
    //Rapidity Cut                                                               
    select &= (abs(photon2.Rapidity())<2.4);                                     
    select &= (abs(photon1.Rapidity())<2.4);                                     
    //PseudoRapidity Cut                                                         
    select &= (abs(photon1.PseudoRapidity())<2.37);                              
    select &= !(1.37<abs(photon1.PseudoRapidity()) && abs(photon1.PseudoRapidity())<1.52);
    select &= (abs(photon2.PseudoRapidity())<2.37);                              
    select &= !(1.37<abs(photon2.PseudoRapidity()) && abs(photon2.PseudoRapidity())<1.52);
    //Pt Cut                                                                     
    select &= (photon1.Pt()>.35*diphoton.M());                                   
    select &= (photon2.Pt()>.25*diphoton.M());                                   
    //Delta R                                                                    
    select &= (photon2.DeltaR(jet)>.4);
    
    if (select){                                                                 
      s = (abs((diphoton+jet).M()));                                             
      yydr = (abs(diphoton.DeltaR(jet)));                                        
      y1dr = (abs(photon1.DeltaR(jet)));                                         
      y2dr = (abs(photon2.DeltaR(jet)));                                         
      ptratio = (abs(photon1.Pt())/abs(photon2.Pt()));                           
      y1y = (abs(photon1.Rapidity()));                                           
      y2y = (abs(photon2.Rapidity()));                                           
      jety = (abs(jet.Rapidity()));                                              
      yydy = (abs(photon1.Rapidity()-photon2.Rapidity()));                       
                                                                                 
      yyE = (abs(diphoton.E()));                                                 
      y1E = (abs(photon1.E()));                                                  
      y2E = (abs(photon2.E()));                                                  
      jetE = (abs(jet.E()));                                                     
                                                                                 
      yypt = (abs(diphoton.Pt()));                                               
      y1pt = abs(photon1.Pt());                                                  
      y2pt = abs(photon2.Pt());                                                  
      jetpt = abs(jet.Pt());                                                     
                                                                                 
      weights = weight_s;
    
      bg_d->Fill();
    }
  }
  out->Write(); 
  out->Close(); 
}

