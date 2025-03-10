/// ===========================================================================
/*! \file   CaloStatusMapperLinkDef.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
/// ===========================================================================

#define CLUSTERSTATUSMAPPER_CC

// module definition
#include "CaloStatusMapper.h"

// calo base
#include <calobase/TowerInfov2.h>

// calo trigger
#include <calotrigger/TriggerAnalyzer.h>

// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>

// jet background utilities
// FIXME remove once MakeQAHistNames is in Jet QA
#include <jetbackground/BeamBackgroundFilterAndQADefs.h>

// phool libraries
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/PHCompositeNode.h>

// qa utilities
#include <qautils/QAHistManagerDef.h>

// root libraries
#include <TH1.h>
#include <TH2.h>

// c++ utiilites
#include <algorithm>
#include <cassert>
#include <iostream>

// abbreviate namespace for convenience
// FIXME remove once MakeQAHistNames is in Jet QA
namespace BeamBackgroundFilter = BBFQD:



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Module constructor
// ----------------------------------------------------------------------------
CaloStatusMapper::CaloStatusMapper(const std::string& modulename)
  : SubsysReco(modulename)
  , m_moduleName(modulename)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 1))
  {
    std::cout << "CaloStatusMapper::CaloStatusMapper(const std::string&) Calling ctor" << std::endl;
  }

  // make sure node vector is empty
  m_inNodes.clear();

}  // end ctor



// ----------------------------------------------------------------------------
//! Module destructor
// ----------------------------------------------------------------------------
CaloStatusMapper::~CaloStatusMapper()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 1))
  {
    std::cout << "CaloStatusMapper::~CaloStatusMapper() Calling dtor" << std::endl;
  }
  delete m_analyzer;

}  // end dtor



// fun4all methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize module
// ----------------------------------------------------------------------------
int CaloStatusMapper::Init(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "CaloStatusMapper::Init(PHCompositeNode*) Initializing" << std::endl;
  }

  // initialize trigger analyzer
  delete m_analyzer;
  m_analyzer = new TriggerAnalyzer();

  // initialize manager/histograms
  InitHistManager();
  BuildHistograms();
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs and fills histograms
// ----------------------------------------------------------------------------
int CaloStatusMapper::process_event(PHCompositeNode* topNode)
{

  if (m_config.debug)
  {
    std::cout << "CaloStatusMapper::process_event(PHCompositeNode* topNode) Processing Event" << std::endl;
  }

  // if needed, check if selected trigger fired
  if (m_config.doTrgSelect)
  {
    m_analyzer -> decodeTriggers(topNode);
    bool hasTrigger = JetQADefs::DidTriggerFire(m_config.trgToSelect, m_analyzer);
    if (!hasTrigger)
    {
      return Fun4AllReturnCodes::EVENT_OK;
    }
  }

  // grab input nodes
  GrabNodes(topNode);

  // loop over input nodes
  for (size_t iNode = 0; iNode < m_inNodes.size(); ++iNode)
  {

    // loop over towers
    TowerInfoContainer* towers = m_inNodes[iNode];
    for (size_t iTower = 0; iTower < towers->size(); ++iTower)
    {

       // grab eta, phi indices
       const int32_t key  = towers -> encode_key(iTower);
       const int32_t iEta = towers -> getTowerEtaBin(key);
       const int32_t iPhi = towers -> getTowerPhiBin(key);

       // get status
       const auto tower  = towers -> get_tower_at_channel(iTower);
       const int  status = CaloStatusMapperDefs::GetTowerStatus(tower);
       if (status == CaloStatusMapperDefs::Stat::Unknown)
       {
         std::cout << PHWHERE << ": Warning! Tower has an unknown status!\n"
                   << "  channel = " << iTower << ", key = " << key << "\n"
                   << "  node = " << m_config.inNodeNames[iNode].first
                   << std::endl; 
         continue;
       } 

       // fill histograms accordingly
       // TODO UPDATE ACCORDINGLY
       m_hists -> Fill(iEta);
       m_hists -> Fill(iPhi);
       m_hists -> Fill(iEta, iPhi);

    }  // end tower loop
  }  // end node loop
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'process_event(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Run final calculations
// ----------------------------------------------------------------------------
int CaloStatusMapper::End(PHCompositeNode *topNode)
{

  if (m_config.debug)
  {
    std::cout << "CaloStatusMapper::End(PHCompositeNode* topNode) This is the End..." << std::endl;
  }

  // register hists and exit
  for (auto& hist : m_hists) {
    m_manager -> registerHisto(hist.second);
  }
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize histogram manager
// ----------------------------------------------------------------------------
void CaloStatusMapper::InitHistManager()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "CaloStatusMapper::InitHistManager() Initializing histogram manager" << std::endl;
  }

  m_manager = QAHistManagerDef::getHistoManager();
  if (!m_manager)
  {
    std::cerr << PHWHERE << ": PANIC! Couldn't grab histogram manager!" << std::endl;
    assert(m_manager);
  }
  return;

}  // end 'InitHistManager()'



// ----------------------------------------------------------------------------
//! Build histograms
// ----------------------------------------------------------------------------
void CaloStatusMapper::BuildHistograms()
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "CaloStatusMapper::BuildHistograms() Creating histograms" << std::endl;
  }

  // instantiate histogram definitions
  const CaloStatusMapperDefs::EMCalHistDef emHistDef;
  const CaloStatusMapperDefs::IHCalHistDef ihHistDef;
  const CaloStatusMapperDefs::OHCalHistDef ohHistDef;

  // grab relevant status labels
  const auto& mapStatLabels = CaloStatusMapperDefs::StatLabels();

  // loop over input node names
  for (const auto& nodeName : m_config.inNodeNames)
  {

    // make status hist name
    const std::string& baseStat = "Status_" + nodeName.first;
    const std::string& nameStat = BBQFQD::MakeQAHistNames({baseStat}, m_moduleName, m_histTag);

    // create status hist
    //   - n.b. calo type doesn't matter here
    m_hists[baseStat] = emHistDef.MakeStatus1D(nameStat);

    // loop over status labels
    for (const auto& statLabel : mapStatLabels)
    {

      // set relevant bin label for status histogram
      m_hists[baseStat] -> GetXaxis() -> SetBinLabel(statLabel.first + 1, statLabel.second.data());

      // make base eta/phi hist name
      const std::string perEtaBase = MakeBaseName("NPerEta", statLabel.second, nodeName.first);
      const std::string perPhiBase = MakeBaseName("NPerPhi", statLabel.second, nodeName.first);
      const std::string phiEtaBase = MakeBaseName("PhiVsEta", staLabel.second. nodeName.first);

      // make full eta/phi hist name
      const std::string namePerEta = BBFQD::MakeQAHistNames({perEtaBase}, m_moduleName, m_histTag);
      const std::string namePerPhi = BBFQD::MakeQAHistNames({perPhiBase}, m_moduleName, m_histTag);
      const std::string namePhiEta = BBFQD::MakeQAHistNames({phiEtaBase}, m_moduleName, m_histTag);

      // make eta/phi hists
      switch (nodeName.second)
      {
        case CaloStatusMapperDefs::Calo::OHC:
          m_hists[perEtaBase] = ohHistDef.MakeEta1D(namePerEta);
          m_hists[perPhiBase] = ohHistDef.MakePhi1D(namePerPhi);
          m_hists[phiEtaBase] = ohHistDef.MakePhiEta2D(namePhiEta);
          break;
        case CaloStatusMapperDefs::Calo::IHC:
          m_hists[perEtaBase] = ihHistDef.MakeEta1D(namePerEta);
          m_hists[perPhiBase] = ihHistDef.MakePhi1D(namePerPhi);
          m_hists[phiEtaBase] = ihHistDef.MakePhiEta2D(namePhiEta);
          break;
        case CaloStatusMapperDefs::Calo::EMC:
          [[fallthrough]];
        default;
          m_hists[perEtaBase] = emHistDef.MakeEta1D(namePerEta);
          m_hists[perPhiBase] = emHistDef.MakePhi1D(namePerPhi);
          m_hists[phiEtaBase] = emHistDef.MakePhiEta2D(namePhiEta);
          break;
      }

    }  // end status loop
  }  // end node loop
  return;

}  // end 'BuildHistograms()'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void CaloStatusMapper::GrabNodes(PHCompositeNode* topNode)
{

  // print debug message
  if (m_config.debug && (Verbosity() > 0))
  {
    std::cout << "CaloStatusMapper::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  // make sure node vector is empty
  m_inNodes.clear();

  // loop over nodes to grab
  for (auto inNodeName : m_config.inNodeNames)
  {
    m_inNodes.push_back(
      findNode::getClass<TowerInfoContainer>(topNode, inNodeName.first)
    );
    if (!m_inNodes.back())
    {
      std::cerr << PHWHERE << ":" << " PANIC! Not able to grab node " << inNodeName.first << "! Aborting!" << std::endl;
      assert(m_inNodes.back());
    }
  }  // end input node name loop
  return;

}  // end 'GrabNodes(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Make base histogram name
// ----------------------------------------------------------------------------
std::string CaloStatusMapper::MakeBaseName(const std::string& base, const std::string& stat, const std::string& node)
{

  if (m_config.debug && (Verbosity() > 2))
  {
    std::cout << "CaloStatusMapper::MakeBaseName(std::string& x 3) Making base histogram name" << std::endl;
  }
  return stat + "_" + base + "_" + node;

}  // end 'MakeBaseName(std::string& x 3)'

// end ========================================================================
