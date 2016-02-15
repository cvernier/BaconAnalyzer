#include "../include/VJetLoader.hh"
#include <cmath>
#include <iostream> 

using namespace baconhep;

VJetLoader::VJetLoader(TTree *iTree,std::string iHLTFile) { 
  fVJets     = new TClonesArray("baconhep::TJet");
  fFatJets   = new TClonesArray("baconhep::TJet");
  fVAddJets  = new TClonesArray("baconhep::TAddJet");
  iTree->SetBranchAddress("CA8Puppi",       &fVJets);
  iTree->SetBranchAddress("AddCA8Puppi",    &fVAddJets);
  fVJetBr     = iTree->GetBranch("CA8Puppi");
  fVAddJetBr  = iTree->GetBranch("AddCA8Puppi");
  
  iTree->SetBranchAddress("CA15Puppi",      &fFatJets);
  fFatJetBr   = iTree->GetBranch("CA15Puppi");
  
  fTrigger = new TTrigger(iHLTFile);
  fTrigString.push_back("HLT_AK8PFJet360_TrimMass30_v*");
  fTrigString.push_back("HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v*");
  fN = 1;
}
VJetLoader::~VJetLoader() { 
  delete fVJets;
  delete fVJetBr;
  delete fVAddJets;
  delete fVAddJetBr;
  //delete fVPupJets;
  //delete fVPupJetBr;
  //delete fVPupAddJets;
  //delete fVPupAddJetBr;
}
void VJetLoader::reset() { 
  fNVJets = 0; 
  fSelVJets.clear();
  for(unsigned int i0 = 0; i0 < fVars.size(); i0++) fVars[i0] = 0;
}
void VJetLoader::setupTree(TTree *iTree) { 
  reset();
  fLabels.clear();
  fLabels.push_back("mass");
  fLabels.push_back("csv");
  fLabels.push_back("flavor");
  fLabels.push_back("t1");
  fLabels.push_back("t2");
  fLabels.push_back("t3");
  fLabels.push_back("msd");
  fLabels.push_back("mprune");
  fLabels.push_back("mtrim");
  fLabels.push_back("pullAngle");
  fLabels.push_back("sj1_csv");
  fLabels.push_back("sj2_csv");
  fLabels.push_back("sj1_qgid");
  fLabels.push_back("sj2_qgid");
  fLabels.push_back("sj1_q");
  fLabels.push_back("sj2_q");
  fLabels.push_back("sj1_z");
  fLabels.push_back("sj2_z");
  fLabels.push_back("iso15");
  fLabels.push_back("trig");
  fLabels.push_back("genm");
  fLabels.push_back("genV");
  fTree = iTree;
  fTree->Branch("nvjet",&fNVJets,"fNVJets/I");
  for(int i0 = 0; i0 < fN*3.;                    i0++) {double pVar = 0; fVars.push_back(pVar);} 
  for(int i0 = 0; i0 < fN*(int(fLabels.size())); i0++) {double pVar = 0; fVars.push_back(pVar);} 
  setupNtuple("vjet",iTree,fN,fVars);
  setupNtuple("vjet",iTree,fN,fVars,fN*3,fLabels);
}
void VJetLoader::load(int iEvent) { 
  fVJets       ->Clear();
  fVJetBr      ->GetEntry(iEvent);
  fVAddJets    ->Clear();
  fVAddJetBr   ->GetEntry(iEvent);
  fFatJets       ->Clear();
  fFatJetBr      ->GetEntry(iEvent);
}
bool VJetLoader::selectVJets(std::vector<TLorentzVector> &iVetoes,bool iVeto) {
  reset(); 
  int lCount = 0; 
  for  (int i0 = 0; i0 < fVJets->GetEntriesFast(); i0++) { 
    TJet *pVJet = (TJet*)((*fVJets)[i0]);
    if(pVJet->pt        <  200)         continue;
    if(fabs(pVJet->eta) >  2.3)         continue;
    if(pVJet->mass < 40)                continue; //First Pass
    if(passVeto(pVJet->eta,pVJet->phi,iVetoes)) continue;
    if(iVeto) addVJet(pVJet,iVetoes,pVJet->mass);
    addJet(pVJet,fSelVJets);
    lCount++;
  }
  fNVJets = lCount;
  fillJet( fN,fSelVJets,fVars);
  fillVJet(fN,fSelVJets,fVars);
  if(fSelVJets.size() == 0) return false;
  return true;
}
void VJetLoader::fillVJet(int iN,std::vector<TJet*> &iObjects,std::vector<double> &iVals) { 
  int lBase = 3.*fN;
  int lMin = iObjects.size();
  int lNLabel = int(fLabels.size());
  if(iN < lMin) lMin = iN;
  for(int i0 = 0; i0 < lMin; i0++) { 
    TAddJet *pAddJet = getAddJet(iObjects[i0]);
    iVals[lBase+i0*lNLabel+0]  = iObjects[i0]->mass;
    iVals[lBase+i0*lNLabel+1]  = iObjects[i0]->csv;
    iVals[lBase+i0*lNLabel+2]  = iObjects[i0]->partonFlavor;
    iVals[lBase+i0*lNLabel+3]  = pAddJet     ->tau1;
    iVals[lBase+i0*lNLabel+4]  = pAddJet     ->tau2;
    iVals[lBase+i0*lNLabel+5]  = pAddJet     ->tau3;
    iVals[lBase+i0*lNLabel+6]  = pAddJet     ->mass_sd0;
    iVals[lBase+i0*lNLabel+7]  = pAddJet     ->mass_prun;
    iVals[lBase+i0*lNLabel+8]  = pAddJet     ->mass_trim;
    iVals[lBase+i0*lNLabel+9]  = pAddJet     ->pullAngle;
    iVals[lBase+i0*lNLabel+10] = pAddJet     ->sj1_csv;
    iVals[lBase+i0*lNLabel+11] = pAddJet     ->sj2_csv;
    iVals[lBase+i0*lNLabel+12] = pAddJet     ->sj1_qgid;
    iVals[lBase+i0*lNLabel+13] = pAddJet     ->sj2_qgid;
    iVals[lBase+i0*lNLabel+14] = pAddJet     ->sj1_q;
    iVals[lBase+i0*lNLabel+15] = pAddJet     ->sj2_q;
    iVals[lBase+i0*lNLabel+16] = pAddJet     ->sj1_pt/iObjects[i0]->pt;
    iVals[lBase+i0*lNLabel+17] = pAddJet     ->sj2_pt/iObjects[i0]->pt;
    TJet *pLargeJet = getLargeJet(iObjects[i0]);
    if(pLargeJet != 0) iVals[lBase+i0*lNLabel+18] = pLargeJet->pt - iObjects[i0]->pt;
    if(pLargeJet != 0) iVals[lBase+i0*lNLabel+19] = pullDot(pLargeJet->pullY,iObjects[i0]->pullY,pLargeJet->pullPhi,iObjects[i0]->pullPhi);
    iVals[lBase+i0*lNLabel+20] = iObjects[i0]->genm;
    iVals[lBase+i0*lNLabel+21] = trigger(iObjects[i0]);
  }
}
void VJetLoader::addBoson(TGenParticle *iBoson) { 
  int lBase = 3.*fN;
  int lMin = int(fSelVJets.size());
  int lNLabel = int(fLabels.size());
  if(fN < lMin) lMin = fN;
  for(int i0 = 0; i0 < lMin; i0++) {
    float pDEta = fabs(iBoson->eta - fSelVJets[i0]->eta);
    float pDPhi = fabs(iBoson->phi - fSelVJets[i0]->phi);
    if(pDPhi > 2.*TMath::Pi()-pDPhi) pDPhi =  2.*TMath::Pi()-pDPhi;
    fVars[lBase+i0*lNLabel+lNLabel-1] = sqrt(pDEta*pDEta+pDPhi*pDPhi);
  }
}
TAddJet *VJetLoader::getAddJet(TJet *iJet) { 
  int lIndex = -1;
  TAddJet *lJet = 0; 
  for(int i0 = 0; i0 < fVJets->GetEntriesFast(); i0++) { 
    if((*fVJets)[i0] == iJet) { lIndex = i0; break;}
  }
  if(lIndex == -1) return 0;
  for  (int i0 = 0; i0 < fVAddJets->GetEntriesFast(); i0++) { 
    TAddJet *pJet = (TAddJet*)((*fVAddJets)[i0]);
    if(pJet->index == fabs(lIndex)) { lJet = pJet; break;}
  }
  return lJet;
}
int  VJetLoader::trigger(TJet *iJet) { 
  int pId = 0; 
  for(int i0 = 0; i0 < int(fTrigString.size()); i0++) if(fTrigger->passObj(fTrigString[i0],1,iJet->hltMatchBits))  pId += TMath::Power(2.,i0);
  return pId;
}
float VJetLoader::pullDot(float iY1,float iY2,float iPhi1,float iPhi2) { 
  return (iY1*iY1 + iPhi1*iPhi2);
}
TJet* VJetLoader::getLargeJet(TJet *iMatch) { 
  TJet *lFatJet = 0;
  for  (int i0 = 0; i0 < fFatJets->GetEntriesFast(); i0++) {
    TJet *pFatJet = (TJet*)((*fFatJets)[i0]);
    float pDEta = fabs(pFatJet->eta-iMatch->eta);
    float pDPhi = fabs(pFatJet->phi-iMatch->phi);
    if(pDPhi > 2.*TMath::Pi()-pDPhi) pDPhi =  2.*TMath::Pi()-pDPhi;
    if(sqrt(pDEta*pDEta+pDPhi*pDPhi) > 0.4) continue;
    lFatJet = pFatJet;
    break;
  }
  return lFatJet;
}
