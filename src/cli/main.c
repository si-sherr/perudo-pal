/**
 * @file main.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief CLI application entrypoint
 *
 */

/***************************************************************
 * Includes
 **************************************************************/
#include "parser.h"
#include <stdio.h>

/***************************************************************
 * `main()`
 **************************************************************/
int
main (int argc, char *argv[])
{
  parse_result_s parser_result = parser (argc, argv);

  if (parser_result.parse_success)
    {
      if (parser_result.help_flag)
        {
          printf ("help flag\n");
        }

      if (parser_result.version_flag)
        {
          printf ("version flag\n");
        }

      printf ("current_bid:\n quantity: %u\n value: %u\n",
              parser_result.game_state.current_bid.quantity,
              parser_result.game_state.current_bid.value);

      printf ("n_total_dice: %u\n", parser_result.game_state.n_total_dice);

      printf ("n_faces: %u\n", parser_result.game_state.n_faces);

      printf ("hand:\n");

      for (size_t i = 0; i < parser_result.game_state.hand_size; i++)
        {
          printf ("  %u\n", parser_result.game_state.hand_array[i]);
        }

      if (parser_result.game_state.palefico_flag)
        {
          printf ("palefico_flag\n");
        }
    }
}
