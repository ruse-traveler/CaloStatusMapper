///  --------------------------------------------------------------------------
/*! \file   CaloStatusMapperDefs.h
 *  \author Derek Anderson
 *  \date   05.22.2024
 *
 *  A Fun4All QA module to plot no. of towers per event
 *  and vs. eta, phi as a function of status.
 */
///  --------------------------------------------------------------------------

// c++ utilities
#include <utility>
#include <vector>

#ifndef CLUSTERSTATUSMAPPER_DEFS_H
#define CLUSTERSTATUSMAPPER_DEFS_H



// ----------------------------------------------------------------------------
//! Miscellaneous definitions for CaloStatusMapper
// ----------------------------------------------------------------------------
/*! A namespace to collect various constants, helper
 *  methods, etc. used in the CaloStatusMapper
 *  module.
 */
namespace CaloStatusMapperDefs {

  // histogram accessors
  enum Calo {EMC, IHC, OHC, ZDC, SEPD};
  enum H1D  {Num, Eta, Phi};
  enum H2D  {EtaVsPhi};

  /* TODO fill in */

}  // end CaloStatusMapperDefs namespace

#endif

// end ------------------------------------------------------------------------

