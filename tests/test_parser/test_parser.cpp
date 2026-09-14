#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

void
ASSERT_HAND_EQUAL (unsigned int *hand1, unsigned int *hand2, size_t hand_size)
{
  for (size_t i = 0; i < hand_size; i++)
    {
      ASSERT_EQ (hand1[i], hand2[i]);
    }
}

const char *
convert (const std::string &s)
{
  return s.c_str ();
}

parse_result_s
invoke_parser (std::string cmdString)
{
  cmdString = "ppal " + cmdString;
  std::stringstream ss (cmdString);
  std::vector<std::string> strings;
  std::vector<const char *> cstrings;

  std::string token;
  while (ss >> token)
    {
      strings.push_back (token);
    }

  std::transform (strings.begin (), strings.end (),
                  std::back_inserter (cstrings), convert);

  return parser (cstrings.size (), (char **)&cstrings[0]);
}

/***************************************************************
 * Test parsing of all args
 **************************************************************/
TEST (ParserTests, ParsesValidArgsSuccesfully)
{
  std::string args = "-t 30 -d 1,2,3,4,5,6 -b 8,6 -f 5 -p";
  unsigned int expected_hand_array[] = { 1, 2, 3, 4, 5, 6 };
  parse_result_s result = invoke_parser (args);

  ASSERT_TRUE (result.parse_success);
  ASSERT_EQ (result.game_state.n_total_dice, 30);
  ASSERT_EQ (result.game_state.hand_size, 6);
  ASSERT_HAND_EQUAL (result.game_state.hand_array, expected_hand_array, 6);
  ASSERT_EQ (result.game_state.current_bid.quantity, 8);
  ASSERT_EQ (result.game_state.current_bid.value, 6);
  ASSERT_EQ (result.game_state.n_faces, 5);
  ASSERT_TRUE (result.game_state.palefico_flag);
}

/***************************************************************
 * Fixtures for general arg validity tests
 **************************************************************/
typedef struct
{
  std::string testArg;
  bool expectedValid;
} ArgValidTestParam;

class ArgValidParametrisedTestFixture
    : public ::testing::TestWithParam<ArgValidTestParam>
{
protected:
  ArgValidTestParam param;
  std::string argsToTest;

  void
  TestArgValidity (void)
  {
    ArgValidTestParam param = GetParam ();
    parse_result_s result = invoke_parser (argsToTest);
    ASSERT_EQ (result.parse_success, param.expectedValid);
  }
};

/***************************************************************
 * Tests for -t arg
 **************************************************************/
class TotalDiceArgValidParametrisedTestFixture
    : public ArgValidParametrisedTestFixture
{
  void
  SetUp ()
  {
    param = GetParam ();
    argsToTest = "-b 1,1 -d 1 " + param.testArg;
  }
};

TEST_P (TotalDiceArgValidParametrisedTestFixture,
        TotalDiceArgReturnsExpectedParseSuccess)
{
  TestArgValidity ();
}

INSTANTIATE_TEST_SUITE_P (
    ParserTests, TotalDiceArgValidParametrisedTestFixture,
    ::testing::Values (ArgValidTestParam{ "-t 30", true },
                       ArgValidTestParam{ "-t 30d", false },
                       ArgValidTestParam{ "-t xx", false },
                       ArgValidTestParam{ "-t ", false }, // Whitespace
                       ArgValidTestParam{ "-t", false },  // No argument
                       ArgValidTestParam{ "", false }     // Option not given
                       ));

/***************************************************************
 * Tests for -b arg
 **************************************************************/

class BidArgValidParametrisedTestFixture
    : public ArgValidParametrisedTestFixture
{
  void
  SetUp ()
  {
    param = GetParam ();
    argsToTest = "-t 30 -d 1 " + param.testArg;
  }
};

TEST_P (BidArgValidParametrisedTestFixture, BidArgReturnsExpectedParseSuccess)
{
  TestArgValidity ();
}

INSTANTIATE_TEST_SUITE_P (
    ParserTests, BidArgValidParametrisedTestFixture,
    ::testing::Values (ArgValidTestParam{ "-b 1,2", true },
                       ArgValidTestParam{ "-b 1,2,3", false }, // >2 args
                       ArgValidTestParam{ "-b 1", false },     // 1 arg
                       ArgValidTestParam{ "-b 1,", false },
                       ArgValidTestParam{ "-b 1,x", false }, // 1 invalid arg
                       ArgValidTestParam{ "-b x,2", false },
                       ArgValidTestParam{ "-b x,y", false }, // Both invalid
                       ArgValidTestParam{ "-b ", false },    // Whitespace
                       ArgValidTestParam{ "-b", false },     // No arg
                       ArgValidTestParam{ "", false }));     // No opt

/***************************************************************
 * Tests for -d arg
 **************************************************************/

class HandArgValidParametrisedTestFixture
    : public ArgValidParametrisedTestFixture
{
  void
  SetUp ()
  {
    param = GetParam ();
    argsToTest = "-t 30 -b 1,1 " + param.testArg;
  }
};

TEST_P (HandArgValidParametrisedTestFixture,
        HandArgReturnsExpectedParseSuccess)
{
  TestArgValidity ();
};

INSTANTIATE_TEST_SUITE_P (
    ParserTests, HandArgValidParametrisedTestFixture,
    ::testing::Values (ArgValidTestParam{ "-d 1,2,3,4,5,6", true },
                       ArgValidTestParam{ "-d 1,2,3,x", false },
                       ArgValidTestParam{ "-d 1,2,3x", false },
                       ArgValidTestParam{ "-d asdf", false },
                       ArgValidTestParam{ "-d x,2,3,4", false },
                       ArgValidTestParam{ "-d 1,2,x,3,4", false },
                       ArgValidTestParam{ "-d ", false },
                       ArgValidTestParam{ "-d", false },
                       ArgValidTestParam{ "", false }));
