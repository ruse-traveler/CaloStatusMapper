///  --------------------------------------------------------------------------
/*! \file   CaloStatusMapperDefs.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
///  --------------------------------------------------------------------------

#ifndef CLUSTERSTATUSMAPPER_DEFS_H
#define CLUSTERSTATUSMAPPER_DEFS_H

// c++ utilities
#include <map>
#include <string>
#include <utility>
#include <vector>

// forward declarations
class TH1D;
class TH2D;



// ----------------------------------------------------------------------------
//! Miscellaneous definitions for CaloStatusMapper
// ----------------------------------------------------------------------------
/*! A namespace to collect various constants, helper
 *  methods, etc. used in the CaloStatusMapper
 *  module.
 */
namespace CaloStatusMapperDefs {

  // convenience types --------------------------------------------------------

  typedef std::pair<std::string, int> NodeDef;
  typedef std::tuple<uint32_t, float, float> BinDef;
  typedef std::vector<std::vector<std::vector<TH1D*>>> H1DVec;
  typedef std::vector<std::vector<std::vector<TH2D*>>> H2DVec;



  // enums --------------------------------------------------------------------

  enum Calo {EMC, IHC, OHC, ZDC, SEPD};
  enum Stat {Good, Hot, BadTime, BadChi, NotInstr, NoCalib};
  enum H1D  {Num, Eta, Phi};
  enum H2D  {EtaVsPhi};



  // constants ----------------------------------------------------------------

  inline uint32_t const& NEMCTow() {
    static uint32_t nEMCTow = 24576;
    return nEMCTow;
  }

  inline uint32_t const& NIHCTow() {
    static uint32_t nIHCTow = 1536;
    return nIHCTow;
  }

  inline uint32_t const& NOHCTow() {
    static uint32_t nOHCTow = 1536;
    return nOHCTow;
  }

  inline uint32_t const& NZDCTow() {
    static uint32_t nZDCTow = 3;
    return nZDCTow;
  }

  inline uint32_t const& NSEPDTow() {
    static uint32_t nSEPDTow = 744;
    return nSEPDTow;
  }



  // histogram definitions ----------------------------------------------------

  // status labels
  inline std::vector<std::string>& StatLabels() {
    static std::vector<std::string> statLabels = {
      "Good",
      "Bad",
      "Hot",
      "BadTime",
      "NotIntsr",
      "NoCalib"
    };
    return statLabels;
  }

  // 1D histogram labels
  inline std::vector<std::string>& H1DLabels() {
    static std::vector<std::string> labelsH1D = {
      "NumberPerEvt",
      "NumberPerEta",
      "NumberPerPhi"
    };
    return labelsH1D;
  }

  // 2D histogram labels
  inline std::vector<std::string>& H2DLabels() {
    static std::vector<std::string> labelsH2D = {
      "EtaVsPhi"
    };
    return labelsH2D;
  }

  // binning definitions
  inline std::vector<BinDef>& NumBins() {
    static std::vector<BinDef> numBins = {
      std::make_tuple(NEMCTow(), -0.5,  NEMCTow() - 0.5),
      std::make_tuple(NIHCTow(), -0.5,  NIHCTow() - 0.5),
      std::make_tuple(NOHCTow(), -0.5,  NOHCTow() - 0.5),
      std::make_tuple(NZDCTow(), -0.5,  NZDCTow() - 0.5),
      std::make_tuple(NSEPDTow(), -0.5, NSEPDTow() - 0.5)
    };
    return numBins;
  }

}  // end CaloStatusMapperDefs namespace

#endif

// end ------------------------------------------------------------------------

