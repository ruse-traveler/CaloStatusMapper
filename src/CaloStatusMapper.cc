///  --------------------------------------------------------------------------
/*! \file   CaloStatusMapperLinkDef.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
///  --------------------------------------------------------------------------

#define CLUSTERSTATUSMAPPER_CC

// c++ utiilites
#include <algorithm>
#include <cassert>
#include <iostream>
// calo base
#include <calobase/TowerInfov2.h>
// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
// phool libraries
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/PHCompositeNode.h>
// qa utilities
#include <qautils/QAHistManagerDef.h>
// root libraries
#include <TH1.h>
#include <TH2.h>

// module definition
#include "CaloStatusMapper.h"



// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Module constructor
// ----------------------------------------------------------------------------
CaloStatusMapper::CaloStatusMapper(const std::string &name) : SubsysReco(name) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::CaloStatusMapper(const std::string &name) Calling ctor" << std::endl;
  }

  // make sure node vector is empty
  m_inNodes.clear();

}  // end ctor



// ----------------------------------------------------------------------------
//! Module destructor
// ----------------------------------------------------------------------------
CaloStatusMapper::~CaloStatusMapper() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::~CaloStatusMapper() Calling dtor" << std::endl;
  }

  /* nothing to do */

}  // end dtor



// fun4all methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize module
// ----------------------------------------------------------------------------
int CaloStatusMapper::Init(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "CaloStatusMapper::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  }

  InitHistManager();
  BuildHistograms();
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Get ready for a new run
// ----------------------------------------------------------------------------
int CaloStatusMapper::InitRun(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "CaloStatusMapper::InitRun(PHCompositeNode *topNode) Preparing for new run" << std::endl;
  }

  /* TODO fill in */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'Init(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Grab inputs and fills histograms
// ----------------------------------------------------------------------------
int CaloStatusMapper::process_event(PHCompositeNode* topNode) {

  if (m_config.debug) {
    std::cout << "CaloStatusMapper::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  }

  // grab input nodes
  GrabNodes(topNode);

  // loop over input nodes
  for (TowerInfoContainer* towers : m_inNodes) {

    uint32_t iEtaBig = 0;
    uint32_t iPhiBig = 0;

    // loop over towers
    for (size_t iTower = 0; iTower < towers -> size(); ++iTower) {

       // grab eta, phi indices
       const uint32_t key  = towers -> encode_key(iTower);
       const uint32_t iEta = towers -> getTowerEtaBin(key);
       const uint32_t iPhi = towers -> getTowerPhiBin(key);

       if (iEta > iEtaBig) iEtaBig = iEta;
       if (iPhi > iPhiBig) iPhiBig = iPhi;

       // get type 
 

    }  // end tower loop
  }  // end node loop
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'process_event(PHCompositeNode*)'



// ----------------------------------------------------------------------------
//! Run final calculations
// ----------------------------------------------------------------------------
int CaloStatusMapper::End(PHCompositeNode *topNode) {

  if (m_config.debug) {
    std::cout << "CaloStatusMapper::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  }

  /* TODO fill in */
  return Fun4AllReturnCodes::EVENT_OK;

}  // end 'End(PHCompositeNode*)'



// private methods ============================================================

// ----------------------------------------------------------------------------
//! Initialize histogram manager
// ----------------------------------------------------------------------------
void CaloStatusMapper::InitHistManager() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::InitHistManager() Initializing histogram manager" << std::endl;
  }

  m_manager = QAHistManagerDef::getHistoManager();
  if (!m_manager) {
    std::cerr << PHWHERE << ": PANIC! Couldn't grab histogram manager!" << std::endl;
    assert(m_manager);
  }
  return;

}  // end 'InitHistManager()'



// ----------------------------------------------------------------------------
//! Build histograms
// ----------------------------------------------------------------------------
void CaloStatusMapper::BuildHistograms() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::BuildHistograms() Creating histograms" << std::endl;
  }

  // grab relevant labels
  const auto& vecStatLabels = CaloStatusMapperDefs::StatLabels();
  const auto& vecH1DLabels  = CaloStatusMapperDefs::H1DLabels();
  const auto& vecH2DLabels  = CaloStatusMapperDefs::H2DLabels();

  // loop over input node names
  m_vecHist1D.resize( m_config.inNodeNames.size() );
  m_vecHist2D.resize( m_config.inNodeNames.size() );
  for (size_t iNode = 0; iNode < m_config.inNodeNames.size(); ++iNode) {

    // loop over status labels
    m_vecHist1D[iNode].resize( vecStatLabels.size() );
    m_vecHist2D[iNode].resize( vecStatLabels.size() );
    for (size_t iStatus = 0; iStatus < vecStatLabels.size(); ++iStatus) {

      // make 1d histograms
      for (const std::string& h1d : vecH1DLabels) {

        // construct name
        std::string histName = "h" + vecStatLabels[iStatus] + h1d + "_";
        histName += m_config.inNodeNames[iNode].first;

        // grab binning for histogram
        const auto& numBinDefs = CaloStatusMapperDefs::NumBins().at( m_config.inNodeNames[iNode].second );

        // create histogram
        m_vecHist1D[iNode][iStatus].push_back(
          new TH1D(
            histName.data(),
            "",  // TODO add axis labels
            std::get<0>(numBinDefs),
            std::get<1>(numBinDefs),
            std::get<2>(numBinDefs)
          )
        );

        // if not null, register histogram with manager
        if (!m_vecHist1D[iNode][iStatus].back()) {
          std::cerr << PHWHERE << ": PANIC! Not able to make histogram " << histName << "! Aborting!" << std::endl;
          assert(m_vecHist1D[iNode][iStatus].back());
        } else {
          m_manager -> registerHisto( m_vecHist1D[iNode][iStatus].back() );
        }
      }  // end 1d histogram loop

      // make 2d histograms
      for (const std::string& h2d : vecH2DLabels) {

        // construct name
        std::string histName = "h" + vecStatLabels[iStatus] + h2d + "_";
        histName += m_config.inNodeNames[iNode].first;

        // grab binning for histogram
        const auto& etaBinDefs = CaloStatusMapperDefs::EtaBins().at( m_config.inNodeNames[iNode].second );
        const auto& phiBinDefs = CaloStatusMapperDefs::PhiBins().at( m_config.inNodeNames[iNode].second );


        // create histogram
        m_vecHist2D[iNode][iStatus].push_back(
          new TH2D(
            histName.data(),
            "",  // TODO add axis labels
            std::get<0>(phiBinDefs),
            std::get<1>(phiBinDefs),
            std::get<2>(phiBinDefs),
            std::get<0>(etaBinDefs),
            std::get<1>(etaBinDefs),
            std::get<2>(etaBinDefs)
          )
        );

        // if not null, register histogram with manager
        if (!m_vecHist2D[iNode][iStatus].back()) {
          std::cerr << PHWHERE << ": PANIC! Not able to make histogram " << histName << "! Aborting!" << std::endl;
          assert(m_vecHist2D[iNode][iStatus].back());
        } else {
          m_manager -> registerHisto( m_vecHist2D[iNode][iStatus].back() );
        }
      }  // end 2d histogram loop
    }  // end status loop
  }  // end node loop
  return;

}  // end 'BuildHistograms()'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void CaloStatusMapper::GrabNodes(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  // make sure node vector is empty
  m_inNodes.clear();

  // loop over nodes to grab
  for (auto inNodeName : m_config.inNodeNames) {
    m_inNodes.push_back(
      findNode::getClass<TowerInfoContainer>(topNode, inNodeName.first)
    );
    if (!m_inNodes.back()) {
      std::cerr << PHWHERE << ":" << " PANIC! Not able to grab node " << inNodeName.first << "! Aborting!" << std::endl;
      assert(m_inNodes.back());
    }
  }  // end input node name loop
  return;

}  // end 'GrabNodes(PHCompositeNode*)'

// end ------------------------------------------------------------------------
