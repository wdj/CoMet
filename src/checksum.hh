//-----------------------------------------------------------------------------
/*!
 * \file   checksum.hh
 * \author Wayne Joubert
 * \date   Mon Aug  7 14:47:01 EDT 2017
 * \brief  Utility to compute checksum of data in a metrics object.
 */
//-----------------------------------------------------------------------------
/*-----------------------------------------------------------------------------

Copyright 2020, UT-Battelle, LLC

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-----------------------------------------------------------------------------*/

#ifndef _gm_checksum_hh_
#define _gm_checksum_hh_

#include "env.hh"
#include "metrics.hh"

//-----------------------------------------------------------------------------

namespace CoMet {

//=============================================================================
/*! \brief Checksum class to compute exact checksum of metrics object.
 *
 *  The purpose of this class is to compute an exact "checksum" or hash
 *  function on the values of a metrics obect, to compare whether different
 *  runs give identical results.  It is not a perfect cryptographic hash but
 *  should identify differences between runs reasonably well.
 *
 *  The checksum value is presented as several 64-bit integers that are
 *  output as a string.  There is a proximity aspect to the hash, in the sense
 *  that metrics objects that are close to each other but not identical
 *  will have checksums that differ in the least significant digits.
 *
 *  These are the conditions under which two metrics arrays will or will not
 *  have the same checksum:
 *  - Runs with synthetic data generated by the package should give identical
 *  checksums if the run settings are identical.  In particular, the floating
 *  point numbers in this case in fact happen to be integers, so roundoff and
 *  order of operations should not be an issue.
 *  - Run on real data with the same settings, two runs need not give the exact
 *  same checksum because of roundoff and order of operations issues.
 *  - For synthetic data, two runs with exact same problem settings and with
 *  all stages and phases computed should give identical results under changes
 *  in processor count, processor decomposition, settings of --compute_method,
 *  --tc and --num_tc_steps, and number of stages or phases.
 */

class Checksum {
public:

  //---------------------------------------------------------------------------
  /// \brief Checksum default constructor.

  Checksum(bool computing_checksum = true);

  //---------------------------------------------------------------------------
  /// \brief Manual copy of checksum entries.

  void copy(const Checksum& cksum);

  //---------------------------------------------------------------------------
   /// \brief Check whether two checksums are equal.

  bool is_equal(const Checksum& cksum2) const;

  //---------------------------------------------------------------------------
  /// \brief Print ckecksum to stdout.

  void print(GMEnv& env);

  //---------------------------------------------------------------------------
  /// \brief compute (global and local) checksum of metrics object.

  static void compute(Checksum& cksum, Checksum& cksum_local,
                      GMMetrics& metrics, GMEnv& env);
  // TODO: try to make metrics, env const here.

private:

  //---------------------------------------------------------------------------
  /// \brief Internal class to manage multiple precision integers.

  struct MultiprecInt {

    enum { SIZE = 16 };
    size_t data_[SIZE];

    MultiprecInt() {
      for (int i=0; i<SIZE; ++i) {
        data_[i] = 0;
      }
    }

  };

  //---------------------------------------------------------------------------

  enum { SIZE = 3 };

  size_t data_[SIZE];
  bool is_overflowed_;
  double value_max_;
  MultiprecInt sum_;
  double sum_d_;
  bool is_started_;
  bool computing_checksum_;

  //---------------------------------------------------------------------------
  /// \brief Checksum helper: return largest value in metrics object.

  static double metrics_max_value(GMMetrics& metrics, GMEnv& env);
  // TODO: try to make metrics, env const here.

  //---------------------------------------------------------------------------
  // Disallowed methods.

  Checksum(  const Checksum&);
  void operator=(const Checksum&);

}; // class Checksum

//=============================================================================

} // namespace CoMet

//-----------------------------------------------------------------------------

#endif // _gm_checksum_hh_

//-----------------------------------------------------------------------------