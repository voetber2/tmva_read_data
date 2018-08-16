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
  
  //chain_b.Add("/home/voetberg/gosam_diphoton_jet/born/born6.root",0); 
  chain_b.Add("$TMPDIR/born6.root",0); 
  chain_b.Add("$TMPDIR/born5.root",0); 
  chain_b.Add("$TMPDIR/born7.root",0);
  chain_b.Add("$TMPDIR/born8.root",0);  
  chain_b.Add("$TMPDIR/born9.root",0); 
  chain_b.Add("$TMPDIR/born10.root",0); 
  chain_b.Add("$TMPDIR/born1.root",0); 
  
  //===============================================                            
  constexpr size_t max = 10;                                                    
                                                                                
  Float_t px_b[max], py_b[max], pz_b[max], E_b[max];                            
  Int_t nparticle_b, kf_b[max];                                                 
  Double_t weight_b;
  //===============================================
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
    TString outname = "data18_pt" + num[i] + "_back.root"; 
    TFile* out = TFile::Open(outname, "RECREATE"); 

    TTree* bg_train = new TTree("bgTrain", "background");                         
    TTree* bg_test = new TTree("bgTest", "background");
    
    bg_train->SetDirectory(out); 
    bg_test->SetDirectory(out); 
    
    cout<<"Set Directory"<<endl; 

                                                                                  
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
    bg_train->Branch("yyy", &yyy); 
    bg_train->Branch("costhet", &costhet);     
    
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
    bg_test->Branch("yyy", &yyy); 
    bg_test->Branch("costhet", &costhet);     
                                                                                  
    bg_test->Branch("weights", &weights);                                         
                                                                                  
    cout<<"Branches set"<<endl;                                                   
                                                                                  
    //==============================================                              
    const Long64_t entries_b = chain_b.GetEntries();                             
    //===============================================
  

    for (long k=0; k<entries_b; ++k){                                             
      chain_b.GetEntry(k);                                                       
                                                                                 
      vector<TLorentzVector> photons;                                             
      TLorentzVector y1, y2, jet;                                       
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
        y1 = photons[0];                                                      
        y2 = photons[1];                                                      
      }                                                                            
      else{                                                                        
        y1 = photons[1];                                                      
        y2 = photons[0];                                                      
      }                                                                            
      TLorentzVector yy = y1 + y2;
      
      //Cuts 
      //Diphoton mass 
      bool select = ((yy.M()>121.)&&(yy.M()<129.));  
      
      //Diphoton Pt cuts 
      if (i==0 && (yy.Pt()<50.)) continue;                                      
      if (i==1 && (yy.Pt()>50. && yy.Pt()<100.)) continue;                        
      if (i==2 && (yy.Pt()>100. && yy.Pt()<150)) continue;                     
      if (i==3 && (yy.Pt()>150. && yy.Pt()<200)) continue;                        
      if (i==4 && (yy.Pt()>200. && yy.Pt()<250)) continue;                      
      if (i==5 && (yy.Pt()>250. && yy.Pt()<300)) continue;                      
      if (i==6 && (yy.Pt()>300. && yy.Pt()<350)) continue;                        
      if (i==7 && (yy.Pt()>350. && yy.Pt()<400)) continue;                       
      if (i==8 && (yy.Pt()>400.)) continue;                                   


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
                                                                                 
      
      if (select){                                                                
        s = (abs((yy+jet).M()));                                               
        yydr = (abs(y1.DeltaR(y2)));                                          
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
      
        yyy = abs(yy.Rapidity()); 
        costhet= (sinh(abs(y1.PseudoRapidity()-y2.PseudoRapidity()))*2*y1.Pt()*y2.Pt())/(sqrt(1.+pow((yy.Pt()/yy.M()),2.))*pow((yy.M()),2.));

                
        weights = weight_b;                                                       
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
    cout<<bg_train->GetEntries() + bg_test->GetEntries()<<" events"<<endl; 
    out->Write(); 
    out->Close(); 
  }
}
