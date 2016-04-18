//================================================================================================
//
// Perform preselection for di-jet events and produce bacon bits 
//
// Input arguments
//   argv[1] => lName = input bacon file name
//   argv[2] => lOption = dataset type: "mc",  **CHECK
//                            "mcwjets",  "mcwplushf",  "mcwpluslf",
//                            "mczjets",  "mczplushf",  "mczpluslf",
//                            "mcdyjets", "mcdyplushf", "mcdypluslf",
//                            "mcgjets",  "mcgplushf",  "mcgpluslf",
//                            "mcttbst",  "mcttcom",
//                            "mctt1l",   "mctt2l",     "mctthad",
//                            "data"
//   argv[3] => lJSON = JSON file for run-lumi filtering of data, specify "none" for MC or no filtering
//   argv[4] => lXS = cross section (pb), ignored for data 
//   argv[5] => weight = total weight, ignored for data
//________________________________________________________________________________________________

#include "../include/GenLoader.hh"
#include "../include/EvtLoader.hh"
#include "../include/ElectronLoader.hh"
#include "../include/MuonLoader.hh"
#include "../include/PhotonLoader.hh"
#include "../include/TauLoader.hh"
#include "../include/JetLoader.hh"
#include "../include/VJetLoader.hh"
#include "../include/RunLumiRangeMap.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include <iostream>

// Object Processors
GenLoader       *fGen      = 0; 
EvtLoader       *fEvt      = 0; 
MuonLoader      *fMuon     = 0; 
ElectronLoader  *fElectron = 0; 
TauLoader       *fTau      = 0; 
PhotonLoader    *fPhoton   = 0; 
JetLoader       *fJet      = 0; 
RunLumiRangeMap *fRangeMap = 0; 

// **NOTE** 
// For other types of jets e.g: 
// VJetLoader      *fVJet15CHS     = 0;
// fVJet15CHS    = new VJetLoader    (lTree,"CA15CHS","AddCA15CHS");
// fVJet15CHS   ->setupTree      (lOut,"bst15_CHSjet");

TH1F *fHist                = 0; 

// Load tree and return infile
TTree* load(std::string iName) { 
  TFile *lFile = TFile::Open(iName.c_str());
  TTree *lTree = (TTree*) lFile->FindObjectAny("Events");
  fHist        = (TH1F* ) lFile->FindObjectAny("TotalEvents");
  return lTree;
}

// For Json 
bool passEvent(unsigned int iRun,unsigned int iLumi) {
  RunLumiRangeMap::RunLumiPairType lRunLumi(iRun,iLumi);
  return fRangeMap->HasRunLumi(lRunLumi);
}

int main( int argc, char **argv ) {
  gROOT->ProcessLine("#include <vector>");
  const std::string lName        = argv[1];
  const std::string lOption      = argv[2];
  const std::string lJSON        = argv[3];
  const double      lXS          = atof(argv[4]);
  const double      weight       = atof(argv[5]);

  fRangeMap = new RunLumiRangeMap();
  if(lJSON.size() > 0) fRangeMap->AddJSONFile(lJSON.c_str());

  TTree *lTree = load(lName); 
  
  // Declare Readers 
  fEvt      = new EvtLoader     (lTree,lName);                                           // fEvt, fEvtBr, fVertices, fVertexBr
  fMuon     = new MuonLoader    (lTree);                                                 // fMuon and fMuonBr, fN = 2 - muonArr and muonBr
  fElectron = new ElectronLoader(lTree);                                                 // fElectrons and fElectronBr, fN = 2
  fTau      = new TauLoader     (lTree);                                                 // fTaus and fTaurBr, fN = 1
  fPhoton   = new PhotonLoader  (lTree);                                                 // fPhotons and fPhotonBr, fN = 1
  fJet      = new JetLoader     (lTree);                                                 // fJets and fJetBr => AK4PUPPI, fN = 4 - includes jet corrections (corrParams), fN = 4
  if(lOption.find("data")==std::string::npos) fGen      = new GenLoader     (lTree);     // fGenInfo, fGenInfoBr => GenEvtInfo, fGens and fGenBr => GenParticle

  TFile *lFile = new TFile("Output.root","RECREATE");
  TTree *lOut  = new TTree("Events","Events");

  // Setup Tree
  fEvt     ->setupTree      (lOut); 
  fMuon    ->setupTree      (lOut);
  fElectron->setupTree      (lOut);
  fTau     ->setupTree      (lOut);
  fPhoton  ->setupTree      (lOut);
  fJet     ->setupTree      (lOut,"res_PUPPIjet");
  fJet     ->setupTreeDiJet (lOut,"res_PUPPIjet"); 
  fJet     ->setupTreeBTag  (lOut,"res_PUPPIjet");
  if(lOption.find("data")==std::string::npos) fGen ->setupTree (lOut,float(lXS));

  //
  // Loop over events i0 = iEvent
  //
  int neventstest = 0;
  for(int i0 = 0; i0 < int(lTree->GetEntriesFast()); i0++) {
  //for(int i0 = 0; i0 < int(100000); i0++){ // for testing
    if(i0 % 100000 == 0) std::cout << "===> Processed " << i0 << " - Done : " << (float(i0)/float(lTree->GetEntriesFast())*100) << " -- " << lOption << std::endl;
    
    // Check JSON and GenInfo
    fEvt->load(i0);
    if(lOption.find("data")!=std::string::npos){
      if(!passEvent(fEvt->fRun,fEvt->fLumi))                                                              continue;
    }
    else{
      fGen->load(i0);
      fEvt->fScale = (float(lXS)*1000.*fGen->fWeight)/weight;
      if(lOption.find("hf")!=std::string::npos && !(fGen->isGenParticle(4)) && !(fGen->isGenParticle(5))) continue;
      if(lOption.find("lf")!=std::string::npos && ((fGen->isGenParticle(4)) || (fGen->isGenParticle(5)))) continue;
      if(lOption.find("tt")!=std::string::npos){
	int nlep = fGen->isttbarType();
	if(lOption.find("tt2l")!=std::string::npos && nlep!=2)                                            continue;
	if(lOption.find("tt1l")!=std::string::npos && nlep!=1)                                            continue;
	if(lOption.find("tthad")!=std::string::npos && nlep!=0)                                           continue;
	if(lOption.find("ttbst")!=std::string::npos && nlep!=2 && nlep!=1 && nlep!=0)                     continue;
	if(lOption.find("ttcom")!=std::string::npos && nlep!=2 && nlep!=1 && nlep!=0)                     continue;
      }
    }

    // Primary vertex requirement
    if(!fEvt->PV()) continue;
    
    // Triggerbits for MET, Electrons, Photons and Muons
    unsigned int trigbits=1;    
    if(fEvt ->passTrigger("HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight_v*") ||
       fEvt ->passTrigger("HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v*") ||
       fEvt ->passTrigger("HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_NoID_v*") ||
       fEvt ->passTrigger("HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight_v*") ||
       fEvt ->passTrigger("HLT_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v*") ||
       fEvt ->passTrigger("HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_NoID_v*")) trigbits = trigbits | 2; 
    if(fEvt ->passTrigger("HLT_Ele27_WP85_Gsf_v*") ||
       fEvt ->passTrigger("HLT_Ele27_WPLoose_Gsf_v*") ||
       fEvt ->passTrigger("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v*") || 
       fEvt ->passTrigger("HLT_Ele23_WPLoose_Gsf_v*")) trigbits= trigbits | 4;
    if(fEvt ->passTrigger("HLT_Photon175_v*") ||
       fEvt ->passTrigger("HLT_Photon165_HE10_v*")) trigbits = trigbits | 8;
    if(trigbits==1) continue;
    
    // Objects
    std::vector<TLorentzVector> lMuons, lElectrons, lPhotons, lJets, lVJet, lVJets, lVetoes;

    // Muons
    fMuon     ->load(i0);
    fMuon     ->selectMuons(lMuons);
    
    // Event info
    fEvt      ->load(i0);
    fEvt      ->fillEvent(trigbits);
    
    // Electrons
    fElectron ->load(i0);
    fElectron ->selectElectrons(fEvt->fRho,lElectrons);
    
    // Lepton SF
    if(lOption.find("data")==std::string::npos){
      fillLepSF(fMuon->fNMuons,lMuons,fMuon->fhMuTight,fMuon->fhMuLoose,fGen->lepmatched(13,lMuons,0.3),fMuon->fmuoSFVars);
      fillLepSF(fElectron->fNElectrons,lElectrons,fElectron->fhEleTight,fElectron->fhEleVeto,fGen->lepmatched(11,lElectrons,0.3),fElectron->feleSFVars);
    }
    
    // Fill Vetoes
    fEvt     ->fillVetoes(lElectrons,lVetoes);
    fEvt     ->fillVetoes(lMuons,lVetoes);

    // Taus
    fTau     ->load(i0);
    fTau     ->selectTaus(lVetoes);

    // Photons
    fPhoton  ->load(i0);
    fPhoton  ->selectPhotons(fEvt->fRho,lElectrons,lPhotons);
    
    // MET selection
    fEvt->fillModifiedMet(lVetoes,lPhotons);
    if(fEvt->fMet < 200. && fEvt->fPuppEt < 200. && fEvt->fFPuppEt < 200. && fEvt->fFMet < 200.) continue;

    // Trigger Efficiencies
    fEvt->triggerEff(lElectrons, lPhotons);
        
    // AK4Puppi Jets
    fJet->load(i0); 
    fJet->selectJets(lVetoes,lVJets,lJets,fEvt->fPuppEtPhi,fEvt->fFPuppEt,fEvt->fFPuppEtPhi);
    if(lJets.size()>1){
      fEvt->fselectBits = fEvt->fselectBits | 2;
      fEvt->fillmT(lJets,fJet->fMT);
    }

    // Select at least 2 narrow Jets
    if(!(fEvt->fselectBits & 2)) continue;

    // ttbar, EWK and kFactor correction
    if(lOption.find("data")==std::string::npos){
      fGen->load(i0);
      if(lJets.size()>0)        fJet->fillBTag(fJet->fGoodJets);
    }

    if(lOption.find("mcg")!=std::string::npos){
      fGen->findBoson(22,0); 
      if(fGen->fBosonPt>0)      fEvt->computeCorr(fGen->fBosonPt,"GJets_1j_NLO/nominal_G","GJets_LO/inv_pt_G","EWKcorr/photon");
    }
    if(lOption.find("mcz")!=std::string::npos || lOption.find("mcdy")!=std::string::npos){
      fGen->findBoson(23,1);
      if(fGen->fBosonPt>0)      fEvt->computeCorr(fGen->fBosonPt,"ZJets_012j_NLO/nominal","ZJets_LO/inv_pt","EWKcorr/Z");
    }
    if(lOption.find("mcw")!=std::string::npos){
      fGen->findBoson(24,1);
      if(fGen->fBosonPt>0)      fEvt->computeCorr(fGen->fBosonPt,"WJets_012j_NLO/nominal","WJets_LO/inv_pt","EWKcorr/W"); 
    }
    if(lOption.find("tt")!=std::string::npos){
      fEvt->fevtWeight *= fGen->computeTTbarCorr();
    }
    
    lOut->Fill();
    neventstest++;
  }
  std::cout << neventstest << std::endl;
  std::cout << lTree->GetEntriesFast() << std::endl;
  lFile->cd();
  lOut->Write();
  lFile->Close();
}