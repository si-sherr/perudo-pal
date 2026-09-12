#include "parser.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

const char *
convert (const std::string &s)
{
  return s.c_str ();
}

parse_result_s
invoke_parser (std::string cmdString)
{
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

TEST (ParserTests, ParsesTotalDiceArgSuccessfully)
{
  std::string cmdString = "ppal -t 30 -d 1,2,3,4,5,6 -b 8,6";
  parse_result_s result = invoke_parser (cmdString);

  ASSERT_TRUE (result.parse_success);
  ASSERT_EQ (result.game_state.n_total_dice, 30);
}