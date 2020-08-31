
/**
 * @file moead_test.cpp
 * @author Sayan Goswami
 * @author Utkarsh Rai
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#include <ensmallen.hpp>
#include "catch.hpp"
#include "test_function_tools.hpp"

using namespace ens;
using namespace ens::test;
using namespace std;

/**
 * Checks if low <= value <= high. Used by MOEADFonsecaFlemingTest.
 *
 * @param value The value being checked.
 * @param low The lower bound.
 * @param high The upper bound.
 * @return true if value lies in the range [low, high].
 * @return false if value does not lie in the range [low, high].
 */
bool InBounds(const double& value, const double& low, const double& high)
{
  return !(value < low) && !(high < value);
}
