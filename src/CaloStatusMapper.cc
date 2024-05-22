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
#include <utility>
// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
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

  BuildHistograms();
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

  /* TODO fill in rest */
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
//! Build histograms
// ----------------------------------------------------------------------------
void CaloStatusMapper::BuildHistograms() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::BuildHistograms() Creating histograms" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'InitOutFile()'



// ----------------------------------------------------------------------------
//! Initialize output TTree
// ----------------------------------------------------------------------------
void CaloStatusMapper::InitOutTree() {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::InitOutTree() Creating output tree" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'InitOutTree()'



// ----------------------------------------------------------------------------
//! Grab input nodes
// ----------------------------------------------------------------------------
void CaloStatusMapper::GrabNodes(PHCompositeNode* topNode) {

  // print debug message
  if (m_config.debug && (Verbosity() > 0)) {
    std::cout << "CaloStatusMapper::GrabNodes(PHCompositeNode*) Grabbing input nodes" << std::endl;
  }

  /* TODO fill in */
  return;

}  // end 'GrabNodes(PHCompositeNode*)'

// end ------------------------------------------------------------------------
