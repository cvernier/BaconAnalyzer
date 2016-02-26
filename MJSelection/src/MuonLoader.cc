#include "../include/MuonLoader.hh"
#include <cmath>
#include <iostream> 
#include <sstream> 

using namespace baconhep;

MuonLoader::MuonLoader(TTree *iTree) { 
  fMuons  = new TClonesArray("baconhep::TMuon");
  iTree->SetBranchAddress("Muon",       &fMuons);
  fMuonBr  = iTree->GetBranch("Muon");
  fN = 2;
}
MuonLoader::~MuonLoader() { 
  delete fMuons;
  delete fMuonBr;
}
void MuonLoader::reset() { 
  fNMuons      = 0; 
  fNMuonsTight = 0; 
  fSelMuons.clear();
  for(unsigned int i0 = 0; i0 < fVars.size(); i0++) fVars[i0] = 0;
}
void MuonLoader::setupTree(TTree *iTree) { 
  reset();
  fTree = iTree;
  fTree->Branch("nmu",&fNMuons,"fNMuons/I"); 
  fTree->Branch("nmuTight",&fNMuonsTight,"fNMuonsTight/I");
  for(int i0 = 0; i0 < fN*3.; i0++) {double pVar = 0; fVars.push_back(pVar);} 
  for(int i0 = 0; i0 <     4; i0++) {double pVar = 0; fVars.push_back(pVar);} 
  setupNtuple("mu",iTree,fN,fVars); // 2 Muons pt,eta,phi (2*3=6)
  addDiMuon  ("dm",iTree,1, fVars,fN*3); // dimuon system *_pt,mass,phi,y for dm0 (1*4)
}
void MuonLoader::load(int iEvent) { 
  fMuons   ->Clear();
  fMuonBr ->GetEntry(iEvent);
}
bool MuonLoader::selectMuons(std::vector<TLorentzVector> &iVetoes) {
  reset(); 
  int lCount = 0,lTCount =0; 
  for  (int i0 = 0; i0 < fMuons->GetEntriesFast(); i0++) if(passMuonTightSel((TMuon*)fMuons->At(i0))) lTCount++; 
  fNMuonsTight = lTCount; // tight muons multiplicty
  std::vector<TMuon*> lVeto;  
  for  (int i0 = 0; i0 < fMuons->GetEntriesFast(); i0++) { 
    TMuon *pMuon = (TMuon*)((*fMuons)[i0]);
    if(pMuon->pt        <=  10)                      continue;
    if(fabs(pMuon->eta) >=  2.4)                     continue;
    if(passVeto(pMuon->eta,pMuon->phi,0.4,iVetoes))  continue;
    if(!passMuonLooseSel(pMuon))                     continue;
    lCount++;
    lVeto.push_back(pMuon);
    addMuon(pMuon,fSelMuons); // addObject to fSelMuons
  }

  for(unsigned int i0 = 0; i0 < lVeto.size(); i0++) addVMuon(lVeto[i0],iVetoes,MUON_MASS);
  fNMuons = lCount; // loose muons multiplicity 
  if(fVars.size() > 0) fillMuon(fN,fSelMuons,fVars); // fillobject, fSelMuons->pt,eta,phi from fVars values
  if(lCount == 0) return false; // no muons selected
  return true;
}
void MuonLoader::addDiMuon(std::string iHeader,TTree *iTree,int iN,std::vector<double> &iVals,int iBase) { 
 for(int i0 = 0; i0 < iN; i0++) { 
   std::stringstream pSPt,pSMass,pSPhi,pSY,pSQG,pSCSV;
   pSPt    << iHeader << i0 << "_pt";
   pSMass  << iHeader << i0 << "_mass";
   pSPhi   << iHeader << i0 << "_phi";
   pSY     << iHeader << i0 << "_y";
   iTree->Branch(pSPt   .str().c_str(),&iVals[iBase+0],(pSPt   .str()+"/D").c_str());
   iTree->Branch(pSMass .str().c_str(),&iVals[iBase+1],(pSMass .str()+"/D").c_str());
   iTree->Branch(pSPhi  .str().c_str(),&iVals[iBase+2],(pSPhi  .str()+"/D").c_str());
   iTree->Branch(pSY    .str().c_str(),&iVals[iBase+3],(pSY    .str()+"/D").c_str());
 }
}
double MuonLoader::fillDiMuon() {
  double lTargetMass = 91.;
  double lMass = -1;
  TLorentzVector lVec; lVec.SetPtEtaPhiM(0,0,0,0);
  for(unsigned int i0 = 0; i0 < fSelMuons.size(); i0++) {
    TLorentzVector pVec0;
    for(unsigned int i1 = 0; i1 < i0; i1++) {
      TLorentzVector pVec1;
      if(fSelMuons[i0]->pt > fSelMuons[i1]->pt){ //lep1 is the leading lepton 
	pVec0.SetPtEtaPhiM(fSelMuons[i0]->pt,fSelMuons[i0]->eta,fSelMuons[i0]->phi,MUON_MASS);
	pVec1.SetPtEtaPhiM(fSelMuons[i1]->pt,fSelMuons[i1]->eta,fSelMuons[i1]->phi,MUON_MASS); 
      }
      else{
	pVec1.SetPtEtaPhiM(fSelMuons[i0]->pt,fSelMuons[i0]->eta,fSelMuons[i0]->phi,MUON_MASS);
	pVec0.SetPtEtaPhiM(fSelMuons[i1]->pt,fSelMuons[i1]->eta,fSelMuons[i1]->phi,MUON_MASS);
      }
      if((!passMuonTightSel(fSelMuons[i0]) && fSelMuons[i0]->pt < 20) || (!passMuonTightSel(fSelMuons[i1]) && fSelMuons[i1]->pt < 20)) continue; // at least one muon must pass tight selection
      if((fSelMuons[i1]->q) * (fSelMuons[i0]->q) > 0) continue;  //  opposite charge dimuons
      double pMass = (pVec0+pVec1).M();
      if(fabs(pMass-lTargetMass) < fabs(lMass-lTargetMass) || lMass < 0) {
        lMass = pMass;
        lVec = pVec0+pVec1;
      } 
    }
  }
  int lBase = 3.*fN; 
  fVars[lBase+0] = lVec.Pt();
  fVars[lBase+1] = lMass;
  fVars[lBase+2] = lVec.Phi();
  fVars[lBase+3] = lVec.Rapidity();
  return lMass;
}
void MuonLoader::setDiMuon(TLorentzVector &iVec) { 
  int lBase = 3.*fN; 
  fVars[lBase+0] = iVec.Pt();
  fVars[lBase+1] = iVec.M();
  fVars[lBase+2] = iVec.Phi();
  fVars[lBase+3] = iVec.Eta();
}
