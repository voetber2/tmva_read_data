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
  TChain chain_b("t3"); 
  TChain chain_s("t3");
  TChain chain_d("mini"); 
  
  chain_b.Add("$TMPDIR/born1_1.root");
  chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_103.root"); 
  chain_d.Add("$TMPDIR/data15.root"); 

  //===============================================                            
  constexpr size_t max = 10;                                                    
  Float_t px_s[max], py_s[max], pz_s[max], E_s[max];                            
  Int_t nparticle_s, kf_s[max];                                                 
  Double_t weight_s;                                                            
                                                                                
  Float_t px_b[max], py_b[max], pz_b[max], E_b[max];                            
  Int_t nparticle_b, kf_b[max];                                                 
  Double_t weight_b;

  Float_t m_yy, dy_yy;
  Int_t photon_n, jet_n;
  Float_t jet_pt[max], jet_eta[max], jet_phi[max], jet_m[max];     
  Float_t photon_pt[max], photon_eta[max], photon_phi[max], photon_m[max]; 
  
  //===============================================
  //Signal mc 
  chain_s.SetBranchAddress("nparticle",&nparticle_s);                          
  chain_s.SetBranchAddress("kf",kf_s);                                         
  chain_s.SetBranchAddress("px",px_s);                                         
  chain_s.SetBranchAddress("py",py_s);                                         
  chain_s.SetBranchAddress("pz",pz_s);                                         
  chain_s.SetBranchAddress("E",E_s);                                           
  chain_s.SetBranchAddress("weight2",&weight_s);                               
              
  //Back mc   
  chain_b.SetBranchAddress("nparticle",&nparticle_b);                          
  chain_b.SetBranchAddress("kf",kf_b);                                         
  chain_b.SetBranchAddress("px",px_b);                                         
  chain_b.SetBranchAddress("py",py_b);                                         
  chain_b.SetBranchAddress("pz",pz_b);                                         
  chain_b.SetBranchAddress("E",E_b);                                           
  chain_b.SetBranchAddress("weight2",&weight_b);  
  
  //Data
  chain_d.SetBranchAddress("m_yy", &m_yy);                                       
  chain_d.SetBranchAddress("dy_yy", &dy_yy);                                     
                                                                                
  chain_d.SetBranchAddress("photon_n", &photon_n);                               
  chain_d.SetBranchAddress("photon_pt", photon_pt);                              
  chain_d.SetBranchAddress("photon_eta", photon_eta);                            
  chain_d.SetBranchAddress("photon_phi", photon_phi);                            
  chain_d.SetBranchAddress("photon_m", photon_m);                                
                                                                                
  chain_d.SetBranchAddress("jet_n", &jet_n);                                     
  chain_d.SetBranchAddress("jet_pt", jet_pt);                                    
  chain_d.SetBranchAddress("jet_eta", jet_eta);                                  
  chain_d.SetBranchAddress("jet_phi", jet_phi);                                  
  chain_d.SetBranchAddress("jet_m", jet_m); 

  vector<TString> num; 
  num.emplace_back("50");
  num.emplace_back("100");
  num.emplace_back("150");
  num.emplace_back("200");
  num.emplace_back("200p");

  for (int i=0; i<5; ++i){
    TString outname = "data16_pt" + num[i] + "_reader.root"; 
    TFile* out = TFile::Open(outname, "RECREATE"); 
    //==============================================                              
    //ALL THE TREES                                                               
    Float_t s, yydr, y1dr, y2dr, ptratio, y1y, y2y,jety, yydy;                    
    Float_t y1E, y1pt, y2E, y2pt, yyE, yypt, jetE;                         
    Float_t weights;                                                              
    //===============================================                             
    TTree* sig_tree = new TTree("sig", "signal mc");                           
    TTree* bg_tree = new TTree("bg", "bckgrnd mc");                             
    TTree* data_tree = new TTree("data", "real data");                             
  
    sig_tree->SetDirectory(out); 
    bg_tree->SetDirectory(out); 
    data_tree->SetDirectory(out); 
    
    //===============================================                             
    //Ouputs                                                                      
    //Signal                                                                      
    sig_tree->Branch("s", &s);            
    sig_tree->Branch("yydr", &yydr);      
    sig_tree->Branch("y1dr", &y1dr);      
    sig_tree->Branch("y2dr", &y2dr);      
    sig_tree->Branch("ptratio", &ptratio);
    sig_tree->Branch("y1y", &y1y);        
    sig_tree->Branch("y2y", &y2y);        
    sig_tree->Branch("jety", &jety);      
    sig_tree->Branch("yydy", &yydy);      
    sig_tree->Branch("y1E", &y1E);        
    sig_tree->Branch("y1pt", &y1pt);      
    sig_tree->Branch("y2E", &y2E);        
    sig_tree->Branch("y2pt", &y2pt);      
    sig_tree->Branch("yyE", &yyE);        
    sig_tree->Branch("yypt", &yypt);      
    sig_tree->Branch("jetE", &jetE);      
                                           
    sig_tree->Branch("weights", &weights);
    //Background                                     
    bg_tree->Branch("s", &s);             
    bg_tree->Branch("yydr", &yydr);       
    bg_tree->Branch("y1dr", &y1dr);       
    bg_tree->Branch("y2dr", &y2dr);       
    bg_tree->Branch("ptratio", &ptratio); 
    bg_tree->Branch("y1y", &y1y);         
    bg_tree->Branch("y2y", &y2y);         
    bg_tree->Branch("jety", &jety);       
    bg_tree->Branch("yydy", &yydy);       
    bg_tree->Branch("y1E", &y1E);         
    bg_tree->Branch("y1pt", &y1pt);       
    bg_tree->Branch("y2E", &y2E);         
    bg_tree->Branch("y2pt", &y2pt);       
    bg_tree->Branch("yyE", &yyE);         
    bg_tree->Branch("yypt", &yypt);       
    bg_tree->Branch("jetE", &jetE);       
                                           
    bg_tree->Branch("weights", &weights);                                    
    
    //Data
    data_tree->Branch("s", &s);             
    data_tree->Branch("yydr", &yydr);       
    data_tree->Branch("y1dr", &y1dr);       
    data_tree->Branch("y2dr", &y2dr);       
    data_tree->Branch("ptratio", &ptratio); 
    data_tree->Branch("y1y", &y1y);         
    data_tree->Branch("y2y", &y2y);         
    data_tree->Branch("jety", &jety);       
    data_tree->Branch("yydy", &yydy);       
    data_tree->Branch("y1E", &y1E);         
    data_tree->Branch("y1pt", &y1pt);       
    data_tree->Branch("y2E", &y2E);         
    data_tree->Branch("y2pt", &y2pt);       
    data_tree->Branch("yyE", &yyE);         
    data_tree->Branch("yypt", &yypt);       
    data_tree->Branch("jetE", &jetE);       
                                            
    //==============================================                              
    const Long64_t entries_s = chain_s.GetEntries();                             
    const Long64_t entries_b = chain_b.GetEntries();                             
    const Long64_t entries_d = chain_d.GetEntries();                             
    //===============================================
  
    //Signal                                                                      
    for (long k=0; k<entries_s; ++k){                                             
      chain_s.GetEntry(k);                                                       
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
      
      //Cuts
      //Mass Cut                                                                  
      bool select = ((higgs.M()>121.)&&(higgs.M()<129.));                         
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
      //Jet Cuts                            
      select &= (jet.Pt()>30);              
      select &= (jet.Rapidity()<4.4);       
     
  
        //Diphoton Pt cuts 
      if (i==0){select &= (higgs.Pt()<50.);}
      if (i==1){select &= (higgs.Pt()>50. && higgs.Pt()<100.);}
      if (i==2){select &= (higgs.Pt()>100. && higgs.Pt()<150);}
      if (i==3){select &= (higgs.Pt()>150. && higgs.Pt()<200);}
      if (i==4){select &= (higgs.Pt()>200.);}
      
      
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
                                                                                  
        weights = weight_s;                                                       
        //===============================================                         
        sig_tree->Fill(); 
      }       
    }
  
  
    //===================================================
    //Background 
    for (long k=0; k<entries_b; ++k){                                             
      chain_b.GetEntry(k);                                                       
                                                                                 
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
      
      //Cuts 
      //Diphoton mass 
      bool select = ((diphoton.M()>121.)&&(diphoton.M()<129.));                    
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
      //Jet Cuts                                                                   
      select &= (jet.Pt()>30);                                                     
      select &= (jet.Rapidity()<4.4);                                           
  
      //Diphoton Pt cuts 
      if (i==0){select &= (diphoton.Pt()<50.);}
      if (i==1){select &= (diphoton.Pt()>50. && diphoton.Pt()<100.);}
      if (i==2){select &= (diphoton.Pt()>100. && diphoton.Pt()<150);}
      if (i==3){select &= (diphoton.Pt()>150. && diphoton.Pt()<200);}
      if (i==4){select &= (diphoton.Pt()>200.);}
      
      
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
                                                                                  
        weights = weight_b;                                                       
        //===============================================                         
        bg_tree->Fill();  
      }       
    }
    //============================================================
    //Data
    for (long k=0; k<entries_d; ++k){
      chain_d.GetEntry(k); 
      TLorentzVector yy, y1, y2, jet; 
      
      if (photon_pt[0]>photon_pt[1]){                                             
        y1.SetPtEtaPhiM(photon_pt[0],photon_eta[0], photon_phi[0], photon_m[0]);  
        y2.SetPtEtaPhiM(photon_pt[1],photon_eta[1], photon_phi[1], photon_m[1]);  
      }                                                                           
      else{                                                                       
        y2.SetPtEtaPhiM(photon_pt[0],photon_eta[0], photon_phi[0], photon_m[0]);  
        y1.SetPtEtaPhiM(photon_pt[1],photon_eta[1], photon_phi[1], photon_m[1]);  
      } 
  
      jet.SetPtEtaPhiM(jet_pt[1],jet_eta[1], jet_phi[1], jet_m[1]);  
      
      yy = y1+y2; 
  
      //Cuts 
      //Diphoton mass 
      bool select = ((yy.M()>121.)&&(yy.M()<129.));                    
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
      select &= (jet.Pt()>30);                                                     
      select &= (jet.Rapidity()<4.4);                                              
                                                                                 
      //Diphoton Pt cuts 
      if (i==0){select &= (yy.Pt()<50.);}
      if (i==1){select &= (yy.Pt()>50. && yy.Pt()<100.);}
      if (i==2){select &= (yy.Pt()>100. && yy.Pt()<150);}
      if (i==3){select &= (yy.Pt()>150. && yy.Pt()<200);}
      if (i==4){select &= (yy.Pt()>200.);}
      
      
      if (select){                                                                
        s = (abs((yy+jet).M()));                                               
        yydr = (abs(yy.DeltaR(jet)));                                          
        y1dr = (abs(y1.DeltaR(jet)));                                        
        y2dr = (abs(y2.DeltaR(jet)));                                        
        ptratio = (abs(y1.Pt())/abs(y2.Pt()));                          
        y1y = (abs(y1.Rapidity()));                                          
        y2y = (abs(y2.Rapidity()));                                          
        jety = (abs(jet.Rapidity()));                                             
        yydy = (abs(y1.Rapidity()-y2.Rapidity()));                      
                                                                                  
        yyE = (abs(yy.E()));                                                   
        y1E = (abs(y1.E()));                                                 
        y2E = (abs(y2.E()));                                                 
        jetE = (abs(jet.E()));                                                    
                                                                                  
        yypt = (abs(yy.Pt()));                                                 
        y1pt = abs(y1.Pt());                                                 
        y2pt = abs(y2.Pt());                                                 
                                                                                  
        data_tree->Fill();  
      }       
    }
    out->Write(); 
    out->Close(); 
  }
}
