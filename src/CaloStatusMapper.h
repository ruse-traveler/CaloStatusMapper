///  --------------------------------------------------------------------------
/*! \file   CaloStatusMapperLinkDef.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
///  --------------------------------------------------------------------------

#ifndef CLUSTERSTATUSMAPPER_H
#define CLUSTERSTATUSMAPPER_H

// c++ utilities
#include <string>
#include <vector>
// calo base
#include <calobase/RawClusterContainer.h>
// f4a libraries
#include <fun4all/SubsysReco.h>

// forward declarations
class PHCompositeNode;
class QAHistManagerHistDef;
class TH1D;
class TH2D;



// ----------------------------------------------------------------------------
//! Options for CaloStatusMapper module
// ----------------------------------------------------------------------------
struct CaloStatusMapperConfig {

  // system options
  bool debug = true;

  // input nodes
  /* will go here */

};



// ----------------------------------------------------------------------------
//! Map Status of Calo Towers
// ----------------------------------------------------------------------------
/*! This Fun4all modules ingests calorimeter towers, and then plots
 *  the no. of towers per event and vs. eta/phi for a given status.
 */
class CaloStatusMapper : public SubsysReco {

  public:

    // ctor
    CaloStatusMapper(const std::string& name = "CaloStatusMapper");
    ~CaloStatusMapper() override;

    // setters
    void SetConfig(const CaloStatusMapperConfig& config) {m_config = config;}

    // getters
    CaloStatusMapperConfig GetConfig() {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode)          override;
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode)           override;

  private:

    // histogram accessors
    enum Calo {EMC, IHC, OHC, ZDC, SEPD};
    enum H1D  {Num, Eta, Phi};
    enum H2D  {EtaVsPhi};

    // private methods
    void BuildHistograms();
    void GrabNodes(PHCompositeNode* topNode);

    // output histograms
    std::vector<std::vector<TH1D*>> vecHist1D;
    std::vector<std::vector<TH2D*>> vecHist2D;

    // f4a members
    Fun4AllHistoManager* m_manager = NULL;

    // module configuration
    CaloStatusMapperConfig m_config;

};

#endif

// end ------------------------------------------------------------------------
