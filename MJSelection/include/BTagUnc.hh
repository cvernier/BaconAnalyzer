//============================================================================================
// B-tagging Uncertainty functions:
//-----------------------------
//  - jet and subjet b-tagging SFs
//  - b-tag MC efficiencies for b-jets, c-jets, and light flavor(u/d/s/g) jets
//  - b-tag event weight depending on # of selected b-tags in event
//============================================================================================
#ifndef BTAGUNC_HH
#define BTAGUNC_HH

// bacon object headers
#include "BaconAna/DataFormats/interface/TJet.hh"
#include "BaconAna/DataFormats/interface/TAddJet.hh"
#include "BaconAna/DataFormats/interface/TGenParticle.hh"

// ROOT headers                                                                                                                                                                                                                  
#include <TLorentzVector.h>
#include <TMath.h>
#include <TClonesArray.h>

// C++ headers                                                                                                                                             
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>

// B-tag calibration and SF headers 
#include "CondFormats/BTauObjects/interface/BTagEntry.h"
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondFormats/BTauObjects/interface/BTagCalibrationReader.h"
#include "BTagCalibrationStandalone.h"

using namespace std;

//=== FUNCTION DECLARATIONS ====================================================================================== 
vector<float> getJetSFs   (std::string flavor, std::vector<const baconhep::TJet*> vGoodJet, BTagCalibrationReader *HFreader, BTagCalibrationReader *LFreader);
vector<float> getSubJetSFs(std::string flavor, const TClonesArray* genParArr, std::vector<TLorentzVector> vGoodSubJet, BTagCalibrationReader *HFSJreader, BTagCalibrationReader *LFSJreader);
float getBtagEventReweight      (int NminBjets, std::vector<const baconhep::TJet*> &vJet, std::vector <float> SF);
float getSubJetBtagEventReweight(const TClonesArray* genParArr, int NminBjets, std::vector <TLorentzVector> &vSubJet, std::vector <float> SF);

//=== FUNCTION DEFINITIONS ======================================================================================
extern vector<float> getJetSFs(std::string flavor, std::vector<const baconhep::TJet*> vGoodJet, BTagCalibrationReader *HFreader, BTagCalibrationReader *LFreader) {
  std::vector<float> vSF, vSFb, vSFlf;
  // std::vector<float> vSFerr;
  
  for(unsigned int ijet=0; ijet<vGoodJet.size(); ++ijet){
    
    bool isB=false; bool isC=false; bool isLF=false;

     if(fabs(vGoodJet[ijet]->hadronFlavor)==5)      isB=true;  
     else if(fabs(vGoodJet[ijet]->hadronFlavor)==4) isC=true;
     else if(fabs(vGoodJet[ijet]->hadronFlavor)==0) isLF=true;

     float SF=1, SFb=1, SFlf=1;
     //if jet pt < 30 things are messed up...
     //read scale factor depending on jet pt & flavor
     if(isB){
       if(vGoodJet[ijet]->pt < 670 && fabs(vGoodJet[ijet]->eta) < 2.4){
	 SF = HFreader->eval(BTagEntry::FLAV_B, vGoodJet[ijet]->eta, vGoodJet[ijet]->pt,0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_B, vGoodJet[ijet]->eta, vGoodJet[ijet]->pt,0.);
       }
       if(fabs(vGoodJet[ijet]->eta) > 2.4){//eta out of bounds safety
	 SF = HFreader->eval(BTagEntry::FLAV_B, 2.399 , vGoodJet[ijet]->pt,0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_B, 2.399 , vGoodJet[ijet]->pt,0.);
       }
       if(vGoodJet[ijet]->pt > 670){       //pt out of bounds safety
	 SF = HFreader->eval(BTagEntry::FLAV_B, vGoodJet[ijet]->eta, 669., 0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_B, vGoodJet[ijet]->eta, 669., 0.);
       }
       
     } else if (isC){
       if(vGoodJet[ijet]->pt < 670 && fabs(vGoodJet[ijet]->eta) < 2.4){
	 SF = HFreader->eval(BTagEntry::FLAV_C,vGoodJet[ijet]->eta, vGoodJet[ijet]->pt,0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_C,vGoodJet[ijet]->eta, vGoodJet[ijet]->pt,0.);
       }
       if(fabs(vGoodJet[ijet]->eta) > 2.4){
	 SF = HFreader->eval(BTagEntry::FLAV_C, 2.399 , vGoodJet[ijet]->pt,0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_C, 2.399 , vGoodJet[ijet]->pt,0.);
       }
       if(vGoodJet[ijet]->pt > 670){
	 SF = HFreader->eval(BTagEntry::FLAV_C,vGoodJet[ijet]->eta, 669., 0.);
	 SFb = HFreader->eval(BTagEntry::FLAV_C,vGoodJet[ijet]->eta, 669., 0.);
       }
     } else if (isLF){ // mistag
       if(vGoodJet[ijet]->pt < 1000 && fabs(vGoodJet[ijet]->eta) < 2.4){
	 SF = LFreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodJet[ijet]->eta), vGoodJet[ijet]->pt,0.);
	 SFlf = LFreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodJet[ijet]->eta), vGoodJet[ijet]->pt,0.);
       }
       if(fabs(vGoodJet[ijet]->eta) > 2.4){
	 SF = LFreader->eval(BTagEntry::FLAV_UDSG, 2.399 , vGoodJet[ijet]->pt,0.);
	 SFlf = LFreader->eval(BTagEntry::FLAV_UDSG, 2.399 , vGoodJet[ijet]->pt,0.);
       }
       if(vGoodJet[ijet]->pt > 1000) {
	 SF = LFreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodJet[ijet]->eta), 999., 0.);
	 SFlf = LFreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodJet[ijet]->eta), 999., 0.);
       }
     }
     
     //  SFerr = fabs(SF_UP - SF) > fabs(SF_DN - SF) ? fabs(SF_DN - SF):fabs(SF_UP - SF);
     vSFb.push_back(SFb);
     vSFlf.push_back(SFlf);
     vSF.push_back(SF);
     //  vSFerr.push_back(SFerr);
  }
  if(flavor.compare("Bs")==0) return vSFb;  
  if(flavor.compare("Ms")==0) return vSFlf;
  return vSF;//vSFb*SFlf
}
//-------------------------------------------------------------------------------------------------------------------------
extern vector<float> getSubJetSFs(std::string flavor, const TClonesArray* genParArr, std::vector<TLorentzVector> vGoodSubJet, BTagCalibrationReader *HFSJreader, BTagCalibrationReader *LFSJreader)
{
  std::vector<float> vSF, vSFb, vSFlf;
  // std::vector<float> vSFerr;

  for(unsigned int ijet=0; ijet<vGoodSubJet.size(); ++ijet){

    bool isB=false; bool isC=false; bool isLF=false;
    TLorentzVector vq;

    for(int j=0; j<genParArr->GetEntriesFast(); j++) {
      const baconhep::TGenParticle *genp = (baconhep::TGenParticle*)genParArr->At(j);
      if(abs(genp->pdgId)==5) {
        vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 5);
        if(vq.DeltaR(vGoodSubJet[ijet])<0.4) isB = true;
      }
      else if(abs(genp->pdgId)==4) {
        vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 1.29);
        if(vq.DeltaR(vGoodSubJet[ijet])<0.4) isC = true;
      }
      else isLF = true;
    }

    float SF=1, SFb=1, SFlf=1;
    //if jet pt < 30 things are messed up...                                                                                                                                                                                     
    //read scale factor depending on jet pt & flavor  
    if(isB){ //isB
      if(vGoodSubJet[ijet].Pt() < 670 && fabs(vGoodSubJet[ijet].Eta()) < 2.4){
	SF = HFSJreader->eval(BTagEntry::FLAV_B, vGoodSubJet[ijet].Eta(), vGoodSubJet[ijet].Pt(),0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_B, vGoodSubJet[ijet].Eta(), vGoodSubJet[ijet].Pt(),0.);
      }
      if(fabs(vGoodSubJet[ijet].Eta()) > 2.4){//eta out of bounds safety
	SF = HFSJreader->eval(BTagEntry::FLAV_B, 2.399 , vGoodSubJet[ijet].Pt(),0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_B, 2.399 , vGoodSubJet[ijet].Pt(),0.);
      }
      if(vGoodSubJet[ijet].Pt() > 670){       //pt out of bounds safety
	SF = HFSJreader->eval(BTagEntry::FLAV_B, vGoodSubJet[ijet].Eta(), 669., 0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_B, vGoodSubJet[ijet].Eta(), 669., 0.);
      }
    } else if (isC){ //isC
      if(vGoodSubJet[ijet].Pt() < 670 && fabs(vGoodSubJet[ijet].Eta()) < 2.4){
	SF = HFSJreader->eval(BTagEntry::FLAV_C,vGoodSubJet[ijet].Eta(), vGoodSubJet[ijet].Pt(),0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_C,vGoodSubJet[ijet].Eta(), vGoodSubJet[ijet].Pt(),0.);
      }
      if(fabs(vGoodSubJet[ijet].Eta()) > 2.4){
	SF = HFSJreader->eval(BTagEntry::FLAV_C, 2.399 , vGoodSubJet[ijet].Pt(),0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_C, 2.399 , vGoodSubJet[ijet].Pt(),0.);
      }
      if(vGoodSubJet[ijet].Pt() > 670){
	SF = HFSJreader->eval(BTagEntry::FLAV_C,vGoodSubJet[ijet].Eta(), 669., 0.);
	SFb = HFSJreader->eval(BTagEntry::FLAV_C,vGoodSubJet[ijet].Eta(), 669., 0.);
      }
    } else if (isLF){ //isLF
      if(vGoodSubJet[ijet].Pt() < 1000 && fabs(vGoodSubJet[ijet].Eta()) < 2.4){
	SF = LFSJreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodSubJet[ijet].Eta()), vGoodSubJet[ijet].Pt(),0.);
	SFlf = LFSJreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodSubJet[ijet].Eta()), vGoodSubJet[ijet].Pt(),0.);
      }
      if(fabs(vGoodSubJet[ijet].Eta()) > 2.4){
	SF = LFSJreader->eval(BTagEntry::FLAV_UDSG, 2.399 , vGoodSubJet[ijet].Pt(),0.);
	SFlf = LFSJreader->eval(BTagEntry::FLAV_UDSG, 2.399 , vGoodSubJet[ijet].Pt(),0.);
      }
      if(vGoodSubJet[ijet].Pt() > 1000) {
	SF = LFSJreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodSubJet[ijet].Eta()), 999., 0.);
	SFlf = LFSJreader->eval(BTagEntry::FLAV_UDSG,fabs(vGoodSubJet[ijet].Eta()), 999., 0.);
      }
    }

    //  SFerr = fabs(SF_UP - SF) > fabs(SF_DN - SF) ? fabs(SF_DN - SF):fabs(SF_UP - SF);                                                                                                                                         
    vSFb.push_back(SFb);
    vSFlf.push_back(SFlf);
    vSF.push_back(SF);
    //  vSFerr.push_back(SFerr);                                                                                                                                                                                                 
  }
  if(flavor.compare("Bs")) return vSFb;
  else if(flavor.compare("Ms")) return vSFlf;
  else  return vSF;
}
//-------------------------------------------------------------------------------------------------------------------------
extern float getBtagEventReweight(int NminBjets, std::vector <const baconhep::TJet*> &vJet, std::vector <float> SF){

  if(vJet.size()==0) return 1;
  
  float mcTag = 1.;
  float mcNoTag = 0.;
  float dataTag = 1.;
  float dataNoTag = 0.;
  
  float mcProd_0 = 1.;
  float dataProd_0 = 1.;
  
  float mcProd_1 = 1.;
  float dataProd_1 = 1.;

  float mcSum = 0.;
  float dataSum = 0.;
  
  // float err1 = 0; 
  // float err2 = 0;
  
  float wtbtag;
  // const float wtbtagErr;
  
  std::vector<float> bjet_ptbin_eff, cjet_ptbin_eff, ljet_ptbin_eff;
  // calculation done in all hadronic ttbar (powheg)
  bjet_ptbin_eff.push_back(0.60592);    cjet_ptbin_eff.push_back(0.122067);   ljet_ptbin_eff.push_back(0.0149924); 
  bjet_ptbin_eff.push_back(0.634613);   cjet_ptbin_eff.push_back(0.119659);   ljet_ptbin_eff.push_back(0.0122088);
  bjet_ptbin_eff.push_back(0.645663);   cjet_ptbin_eff.push_back(0.123723);   ljet_ptbin_eff.push_back(0.0116547);
  bjet_ptbin_eff.push_back(0.646712);   cjet_ptbin_eff.push_back(0.132141);   ljet_ptbin_eff.push_back(0.0116757);
  bjet_ptbin_eff.push_back(0.649283);   cjet_ptbin_eff.push_back(0.143654);   ljet_ptbin_eff.push_back(0.0151652);
  bjet_ptbin_eff.push_back(0.621973);   cjet_ptbin_eff.push_back(0.143127);   ljet_ptbin_eff.push_back(0.0165696);
  bjet_ptbin_eff.push_back(0.554093);   cjet_ptbin_eff.push_back(0.133581);   ljet_ptbin_eff.push_back(0.0200991);
  
  std::vector<float> ptbinlow, ptbinhigh;
  ptbinlow.push_back(30);  ptbinhigh.push_back(50);
  ptbinlow.push_back(50);  ptbinhigh.push_back(70);
  ptbinlow.push_back(70);  ptbinhigh.push_back(100);
  ptbinlow.push_back(100); ptbinhigh.push_back(140);
  ptbinlow.push_back(140); ptbinhigh.push_back(200);
  ptbinlow.push_back(200); ptbinhigh.push_back(300);
  ptbinlow.push_back(300); ptbinhigh.push_back(670);
  
  //if jet pt < 30 things are messed up...   
  for(unsigned int ij=0; ij<vJet.size(); ++ij){
    for(unsigned int ipt=0; ipt<ptbinhigh.size(); ++ipt){
      if(vJet[ij]->pt > 670){
	if(vJet[ij]->hadronFlavor == 5)    {    mcTag  = bjet_ptbin_eff[ipt]; mcNoTag  = (1 - bjet_ptbin_eff[ipt]); dataTag  = bjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - bjet_ptbin_eff[ipt]*SF[ij]); 
	} else if(vJet[ij]->hadronFlavor == 4){ mcTag  = cjet_ptbin_eff[ipt]; mcNoTag  = (1 - cjet_ptbin_eff[ipt]); dataTag  = cjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - cjet_ptbin_eff[ipt]*SF[ij]);
	} else if(vJet[ij]->hadronFlavor == 0){ mcTag  = ljet_ptbin_eff[ipt]; mcNoTag  = (1 - ljet_ptbin_eff[ipt]); dataTag  = ljet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - ljet_ptbin_eff[ipt]*SF[ij]);
	}
      }
      if(vJet[ij]->pt > ptbinlow[ipt] && vJet[ij]->pt <= ptbinhigh[ipt]){
	if(vJet[ij]->hadronFlavor == 5)     {   mcTag  = bjet_ptbin_eff[ipt]; mcNoTag  = (1 - bjet_ptbin_eff[ipt]); dataTag  = bjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - bjet_ptbin_eff[ipt]*SF[ij]);
	} else if(vJet[ij]->hadronFlavor == 4){ mcTag  = cjet_ptbin_eff[ipt]; mcNoTag  = (1 - cjet_ptbin_eff[ipt]); dataTag  = cjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - cjet_ptbin_eff[ipt]*SF[ij]);
	} else if(vJet[ij]->hadronFlavor == 0){ mcTag  = ljet_ptbin_eff[ipt]; mcNoTag  = (1 - ljet_ptbin_eff[ipt]); dataTag  = ljet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - ljet_ptbin_eff[ipt]*SF[ij]);}
      }
    }
    
    mcProd_0   *= mcNoTag;
    dataProd_0 *= dataNoTag;

    for(unsigned int ik=0; ik < vJet.size(); ++ik){
       for(unsigned int jpt=0; jpt<ptbinhigh.size(); ++jpt){
	 if(ik == ij) continue;
	 if(vJet[ik]->pt > 670){
	   if(vJet[ik]->hadronFlavor == 5)    {    mcNoTag = (1 - bjet_ptbin_eff[jpt]); dataNoTag  = (1 - bjet_ptbin_eff[jpt]*SF[ik]); 
	   } else if(vJet[ik]->hadronFlavor == 4){ mcNoTag = (1 - cjet_ptbin_eff[jpt]); dataNoTag  = (1 - cjet_ptbin_eff[jpt]*SF[ik]);
	   } else if(vJet[ik]->hadronFlavor == 0){ mcNoTag = (1 - ljet_ptbin_eff[jpt]); dataNoTag  = (1 - ljet_ptbin_eff[jpt]*SF[ik]);
	   }
	 }
	 if(vJet[ik]->pt > ptbinlow[jpt] && vJet[ik]->pt <= ptbinhigh[jpt]){
	   if(vJet[ik]->hadronFlavor == 5)     {    mcNoTag  = (1 - bjet_ptbin_eff[jpt]);  dataNoTag  = (1 - bjet_ptbin_eff[jpt]*SF[ik]);
	   } else if(vJet[ik]->hadronFlavor == 4){  mcNoTag  = (1 - cjet_ptbin_eff[jpt]);  dataNoTag  = (1 - cjet_ptbin_eff[jpt]*SF[ik]);
	   } else if(vJet[ik]->hadronFlavor == 0){  mcNoTag  = (1 - ljet_ptbin_eff[jpt]);  dataNoTag  = (1 - ljet_ptbin_eff[jpt]*SF[ik]);}
	 }
       }
       mcProd_1   *= mcNoTag;
       dataProd_1 *= dataNoTag;
    }

    mcSum   += mcTag*mcProd_1;
    dataSum += dataTag*dataProd_1;
  
  }
  
  // re-weighting for events with 0 jets
  if(NminBjets==0){   
    return wtbtag = dataProd_0/mcProd_0;

    //re-weighting for events with exactly 1 b-tag jet
  } else if(NminBjets==-1){
    return wtbtag = dataSum/mcSum;
    
    // re-weighting for events with 1 or more b-tag jets
  } else if(NminBjets==1){
    return wtbtag = (1 - dataProd_0)/(1 - mcProd_0);

    // re-weighting for events with 2 or more b-tag jets
  } else if(NminBjets==2){
    return wtbtag = (1 - dataProd_0 - dataSum)/(1 - mcProd_0 - mcSum);
    
  } 
  else return 1;

  //return wtbtagErr = sqrt(2*(pow(err1+err2,2)))*wtbtag;
}
//-------------------------------------------------------------------------------------------------------------------------
extern float getSubJetBtagEventReweight(const TClonesArray* genParArr, int NminBjets, std::vector <TLorentzVector> &vSubJet, std::vector <float> SF)
{

  if(vSubJet.size()==0) return 1;

  float mcTag = 1.;
  float mcNoTag = 0.;
  float dataTag = 1.;
  float dataNoTag = 0.;

  float mcProd_0 = 1.;
  float dataProd_0 = 1.;

  float mcProd_1 = 1.;
  float dataProd_1 = 1.;

  float mcSum = 0.;
  float dataSum = 0.;

  // float err1 = 0;
  // float err2 = 0;

  float wtbtag;
  // const float wtbtagErr;

  std::vector<float> bjet_ptbin_eff, cjet_ptbin_eff, ljet_ptbin_eff;
  // calculation done in all hadronic ttbar (powheg)
  bjet_ptbin_eff.push_back(0.60592);    cjet_ptbin_eff.push_back(0.122067);   ljet_ptbin_eff.push_back(0.0149924);
  bjet_ptbin_eff.push_back(0.634613);   cjet_ptbin_eff.push_back(0.119659);   ljet_ptbin_eff.push_back(0.0122088);
  bjet_ptbin_eff.push_back(0.645663);   cjet_ptbin_eff.push_back(0.123723);   ljet_ptbin_eff.push_back(0.0116547);
  bjet_ptbin_eff.push_back(0.646712);   cjet_ptbin_eff.push_back(0.132141);   ljet_ptbin_eff.push_back(0.0116757);
  bjet_ptbin_eff.push_back(0.649283);   cjet_ptbin_eff.push_back(0.143654);   ljet_ptbin_eff.push_back(0.0151652);
  bjet_ptbin_eff.push_back(0.621973);   cjet_ptbin_eff.push_back(0.143127);   ljet_ptbin_eff.push_back(0.0165696);
  bjet_ptbin_eff.push_back(0.554093);   cjet_ptbin_eff.push_back(0.133581);   ljet_ptbin_eff.push_back(0.0200991);

  std::vector<float> ptbinlow, ptbinhigh;
  ptbinlow.push_back(30);  ptbinhigh.push_back(50);
  ptbinlow.push_back(50);  ptbinhigh.push_back(70);
  ptbinlow.push_back(70);  ptbinhigh.push_back(100);
  ptbinlow.push_back(100); ptbinhigh.push_back(140);
  ptbinlow.push_back(140); ptbinhigh.push_back(200);
  ptbinlow.push_back(200); ptbinhigh.push_back(300);
  ptbinlow.push_back(300); ptbinhigh.push_back(670);

  //if jet pt < 30 things are messed up...
  for(unsigned int ij=0; ij<vSubJet.size(); ++ij){

    bool isBij=false; bool isCij=false; bool isLFij=false;
    TLorentzVector vq;

    for(int j=0; j<genParArr->GetEntriesFast(); j++) {
      const baconhep::TGenParticle *genp = (baconhep::TGenParticle*)genParArr->At(j);
      if(abs(genp->pdgId)==5) {
        vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 5);
        if(vq.DeltaR(vSubJet[ij])<0.4) isBij = true;
      }
      else if(abs(genp->pdgId)==4) {
        vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 1.29);
        if(vq.DeltaR(vSubJet[ij])<0.4) isCij = true;
      }
      else isLFij = true;
    }

    for(unsigned int ipt=0; ipt<ptbinhigh.size(); ++ipt){
      if(vSubJet[ij].Pt() > 670){
        if(isBij)        { mcTag = bjet_ptbin_eff[ipt]; mcNoTag = (1 - bjet_ptbin_eff[ipt]); dataTag  = bjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - bjet_ptbin_eff[ipt]*SF[ij]);
        } else if(isCij) { mcTag = cjet_ptbin_eff[ipt]; mcNoTag = (1 - cjet_ptbin_eff[ipt]); dataTag  = cjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - cjet_ptbin_eff[ipt]*SF[ij]);
        } else if(isLFij){ mcTag = ljet_ptbin_eff[ipt]; mcNoTag = (1 - ljet_ptbin_eff[ipt]); dataTag  = ljet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - ljet_ptbin_eff[ipt]*SF[ij]);
        }
      }
      if(vSubJet[ij].Pt() > ptbinlow[ipt] && vSubJet[ij].Pt() <= ptbinhigh[ipt]){
        if(isBij)        { mcTag  = bjet_ptbin_eff[ipt]; mcNoTag  = (1 - bjet_ptbin_eff[ipt]); dataTag  = bjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - bjet_ptbin_eff[ipt]*SF[ij]);
        } else if(isCij) { mcTag  = cjet_ptbin_eff[ipt]; mcNoTag  = (1 - cjet_ptbin_eff[ipt]); dataTag  = cjet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - cjet_ptbin_eff[ipt]*SF[ij]);
        } else if(isLFij){ mcTag  = ljet_ptbin_eff[ipt]; mcNoTag  = (1 - ljet_ptbin_eff[ipt]); dataTag  = ljet_ptbin_eff[ipt]*SF[ij]; dataNoTag  = (1 - ljet_ptbin_eff[ipt]*SF[ij]);}
      }
    }

    mcProd_0   *= mcNoTag;
    dataProd_0 *= dataNoTag;

    for(unsigned int ik=0; ik < vSubJet.size(); ++ik){
      bool isBik=false; bool isCik=false; bool isLFik=false;
      TLorentzVector vq;

      for(int j=0; j<genParArr->GetEntriesFast(); j++) {
        const baconhep::TGenParticle *genp = (baconhep::TGenParticle*)genParArr->At(j);
        if(abs(genp->pdgId)==5) {
          vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 5);
          if(vq.DeltaR(vSubJet[ik])<0.4) isBik = true;
        }
        else if(abs(genp->pdgId)==4) {
          vq.SetPtEtaPhiM(genp->pt, genp->eta, genp->phi, 1.29);
          if(vq.DeltaR(vSubJet[ik])<0.4) isCik = true;
        }
        else isLFik = true;
      }

      for(unsigned int jpt=0; jpt<ptbinhigh.size(); ++jpt){
        if(ik == ij) continue;
        if(vSubJet[ik].Pt() > 670){
          if(isBik)        { mcNoTag = (1 - bjet_ptbin_eff[jpt]); dataNoTag  = (1 - bjet_ptbin_eff[jpt]*SF[ik]);
          } else if(isCik) { mcNoTag = (1 - cjet_ptbin_eff[jpt]); dataNoTag  = (1 - cjet_ptbin_eff[jpt]*SF[ik]);
          } else if(isLFik){ mcNoTag = (1 - ljet_ptbin_eff[jpt]); dataNoTag  = (1 - ljet_ptbin_eff[jpt]*SF[ik]);
	  }
        }
        if(vSubJet[ik].Pt() > ptbinlow[jpt] && vSubJet[ik].Pt() <= ptbinhigh[jpt]){
          if(isBik)        { mcNoTag  = (1 - bjet_ptbin_eff[jpt]);  dataNoTag  = (1 - bjet_ptbin_eff[jpt]*SF[ik]);
          } else if(isCik) { mcNoTag  = (1 - cjet_ptbin_eff[jpt]);  dataNoTag  = (1 - cjet_ptbin_eff[jpt]*SF[ik]);
          } else if(isLFik){ mcNoTag  = (1 - ljet_ptbin_eff[jpt]);  dataNoTag  = (1 - ljet_ptbin_eff[jpt]*SF[ik]);}
        }
      }
      mcProd_1   *= mcNoTag;
      dataProd_1 *= dataNoTag;
    }

    mcSum   += mcTag*mcProd_1;
    dataSum += dataTag*dataProd_1;
  }


  // re-weighting for events with 0 jets
  if(NminBjets==0){
    return wtbtag = dataProd_0/mcProd_0;

    //re-weighting for events with exactly 1 b-tag jet
  } else if(NminBjets==-1){
    return wtbtag = dataSum/mcSum;
    // re-weighting for events with 1 or more b-tag jets                                                                                                                                                                          
  } else if(NminBjets==1){
    return wtbtag = (1 - dataProd_0)/(1 - mcProd_0);
    // re-weighting for events with 2 or more b-tag jets                                                                                                                                                                          
  } else if(NminBjets==2){
    return wtbtag = (1 - dataProd_0 - dataSum)/(1 - mcProd_0 - mcSum);

  }
  else return 1;

  //return wtbtagErr = sqrt(2*(pow(err1+err2,2)))*wtbtag;                                                                                                                                                                       
}

#endif
