/**
 * @file parser.h
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief CLI parser header file
 *
 */

/***************************************************************
 * Includes
 **************************************************************/
#include "game_engine.h"

/***************************************************************
 * Types
 **************************************************************/
typedef struct
{
  bool parse_success;
  bool version_flag;
  bool help_flag;
  perudo_game_state_s game_state;
} parse_result_s;

/***************************************************************
 * Public Function Prototypes
 **************************************************************/

parse_result_s parser (int argc, char *argv[]);