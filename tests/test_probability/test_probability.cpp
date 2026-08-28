#include <gtest/gtest.h>
#include <probability.h>

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
    ::testing::Values (ChooseTestParam{ 6, 6, 1 }, ChooseTestParam{ 6, 5, 6 },
                       ChooseTestParam{ 6, 3, 20 }, ChooseTestParam{ 6, 1, 6 },
                       ChooseTestParam{ 6, 0, 1 },
                       ChooseTestParam{ 20, 10, 184756 },
                       ChooseTestParam{ 100, 5, 75287520 }));