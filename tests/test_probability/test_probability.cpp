#include <gtest/gtest.h>
#include <math.h>
#include <probability.h>

#define FLOAT_COMP_DP 4U

float
round_to_dp (float input, unsigned int num_dp)
{
  float sf = powf (10.0f, (float)num_dp);
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
  float p;
  float expected_biprob;
  float expected_cum_biprob;
} BiProbTestParam;

class BiProbParametrisedTestFixture
    : public ::testing::TestWithParam<BiProbTestParam>
{
};

TEST_P (BiProbParametrisedTestFixture, BiProbReturnsExpectedValues)
{
  BiProbTestParam param = GetParam ();
  float actual_prob = binomial_probability (param.p, param.k, param.n);
  ASSERT_EQ (round_to_dp (actual_prob, FLOAT_COMP_DP), param.expected_biprob);
};

TEST_P (BiProbParametrisedTestFixture, CumBiProbReturnsExpectedValues)
{
  BiProbTestParam param = GetParam ();
  float actual_prob
      = cumulative_binomial_probability (param.p, param.k, param.n);
  ASSERT_EQ (round_to_dp (actual_prob, FLOAT_COMP_DP),
             param.expected_cum_biprob);
};

INSTANTIATE_TEST_SUITE_P (
    ProbabilityTests, BiProbParametrisedTestFixture,
    ::testing::Values (
        BiProbTestParam{ 6, 3, 0.5f, 0.3125f, 0.6563f },
        BiProbTestParam{ 6, 6, 0.5f, 0.0156f, 1.0f },     // k == n
        BiProbTestParam{ 6, 1, 0.5f, 0.0938f, 0.1094f },  // k == 1
        BiProbTestParam{ 6, 0, 0.5f, 0.0156f, 0.0156f },  // k == 0
        BiProbTestParam{ 6, 7, 0.5f, 0.0f, 0.0f },        // k > n
        BiProbTestParam{ 0, 0, 0.5f, 1.0f, 1.0f },        // n == 0
        BiProbTestParam{ 6, 3, 1.0f, 0.0f, 0.0f },        // p == 1
        BiProbTestParam{ 6, 6, 1.0f, 1.0f, 1.0f },        // p == 1, k == n
        BiProbTestParam{ 6, 3, 0.0f, 0.0f, 1.0f },        // p == 0
        BiProbTestParam{ 6, 0, 0.0f, 1.0f, 1.0f },        // p == 0, k == 0
        BiProbTestParam{ 20, 7, 0.33f, 0.1811f, 0.6732f } // high n
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
    ::testing::Values (ChooseTestParam{ 6, 6, 1 }, // k == n
                       ChooseTestParam{ 6, 5, 6 }, // k == n - 1
                       ChooseTestParam{ 6, 3, 20 },
                       ChooseTestParam{ 6, 1, 6 },         // k == 1
                       ChooseTestParam{ 6, 0, 1 },         // k == 0
                       ChooseTestParam{ 20, 10, 184756 },  // larger n
                       ChooseTestParam{ 100, 5, 75287520 } // very large n
                       ));