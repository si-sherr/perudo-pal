/**
 * @file parser.h
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Header file for CLI parser
 *
 */

#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

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

/**
 * @brief Parse the command line arguments from `main()`
 *
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return parse_result_s Result struct, containing parsed values (if
 * successful)
 */
parse_result_s parser (int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif // PARSER_H