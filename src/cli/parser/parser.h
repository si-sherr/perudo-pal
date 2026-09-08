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
} parser_data_s;

/***************************************************************
 * Public Function Prototypes
 **************************************************************/

/**
 * @brief Parse `main()` CLI args into game state
 *
 */
parser_data_s parser (int argc, char *argv[]);