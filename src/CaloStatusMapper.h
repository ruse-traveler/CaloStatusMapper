/// ===========================================================================
/*! \file   CaloStatusMapper.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
/// ===========================================================================

#ifndef CLUSTERSTATUSMAPPER_H
#define CLUSTERSTATUSMAPPER_H

// jet qa
// FIXME change to local include when ready to merge
#include <jetqa/JetQADefs.h>

// module definitions
#include "CaloStatusMapperDefs.h"

// calo base
#include <calobase/TowerInfoContainerv2.h>

// f4a libraries
#include <fun4all/SubsysReco.h>

// c++ utilities
#include <string>
#include <vector>

// forward declarations
class PHCompositeNode;
class Fun4AllHistoManager;
class TriggerAnalyzer;



// ============================================================================
//! Map Status of Calo Towers
// ============================================================================
/*! This Fun4all modules ingests calorimeter towers, and then plots
 *  the no. of towers per event and vs. eta/phi for a given status.
 */
class CaloStatusMapper : public SubsysReco
{

  public:

    // ========================================================================
    //! Options for CaloStatusMapper module
    // ========================================================================
    struct Config
    {

      ///! turn debug messages on/off
      bool debug {true};

      ///! input nodes and what type of calo they are
      std::vector<CaloStatusMapperDefs::NodeDef> inNodeNames
      {
        {"TOWERINFO_CALIB_CEMC",    CaloStatusMapperDefs::Calo::EMC},
        {"TOWERINFO_CALIB_HCALIN",  CaloStatusMapperDefs::Calo::IHC},
        {"TOWERINFO_CALIB_HCALOUT", CaloStatusMapperDefs::Calo::OHC}
      };

     ///! turn trigger selection on/off
     bool doTrgSelect {false};

     ///! trigger to select
     uint32_t trgToSelect {JetQADefs::GL1::MBDNSJet1};

    };  // end Config

    // ctor/dtor
    CaloStatusMapper(const std::string& modulename = "CaloStatusMapper");
    ~CaloStatusMapper() override;

    // setters
    void SetConfig(const Config& config) {m_config = config;}
    void SetHistTag(const std::string& tag) {m_histTag = tag;}

    // getters
    Config GetConfig() {return m_config;}

    // f4a methods
    int Init(PHCompositeNode* topNode) override;
    int process_event(PHCompositeNode* topNode) override;
    int End(PHCompositeNode* topNode) override;

  private:

    // private methods
    void InitHistManager();
    void BuildHistograms();
    void GrabNodes(PHCompositeNode* topNode);

    // output histograms
    CaloStatusMapperDefs::H1DVec m_vecHist1D;
    CaloStatusMapperDefs::H2DVec m_vecHist2D;

    // f4a members
    Fun4AllHistoManager* m_manager {nullptr};
    TriggerAnalyzer* m_analyzer {nullptr};

    // module name and hist tag
    std::string m_moduleName;
    std::string m_histTag {"AllTrig"};

    // input nodes
    std::vector<TowerInfoContainer*> m_inNodes;

    // module configuration
    Config m_config;

};  // end CaloStatusMapper

#endif

// end ========================================================================
