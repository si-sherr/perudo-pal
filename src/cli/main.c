/**
 * @file main.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief CLI application entrypoint
 *
 */

/***************************************************************
 * Includes
 **************************************************************/
#include "handler.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************
 * `main()`
 **************************************************************/
int
main (int argc, char *argv[])
{
  parse_result_s parser_result = parser (argc, argv);
  int exit_code = EXIT_FAILURE;

  if (parser_result.help_flag)
    {
      handle_usage (false);
      exit_code = EXIT_SUCCESS;
    }
  else if (parser_result.version_flag)
    {
      handle_version ();
      exit_code = EXIT_SUCCESS;
    }
  else if (!parser_result.parse_success)
    {
      handle_usage (true); // Print usage, this time to stderr
    }
  else if (!handle_game_state (&parser_result.game_state))
    {
      handle_usage (true);
    }

  exit (exit_code);
}
