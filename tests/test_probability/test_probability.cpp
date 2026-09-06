#include "probability.h"
#include <gtest/gtest.h>
#include <math.h>

#define DOUBLE_COMP_DP 4U

double
round_to_dp (double input, unsigned int num_dp)
{
  double sf = pow (10.0, (double)num_dp);
  input *= sf;
  input = roundf (input);
  input /= sf;
  return input;
}

/***************************************************************
 * binomial_probability() & cumulative_binomial_probability() tests
 **************************************************************/
typedef struct
{
  int n;
  int k;
  double p;
  double expected_biprob;
  double expected_cum_biprob;
} BiProbTestParam;

class BiProbParametrisedTestFixture
    : public ::testing::TestWithParam<BiProbTestParam>
{
};

TEST_P (BiProbParametrisedTestFixture, BiProbReturnsExpectedValues)
{
  BiProbTestParam param = GetParam ();
  double actual_prob = binomial_probability (param.p, param.k, param.n);
  ASSERT_EQ (round_to_dp (actual_prob, DOUBLE_COMP_DP), param.expected_biprob);
};

TEST_P (BiProbParametrisedTestFixture, CumBiProbReturnsExpectedValues)
{
  BiProbTestParam param = GetParam ();
  double actual_prob
      = cumulative_binomial_probability (param.p, param.k, param.n);
  ASSERT_EQ (round_to_dp (actual_prob, DOUBLE_COMP_DP),
             param.expected_cum_biprob);
};

INSTANTIATE_TEST_SUITE_P (
    ProbabilityTests, BiProbParametrisedTestFixture,
    ::testing::Values (
        BiProbTestParam{ .n = 6,
                         .k = 3,
                         .p = 0.5,
                         .expected_biprob = 0.3125,
                         .expected_cum_biprob = 0.6563 },
        BiProbTestParam{ 6, 6, 0.5, 0.0156, 1.0 },     // k == n
        BiProbTestParam{ 6, 1, 0.5, 0.0938, 0.1094 },  // k == 1
        BiProbTestParam{ 6, 0, 0.5, 0.0156, 0.0156 },  // k == 0
        BiProbTestParam{ 6, 7, 0.5, 0.0, 0.0 },        // k > n
        BiProbTestParam{ 0, 0, 0.5, 1.0, 1.0 },        // n == 0
        BiProbTestParam{ 6, 3, 1.0, 0.0, 0.0 },        // p == 1
        BiProbTestParam{ 6, 6, 1.0, 1.0, 1.0 },        // p == 1, k == n
        BiProbTestParam{ 6, 3, 0.0, 0.0, 1.0 },        // p == 0
        BiProbTestParam{ 6, 0, 0.0, 1.0, 1.0 },        // p == 0, k == 0
        BiProbTestParam{ 20, 7, 0.33, 0.1811, 0.6732 } // high n
        ));

/***************************************************************
 * choose() tests
 **************************************************************/
typedef struct
{
  int n;
  int k;
  int expected_value;
} ChooseTestParam;

class ChooseParametrisedTestFixture
    : public ::testing::TestWithParam<ChooseTestParam>
{
};

TEST_P (ChooseParametrisedTestFixture, ChooseReturnsExpectedValues)
{
  ChooseTestParam param = GetParam ();
  int actual_value = choose (param.n, param.k);
  ASSERT_EQ (actual_value, param.expected_value);
};

INSTANTIATE_TEST_SUITE_P (
    ProbabilityTests, ChooseParametrisedTestFixture,
    ::testing::Values (
        ChooseTestParam{ .n = 6, .k = 6, .expected_value = 1 }, // k == n
        ChooseTestParam{ 6, 5, 6 },                             // k == n - 1
        ChooseTestParam{ 6, 3, 20 },                            //
        ChooseTestParam{ 6, 1, 6 },                             // k == 1
        ChooseTestParam{ 6, 0, 1 },                             // k == 0
        ChooseTestParam{ 20, 10, 184756 },                      // larger n
        ChooseTestParam{ 100, 5, 75287520 }                     // very large n
        ));