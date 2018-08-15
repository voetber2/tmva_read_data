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
  TChain chain_s("t3"); 
  
  chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_100.root",0); 
  chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_101.root",0); 
  chain_s.Add("$TMPDIR/H1.0j_GGFHT_B_6500_pt25.0_eta4.5_r100_102.root",0); 
  //===============================================                            
  constexpr size_t max = 10;                                                    
  Float_t px_s[max], py_s[max], pz_s[max], E_s[max];                            
  Int_t nparticle_s, kf_s[max];                                                 
  Double_t weight_s;                                                            
                                                                                
  //===============================================
  chain_s.SetBranchAddress("nparticle",&nparticle_s);                          
  chain_s.SetBranchAddress("kf",kf_s);                                         
  chain_s.SetBranchAddress("px",px_s);                                         
  chain_s.SetBranchAddress("py",py_s);                                         
  chain_s.SetBranchAddress("pz",pz_s);                                         
  chain_s.SetBranchAddress("E",E_s);                                           
  chain_s.SetBranchAddress("weight2",&weight_s);                               
  //==============================================                              
  //ALL THE TREES                                                               
  Float_t s, yydr, y1dr, y2dr, ptratio, y1y, y2y,jety, yydy;                    
  Float_t y1E, y1pt, y2E, y2pt, yyE, yypt, jetE, jetpt, yyy, costhet;                         
  Float_t weights;                                                              
  //===============================================                             
  vector<TString>num;
  num.emplace_back("50"); 
  num.emplace_back("100");
  num.emplace_back("150");
  num.emplace_back("200");
  num.emplace_back("250");
  num.emplace_back("300");
  num.emplace_back("350");
  num.emplace_back("400");

  for (int i=0; i<9; ++i){
    TString outname = "data18_pt" + num[i] + "_sig.root"; 
    TFile* out = TFile::Open(outname, "RECREATE"); 

    TTree* sig_train = new TTree("sigTrain", "signal");                           
    TTree* sig_test = new TTree("sigTest", "signal");                             
    
    sig_train->SetDirectory(out); 
    sig_test->SetDirectory(out); 
    
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
    sig_train->Branch("yyy", &yyy);                                              
    sig_train->Branch("costhet", &costhet);       
                                                                                  
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
    sig_test->Branch("yyy", &yyy);                                              
    sig_test->Branch("costhet", &costhet);       
    
    sig_test->Branch("weights", &weights);                                        
                                                                                  
    cout<<"Branches set"<<endl;                                                   
                                                                                  
    //==============================================                              
    const Long64_t entries_s = chain_s.GetEntries();                             
    //===============================================
  
    //Signal                                                                      
    for (long j=0; j<entries_s; ++j){                                             
      chain_s.GetEntry(j);                                                       
      TLorentzVector yy, y1, y2, jet;                                
      vector<TLorentzVector> jets;                                                
      for (long i=0; i<nparticle_s; ++i){                                         
        if (kf_s[i]==25){                                                         
          yy.SetPxPyPzE(px_s[i],py_s[i],pz_s[i],E_s[i]);                       
        }                                                                         
        else{                                                                     
          jets.emplace_back(px_s[i],py_s[i],pz_s[i],E_s[i]);                      
        }                                                                         
      }                                                                           
                                                                                  
      //Decay yy                                                               
      pair<TLorentzVector, TLorentzVector> diphoton = Hdecay(yy);              
                                                                                  
      //Sort Photons                                                                            
      if (diphoton.first.Pt()>diphoton.second.Pt()){                              
        y1 = diphoton.first;                                                 
        y2 = diphoton.second;                                                
      }                                                                           
      else{                                                                       
        y1 = diphoton.second;                                                
        y2 = diphoton.first;                                                 
      }                                                                           
                                                                                  
      jet=jets[0]; 
      
      //Cuts
      //Mass Cut                                                                  
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
     
  
        //Diy Pt cuts 
      if (i==0){select &= (yy.Pt()<50.);}
      if (i==1){select &= (yy.Pt()>50. && yy.Pt()<100.);}
      if (i==2){select &= (yy.Pt()>100. && yy.Pt()<150);}
      if (i==3){select &= (yy.Pt()>150. && yy.Pt()<200);}
      if (i==4){select &= (yy.Pt()>200. && yy.Pt()<250);}                       
      if (i==5){select &= (yy.Pt()>250. && yy.Pt()<300.);}                      
      if (i==6){select &= (yy.Pt()>300. && yy.Pt()<350);}                       
      if (i==7){select &= (yy.Pt()>350. && yy.Pt()<400);}                       
      if (i==8){select &= (yy.Pt()>400.);} 
      
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
  
    cout<<outname<<" written"<<endl; 
    out->Write(); 
    out->Close(); 
  }
}
