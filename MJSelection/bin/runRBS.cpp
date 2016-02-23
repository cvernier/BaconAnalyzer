//Driver to run Bacon 
#include "../include/GenLoader.hh"
#include "../include/EvtLoader.hh"
#include "../include/ElectronLoader.hh"
#include "../include/MuonLoader.hh"
#include "../include/PhotonLoader.hh"
#include "../include/TauLoader.hh"
#include "../include/VJetLoader.hh"
#include "../include/Rebalance.hh"
#include "../include/RunLumiRangeMap.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include <iostream>

//Object Processors
GenLoader       *fGen      = 0; 
EvtLoader       *fEvt      = 0; 
MuonLoader      *fMuon     = 0; 
ElectronLoader  *fElectron = 0; 
TauLoader       *fTau      = 0; 
PhotonLoader    *fPhoton   = 0; 
VJetLoader      *fVJet     = 0; 
Rebalance       *fRebalance = 0; 
RunLumiRangeMap *fRangeMap = 0; 

TH1F *fHist                = 0; 

TTree* load(std::string iName) { 
  TFile *lFile = TFile::Open(iName.c_str());
  TTree *lTree = (TTree*) lFile->FindObjectAny("Events");
  fHist        = (TH1F* ) lFile->FindObjectAny("TotalEvents");
  return lTree;
}
bool passEvent(unsigned int iRun,unsigned int iLumi) { 
  RunLumiRangeMap::RunLumiPairType lRunLumi(iRun,iLumi);
  return fRangeMap->HasRunLumi(lRunLumi);
}
int main( int argc, char **argv ) {
  gROOT->ProcessLine("#include <vector>");          
  float maxEvents      = atof(argv[1]);
  std::string lName    = argv[2];
  int         lGen     = atoi(argv[3]);
  std::string lJSON    = argv[4];
  int         lOption  = atoi(argv[5]);
  double      lXS      = atof(argv[6]);
  fRangeMap = new RunLumiRangeMap();
  if(lJSON.size() > 0) fRangeMap->AddJSONFile(lJSON.c_str());

  TTree *lTree = load(lName); 
  float lWeight = float(lXS)/maxEvents/1000.; if(lGen == 0) lWeight = 1.;
  if(lTree->GetEntriesFast() < maxEvents || abs(maxEvents) == 1) maxEvents = float(lTree->GetEntriesFast()); 

  //Declare Readers
  fEvt        = new EvtLoader     (lTree,lName);
  fMuon       = new MuonLoader    (lTree);
  fElectron   = new ElectronLoader(lTree);
  fTau        = new TauLoader     (lTree); 
  fPhoton     = new PhotonLoader  (lTree); 
  fRebalance  = new Rebalance     (lTree,(lGen==0));
  fVJet       = new VJetLoader    (lTree);
  if(lGen == 1) fGen      = new GenLoader     (lTree);

  TFile *lFile = new TFile("Output.root","RECREATE");
  TTree *lOut  = new TTree("Tree","Tree");
  //Setup Tree
  fEvt      ->setupTree      (lOut,lWeight,false);
  //fMuon     ->setupTree      (lOut); 
  //fElectron ->setupTree      (lOut); 
  //fTau      ->setupTree      (lOut); 
  //fPhoton   ->setupTree      (lOut); 
  fRebalance->setupTree      (lOut); 
  fVJet     ->setupTree      (lOut); 
  //if(lGen == 1) fGen ->setupTree (lOut,float(lXS));
  //Add the triggers we want
  fEvt ->addTrigger("HLT_AK8PFJet360_TrimMass30_v*");
  fEvt ->addTrigger("HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v*");
  fEvt ->addTrigger("HLT_PFHT200_v*");
  fEvt ->addTrigger("HLT_PFHT250_v*");
  fEvt ->addTrigger("HLT_PFHT300_v*");
  fEvt ->addTrigger("HLT_PFHT350_v*");
  fEvt ->addTrigger("HLT_PFHT400_v*");
  fEvt ->addTrigger("HLT_PFHT475_v*");
  fEvt ->addTrigger("HLT_PFHT600_v*");
  fEvt ->addTrigger("HLT_PFHT650_v*");
  fEvt ->addTrigger("HLT_PFHT800_v*");
  fEvt ->addTrigger("HLT_PFHT900_v*");
  for(int i0 = 0; i0 < int(maxEvents); i0++) { 
    if(i0 % 1000 == 0) std::cout << "===> Processed " << i0 << " - Done : " << (float(i0)/float(maxEvents)) << " -- " << lOption << std::endl;
    //if(i0 % 5 != 0) continue; 
    //Load event and require trigger
    std::vector<TLorentzVector> lVetoes; 
    fEvt     ->load(i0);
    fEvt     ->fillEvent();
    //Lepton Vetoes
    if(lGen == 0 && !passEvent(fEvt->fRun,fEvt->fLumi))    continue;
    fMuon    ->load(i0);     
    if(fMuon    ->selectMuons(lVetoes) > 0)                continue;
    fElectron->load(i0);
    if(fElectron->selectElectrons(fEvt->fRho,lVetoes) > 0) continue;
    fTau     ->load(i0);
    if(fTau     ->selectTaus(lVetoes) > 0)                 continue;
    fPhoton  ->load(i0);
    if(fPhoton  ->selectPhotons(fEvt->fRho,lVetoes) > 0)   continue;
    //Setup
    fRebalance->load(i0); 
    fRebalance->selectJets(fEvt->fMet,fEvt->fMetPhi,lVetoes);
    fVJet->load(i0);
    fVJet->selectVJets(lVetoes);
    //if(lGen == 1) {
    //  fGen->load(i0);
    //  fGen->fillGenEvent();
    //}
    lOut->Fill();
  }
  lFile->cd();
  lOut->Write();
  lFile->Close();
}
