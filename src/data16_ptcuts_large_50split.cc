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
  //===============================================                         
  TChain chain_b("t3"); 
  TChain chain_s("t3"); 
  
  //chain_b.Add("/home/voetberg/gosam_diphoton_jet/born/born6.root",0); 
  chain_b.Add("$TMPDIR/born6.root",0); 
  //chain_b.Add("$TMPDIR/born5.root",0); 
  //chain_b.Add("$TMPDIR/born7.root",0);
  //chain_b.Add("$TMPDIR/born8.root",0); 
  
  chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_100.root",0); 
  //chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_101.root",0); 
  //chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_102.root",0); 
  //===============================================                            
  constexpr size_t max = 10;                                                    
  Float_t px_s[max], py_s[max], pz_s[max], E_s[max];                            
  Int_t nparticle_s, kf_s[max];                                                 
  Double_t weight_s;                                                            
                                                                                
  Float_t px_b[max], py_b[max], pz_b[max], E_b[max];                            
  Int_t nparticle_b, kf_b[max];                                                 
  Double_t weight_b;
  //===============================================
  chain_s.SetBranchAddress("nparticle",&nparticle_s);                          
  chain_s.SetBranchAddress("kf",kf_s);                                         
  chain_s.SetBranchAddress("px",px_s);                                         
  chain_s.SetBranchAddress("py",py_s);                                         
  chain_s.SetBranchAddress("pz",pz_s);                                         
  chain_s.SetBranchAddress("E",E_s);                                           
  chain_s.SetBranchAddress("weight2",&weight_s);                               
                                                                                
  chain_b.SetBranchAddress("nparticle",&nparticle_b);                          
  chain_b.SetBranchAddress("kf",kf_b);                                         
  chain_b.SetBranchAddress("px",px_b);                                         
  chain_b.SetBranchAddress("py",py_b);                                         
  chain_b.SetBranchAddress("pz",pz_b);                                         
  chain_b.SetBranchAddress("E",E_b);                                           
  chain_b.SetBranchAddress("weight2",&weight_b);  
  //==============================================                              
  //ALL THE TREES                                                               
  Float_t s, yydr, y1dr, y2dr, ptratio, y1y, y2y,jety, yydy;                    
  Float_t y1E, y1pt, y2E, y2pt, yyE, yypt, jetE, jetpt;                         
  Float_t weights;                                                              
  //===============================================                             
  vector<TString>num;
  num.emplace_back("50"); 
  num.emplace_back("100");
  num.emplace_back("150");
  num.emplace_back("200");
  num.emplace_back("200p");

  for (int i=0; i<5; ++i){
    TString outname = "data16_pt" + num[i] + ".root"; 
    TFile* out = TFile::Open(outname, "RECREATE"); 

    TTree* sig_train = new TTree("sigTrain", "signal");                           
    TTree* bg_train = new TTree("bgTrain", "background");                         
    TTree* sig_test = new TTree("sigTest", "signal");                             
    TTree* bg_test = new TTree("bgTest", "background");
    
    sig_train->SetDirectory(out); 
    bg_train->SetDirectory(out); 
    sig_test->SetDirectory(out); 
    bg_test->SetDirectory(out); 
    
    cout<<"Set Directory"<<endl; 

    //===============================================                             
    //Ouputs                                 
    //Signal
    sig_train->Branch("s", &s);
    sig_train->Branch("yydr", &yydr);
    sig_train->Branch("y1dr", &y1dr);
    sig_train->Branch("y2dr", &y2dr);
    sig_train->Branch("ptratio", &ptratio);                                       
    sig_train->Branch("y1y", &y1y);                                               
    sig_train->Branch("y2y", &y2y);                                               
    sig_train->Branch("jety", &jety);                                             
    sig_train->Branch("yydy", &yydy);                                             
    sig_train->Branch("y1E", &y1E);                                               
    sig_train->Branch("y1pt", &y1pt);                                             
    sig_train->Branch("y2E", &y2E);                                               
    sig_train->Branch("y2pt", &y2pt);                                             
    sig_train->Branch("yyE", &yyE);                                               
    sig_train->Branch("yypt", &yypt);                                             
    sig_train->Branch("jetE", &jetE);                                             
                                                                                  
    sig_train->Branch("weights", &weights);  
    //                                                                            
    sig_test->Branch("s", &s);                                                    
    sig_test->Branch("yydr", &yydr);                                              
    sig_test->Branch("y1dr", &y1dr);                                              
    sig_test->Branch("y2dr", &y2dr);                                              
    sig_test->Branch("ptratio", &ptratio);                                        
    sig_test->Branch("y1y", &y1y);                                                
    sig_test->Branch("y2y", &y2y);                                                
    sig_test->Branch("jety", &jety);                                              
    sig_test->Branch("yydy", &yydy);                                              
    sig_test->Branch("y1E", &y1E);                                                
    sig_test->Branch("y1pt", &y1pt);                                              
    sig_test->Branch("y2E", &y2E);                                                
    sig_test->Branch("y2pt", &y2pt);                                              
    sig_test->Branch("yyE", &yyE);                                                
    sig_test->Branch("yypt", &yypt);                                              
    sig_test->Branch("jetE", &jetE);                                              
                                                                                  
    sig_test->Branch("weights", &weights);                                        
                                                                                  
    //Background                                                                  
    bg_train->Branch("s", &s);                                                    
    bg_train->Branch("yydr", &yydr);                                              
    bg_train->Branch("y1dr", &y1dr);                                              
    bg_train->Branch("y2dr", &y2dr);                                              
    bg_train->Branch("ptratio", &ptratio);                                        
    bg_train->Branch("y1y", &y1y);                                                
    bg_train->Branch("y2y", &y2y);                                                
    bg_train->Branch("jety", &jety);                                              
    bg_train->Branch("yydy", &yydy);                                              
    bg_train->Branch("y1E", &y1E);                                                
    bg_train->Branch("y1pt", &y1pt);                                              
    bg_train->Branch("y2E", &y2E);                                                
    bg_train->Branch("y2pt", &y2pt);                                              
    bg_train->Branch("yyE", &yyE);                                                
    bg_train->Branch("yypt", &yypt);                                              
    bg_train->Branch("jetE", &jetE);                                              
                                                                                  
    bg_train->Branch("weights", &weights);    
    //                                                                            
    bg_test->Branch("s", &s);                                                     
    bg_test->Branch("yydr", &yydr);                                               
    bg_test->Branch("y1dr", &y1dr);                                               
    bg_test->Branch("y2dr", &y2dr);                                               
    bg_test->Branch("ptratio", &ptratio);                                         
    bg_test->Branch("y1y", &y1y);                                                 
    bg_test->Branch("y2y", &y2y);                                                 
    bg_test->Branch("jety", &jety);                                               
    bg_test->Branch("yydy", &yydy);                                               
    bg_test->Branch("y1E", &y1E);                                                 
    bg_test->Branch("y1pt", &y1pt);                                               
    bg_test->Branch("y2E", &y2E);                                                 
    bg_test->Branch("y2pt", &y2pt);                                               
    bg_test->Branch("yyE", &yyE);                                                 
    bg_test->Branch("yypt", &yypt);                                               
    bg_test->Branch("jetE", &jetE);                                               
                                                                                  
    bg_test->Branch("weights", &weights);                                         
                                                                                  
    cout<<"Branches set"<<endl;                                                   
                                                                                  
    //==============================================                              
    const Long64_t entries_s = chain_s.GetEntries();                             
    const Long64_t entries_b = chain_b.GetEntries();                             
    //===============================================
  
    //Signal                                                                      
    for (long j=0; j<entries_s; ++j){                                             
      chain_s.GetEntry(j);                                                       
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
        jetpt = abs(jet.Pt());                                                    
                                                                                  
        weights = weight_s;                                                       
        //===============================================                         
        TRandom3 *r = new TRandom3(std::chrono::system_clock::now().time_since_epoch().count());
        double ran = r->Rndm();                                                   
        const bool tts = (ran<.5);                                               
                                                                                  
        if (tts){                                                                 
          sig_train->Fill();                                                      
        }                                                                         
        else{                                                                     
          sig_test->Fill();                                                       
        }                                                                         
      }       
    }
  
    cout<<"Set signal for "<<outname<<endl; 

    for (long k=0; k<entries_b; ++k){                                             
      chain_b.GetEntry(k);                                                       
                                                                                 
      vector<TLorentzVector> photons;                                             
      TLorentzVector photon1, photon2, jet;                                       
      photons.clear();                                                            
                                                                                 
      //Assign particles                                                           
      for (long j=0; j<nparticle_b; ++j){                                          
        //if photon                                                                
        if (kf_b[j]== 22){                                                         
          photons.emplace_back(px_b[j], py_b[j], pz_b[j], E_b[j]);                 
        }                                                                          
        //if jet                                                                   
        else{                                                                      
          jet.SetPxPyPzE(px_b[j], py_b[j], pz_b[j], E_b[j]);                       
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
        jetpt = abs(jet.Pt());                                                    
                                                                                  
        weights = weight_s;                                                       
        //===============================================                         
        TRandom3 *r = new TRandom3(std::chrono::system_clock::now().time_since_epoch().count());
        double ran = r->Rndm();                                                   
        const bool tts = (ran<.5);                                               
                                                                                  
        if (tts){                                                                 
          bg_train->Fill();                                                      
        }                                                                         
        else{                                                                     
          bg_test->Fill();                                                       
        }  
      }       
    }
    cout<<outname<<" written"<<endl; 
    out->Write(); 
    out->Close(); 
  }
}
