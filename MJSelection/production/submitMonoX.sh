#!/bin/bash   

sample=$1
option1=$2
option2=$3
dir="monoxbits"
eos="/store/cmst3/group/monojet/production/08/"
exec="runMonoX"
if [[ ($sample = "All") || ($sample != "All" && $sample = "QCD") ]]; then 
    ./sub.sh mc $eos QCD_HT100to200_13TeVRunIIFall15MiniAODv2_PU25nsData2015v1_76X_mcRun2_asymptotic_v12_v1  27500000  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT200to300_13TeVRunIIFall15MiniAODv2_PU25nsData2015v1_76X_mcRun2_asymptotic_v12_v1   1735000  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT300to500_13TeVRunIIFall15MiniAODv2_PU25nsData2015v1_76X_mcRun2_asymptotic_v12_v1    367000  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT500to700_13TeVRunIIFall15MiniAODv2_PU25nsData2015v1_76X_mcRun2_asymptotic_v12_v1     29370  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT700to1000_13TeV_2                                                                     6524  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT1000to1500_13TeV_2                                                                    1064  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT1500to2000_13TeV_2                                                                   121.5  $dir $exec $option1 $option2
    ./sub.sh mc $eos QCD_HT2000toInf_13TeV_2                                                                    25.42  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "WHF") ]]; then 
    ./sub.sh mcwplushf $eos WJetsToLNu_HT_100To200_13TeV                                                         1343  $dir $exec $option1 $option2
    ./sub.sh mcwplushf $eos WJetsToLNu_HT_200To400_13TeV                                                        359.6  $dir $exec $option1 $option2
    ./sub.sh mcwplushf $eos WJetsToLNu_HT_400To600_13TeV                                                        48.85  $dir $exec $option1 $option2
    ./sub.sh mcwplushf $eos WJetsToLNu_HT_600ToInf_13TeV                                                        18.91  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "WLF") ]]; then
    ./sub.sh mcwpluslf $eos WJetsToLNu_HT_100To200_13TeV                                                         1343  $dir $exec $option1 $option2
    ./sub.sh mcwpluslf $eos WJetsToLNu_HT_200To400_13TeV                                                        359.6  $dir $exec $option1 $option2
    ./sub.sh mcwpluslf $eos WJetsToLNu_HT_400To600_13TeV                                                        48.85  $dir $exec $option1 $option2
    ./sub.sh mcwpluslf $eos WJetsToLNu_HT_600ToInf_13TeV                                                        18.91  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "DYHF") ]]; then
    ./sub.sh mcdyplushf $eos DYJetsToLL_M_50_HT_100to200_13TeV                                                  148.0  $dir $exec $option1 $option2
    ./sub.sh mcdyplushf $eos DYJetsToLL_M_50_HT_200to400_13TeV                                                  40.94  $dir $exec $option1 $option2
    ./sub.sh mcdyplushf $eos DYJetsToLL_M_50_HT_400to600_13TeV                                                  5.497  $dir $exec $option1 $option2
    ./sub.sh mcdyplushf $eos DYJetsToLL_M_50_HT_600toInf_13TeV                                                  2.193  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "DYLF") ]]; then
    ./sub.sh mcdypluslf $eos DYJetsToLL_M_50_HT_100to200_13TeV                                                  148.0  $dir $exec $option1 $option2
    ./sub.sh mcdypluslf $eos DYJetsToLL_M_50_HT_200to400_13TeV                                                  40.94  $dir $exec $option1 $option2
    ./sub.sh mcdypluslf $eos DYJetsToLL_M_50_HT_400to600_13TeV                                                  5.497  $dir $exec $option1 $option2
    ./sub.sh mcdypluslf $eos DYJetsToLL_M_50_HT_600toInf_13TeV                                                  2.193  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "ZHF") ]]; then
    ./sub.sh mczplushf $eos ZJetsToNuNu_HT_100To200_13TeV_madgraph                                              280.5  $dir $exec $option1 $option2
    ./sub.sh mczplushf $eos ZJetsToNuNu_HT_200To400_13TeV_madgraph                                               77.7  $dir $exec $option1 $option2
    ./sub.sh mczplushf $eos ZJetsToNuNu_HT_400To600_13TeV_madgraph                                              10.71  $dir $exec $option1 $option2
    ./sub.sh mczplushf $eos ZJetsToNuNu_HT_600ToInf_13TeV_madgraph                                              4.098  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "ZLF") ]]; then
    ./sub.sh mczpluslf $eos ZJetsToNuNu_HT_100To200_13TeV_madgraph                                              280.5  $dir $exec $option1 $option2
    ./sub.sh mczpluslf $eos ZJetsToNuNu_HT_200To400_13TeV_madgraph                                               77.7  $dir $exec $option1 $option2
    ./sub.sh mczpluslf $eos ZJetsToNuNu_HT_400To600_13TeV_madgraph                                              10.71  $dir $exec $option1 $option2
    ./sub.sh mczpluslf $eos ZJetsToNuNu_HT_600ToInf_13TeV_madgraph                                              4.098  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "T") ]]; then 
    ./sub.sh mc $eos ST_t_channel_antitop_4f_inclusiveDecays_13TeV_powhegV2_madspin_pythia8_TuneCUETP8M1      44.0802  $dir $exec $option1 $option2
    ./sub.sh mc $eos ST_t_channel_top_4f_inclusiveDecays_13TeV_powhegV2_madspin_pythia8_TuneCUETP8M1          26.2343  $dir $exec $option1 $option2
    ./sub.sh mc $eos ST_tW_antitop_5f_inclusiveDecays_13TeV_powheg_pythia8_TuneCUETP8M1                          35.6  $dir $exec $option1 $option2
    ./sub.sh mc $eos ST_tW_top_5f_inclusiveDecays_13TeV_powheg_pythia8_TuneCUETP8M1                              35.6  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "TZ") ]]; then
    ./sub.sh mc $eos tZq_ll_4f_13TeV_amcatnlo_pythia8_TuneCUETP8M1                                             0.0758  $dir $exec $option1 $option2;
#     ./sub.sh mc $eos Spring15_a25ns_tZq_nunu_4f_MINIAOD                                                        0.1379  $dir $exec $option1 $option2;
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "TTZ") ]]; then
    ./sub.sh mc $eos TTZToLLNuNu_M_10_13TeV_amcatnlo_pythia8                                                   0.2529  $dir $exec $option1 $option2;
fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTG") ]]; then
#     ./sub.sh mc $eos Spring15_a25ns_TTGJets_MINIAOD                                                             3.697  $dir $exec $option1 $option2;
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "THQ") ]]; then
#     ./sub.sh mc $eos Spring15_a25ns_tHq_invisible_MINIAOD                                                    0.070940  $dir $exec $option1 $option2;
# fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "TT") ]]; then
    ./sub.sh mctt $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                     831.76  $dir $exec $option1 $option2;
fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TT1L") ]]; then
#     ./sub.sh mctt1l $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                   831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TT2L") ]]; then
#     ./sub.sh mctt2l $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                   831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTHAD") ]]; then
#     ./sub.sh mctthad $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                  831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTBST") ]]; then
#     ./sub.sh mcttbst $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                  831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTCOM") ]]; then
#     ./sub.sh mcttcom $eos TTJets_13TeV_amcatnloFXFX_pythia8_2                                                  831.76  $dir $exec $option1 $option2; 
# fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "WW") ]]; then
    ./sub.sh mc $eos WW_13TeV_pythia8                                                                           118.7  $dir $exec $option1 $option2; 
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "WZ") ]]; then
    ./sub.sh mc $eos WZ_13TeV_pythia8                                                                            47.2  $dir $exec $option1 $option2; 
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "ZZ") ]]; then
    ./sub.sh mc $eos ZZ_13TeV_pythia8                                                                            31.8  $dir $exec $option1 $option2; 
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "MET") ]]; then 
   ./sub.sh data $eos METRun2015D_16Dec2015_v1                                                                     1  $dir $exec $option1 $option2
fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "GHF") ]]; then 
#     ./sub.sh mcgplushf $eos Spring15_a25ns_GJets_HT-100To200_MINIAOD                                             9235  $dir $exec $option1 $option2
#     ./sub.sh mcgplushf $eos Spring15_a25ns_GJets_HT-200To400_MINIAOD                                             2298  $dir $exec $option1 $option2
#     ./sub.sh mcgplushf $eos Spring15_a25ns_GJets_HT-400To600_MINIAOD                                            277.6  $dir $exec $option1 $option2
#     ./sub.sh mcgplushf $eos Spring15_a25ns_GJets_HT-600ToInf_MINIAOD                                            93.47  $dir $exec $option1 $option2
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "GLF") ]]; then 
#     ./sub.sh mcgpluslf $eos Spring15_a25ns_GJets_HT-100To200_MINIAOD                                             9235  $dir $exec $option1 $option2
#     ./sub.sh mcgpluslf $eos Spring15_a25ns_GJets_HT-200To400_MINIAOD                                             2298  $dir $exec $option1 $option2
#     ./sub.sh mcgpluslf $eos Spring15_a25ns_GJets_HT-400To600_MINIAOD                                            277.6  $dir $exec $option1 $option2
#     ./sub.sh mcgpluslf $eos Spring15_a25ns_GJets_HT-600ToInf_MINIAOD                                            93.47  $dir $exec $option1 $option2
# fi
#if [[ ($sample = "All") || ($sample != "All" && $sample = "SinglePhoton") ]]; then
#    ./sub.sh data $eos SinglePhotonRun2015D_16Dec2015_v1                                                            1  $dir $exec $option1 $option2
#fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "MonoTop") ]]; then
    ./sub.sh mc $eos Monotop_S1_Mres_1100_Mchi_100_13TeV_madgraph_pythia8                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-1300_Mchi-100_MINIAOD                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-1500_Mchi-100_MINIAOD                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-1700_Mchi-100_MINIAOD                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-1900_Mchi-100_MINIAOD                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-2100_Mchi-100_MINIAOD                                           1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S1_Mres-900_Mchi-100_MINIAOD                                            1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-1100_MINIAOD                                                    1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-1300_MINIAOD                                                    1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-1500_MINIAOD                                                    1  $dir $exec $option1 $option2
    ./sub.sh mc $eos Monotop_S4_Mchi_300_13TeV_madgraph_pythia8                                                     1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-500_MINIAOD                                                     1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-700_MINIAOD                                                     1  $dir $exec $option1 $option2
#     ./sub.sh mc $eos Spring15_a25ns_Monotop_S4_Mchi-900_MINIAOD                                                     1  $dir $exec $option1 $option2
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "SingleElectron") ]]; then
  ./sub.sh data $eos SingleElectronRun2015D_16Dec2015_v1                                                           1  $dir $exec $option1 $option2   
fi
if [[ ($sample = "All") || ($sample != "All" && $sample = "SingleMuon") ]]; then
    ./sub.sh data $eos SingleMuonRun2015D_16Dec2015_v1                                                              1  $dir $exec $option1 $option2
fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTherwig1L") ]]; then
#     ./sub.sh mctt1l $eos Spring15_a25ns_TT_powheg_herwig_MINIAOD                                               831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTherwig2L") ]]; then
#     ./sub.sh mctt2l $eos Spring15_a25ns_TT_powheg_herwig_MINIAOD                                               831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTherwigHAD") ]]; then
#     ./sub.sh mctthad $eos Spring15_a25ns_TT_powheg_herwig_MINIAOD                                              831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTherwigBST") ]]; then
#     ./sub.sh mcttbst $eos Spring15_a25ns_TT_powheg_herwig_MINIAOD                                              831.76  $dir $exec $option1 $option2; 
# fi
# if [[ ($sample = "All") || ($sample != "All" && $sample = "TTherwigCOM") ]]; then
#     ./sub.sh mcttcom $eos Spring15_a25ns_TT_powheg_herwig_MINIAOD                                              831.76  $dir $exec $option1 $option2; 
# fi
