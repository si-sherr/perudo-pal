/**
 * @file handler.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Implementation file for CLI handler
 *
 */

/***************************************************************
 * Implements
 **************************************************************/
#include "handler.h"

/***************************************************************
 * Includes
 **************************************************************/
#include "ppal_version.h"
#include <stdio.h>

/***************************************************************
 * Defines
 **************************************************************/
#define USAGE                                                                 \
  "Usage:\n  ppal -t <total-dice> -b "                                        \
  "<current-bid> -d "                                                         \
  "<hand-dice>\n\nOptions:\n  [-h, --help]                 Show this "        \
  "message\n  [-v "                                                           \
  "--version] "                                                               \
  "              Show version\n  [-t, --total] <val>          Total number "  \
  "of dice in "                                                               \
  "play (required)\n  [-b, --bid] <quant,val>      Current bid, e.g. 7,3 "    \
  "(required)\n  [-d, --hand] <val1,...valX>  Dice in hand, e.g. "            \
  "1,1,3,6 (required)\n  [-p, --palefico]             Apply Palefico round "  \
  "bidding "                                                                  \
  "rules\n  [-f, --faces] <val>          Number of faces on "                 \
  "the "                                                                      \
  "dice (default=6)\n"

/***************************************************************
 * Public Function Definitions
 **************************************************************/
void
handle_usage (bool is_error)
{
  FILE *stream = is_error ? stderr : stdout;
  fprintf (stream, USAGE);
}

void
handle_version (void)
{
  printf ("%s\n", PPAL_VERSION);
}

bool
handle_game_state (const perudo_game_state_s *game_state)
{
  printf ("Current bid: %u %u's\n", game_state->current_bid.quantity,
          game_state->current_bid.value);

  printf ("Total dice: %u\n", game_state->n_total_dice);

  printf ("Faces: %u\n", game_state->n_faces);

  printf ("Hand: ");

  for (size_t i = 0; i < game_state->hand_size; i++)
    {
      printf ("%u ", game_state->hand_array[i]);
    }
  printf ("\n");

  if (game_state->palefico_flag)
    {
      printf ("Palefico\n");
    }

  return true;
}
