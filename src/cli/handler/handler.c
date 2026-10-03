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
#include "game_engine.h"
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

#define OUT_TABLE_BID_NAME_COL_W 12
#define OUT_TABLE_BID_QUANTITY_W 10
#define OUT_TABLE_BID_VALUE_W 7
#define OUT_TABLE_PWIN_COL_W 8
#define OUT_TABLE_PLOSE_COL_W 9
#define OUT_TABLE_PEXACT_COL_W 10

/***************************************************************
 * Typedefs
 **************************************************************/
typedef struct
{
  perudo_bid_s bid;
  double p_win;
  double p_exact;
  double p_lose;

} bid_summary_s;

/***************************************************************
 * Private Function Definitions
 **************************************************************/
/**
 * @brief Calculate probabilities associated with bid
 * @param p_game_state Pointer to the game state
 * @param p_bid Pointer to the bid to calculate probabilities for
 * @return bid_summary_s Struct containing a summary of the bid and its
 * probabilities
 */
bid_summary_s
generate_bid_summary (const perudo_game_state_s *p_game_state,
                      const perudo_bid_s *p_bid)
{
  bid_summary_s summary;
  summary.bid = *p_bid;
  summary.p_win = probability_bid_correct (p_game_state, p_bid);
  summary.p_exact = probability_bid_exact (p_game_state, p_bid);
  summary.p_lose = probability_bid_incorrect (p_game_state, p_bid);
  return summary;
}

/**
 * @brief Print a row of the bid summary table
 * @param bid_label String used to label the bid
 * @param p_bid_summary Pointer to the bid summary to print
 */
void
print_bid_summary_row (char *bid_label, const bid_summary_s *p_bid_summary)
{
  printf ("%-*s%*u%*u%*.3f%*.3f%*.3f\n", OUT_TABLE_BID_NAME_COL_W, bid_label,
          OUT_TABLE_BID_QUANTITY_W, p_bid_summary->bid.quantity,
          OUT_TABLE_BID_VALUE_W, p_bid_summary->bid.value,
          OUT_TABLE_PWIN_COL_W, p_bid_summary->p_win, OUT_TABLE_PEXACT_COL_W,
          p_bid_summary->p_exact, OUT_TABLE_PLOSE_COL_W,
          p_bid_summary->p_lose);
}

/**
 * @brief Print the bid summary table
 * @param p_current_bid_summary Summary of the current bid
 * @param p_next_bid_summary Summary of the next best bid
 */
void
print_bid_summary_table (const bid_summary_s *p_current_bid_summary,
                         const bid_summary_s *p_next_bid_summary)
{
  // Header
  printf ("%-*s%*s%*s%*s%*s%*s\n", OUT_TABLE_BID_NAME_COL_W,
          "Bid:", OUT_TABLE_BID_QUANTITY_W, "Quantity:", OUT_TABLE_BID_VALUE_W,
          "Value:", OUT_TABLE_PWIN_COL_W, "P(win):", OUT_TABLE_PEXACT_COL_W,
          "P(exact):", OUT_TABLE_PLOSE_COL_W, "P(lose):");

  // Bid data
  if (p_current_bid_summary != nullptr && p_next_bid_summary != nullptr)
    {
      print_bid_summary_row ("Current", p_current_bid_summary);
      print_bid_summary_row ("Next Best", p_next_bid_summary);
    }
}

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
handle_game_state (const perudo_game_state_s *p_game_state)
{
  bool success = false;

  if (is_game_valid (p_game_state))
    {
      bid_summary_s current_bid_summary
          = generate_bid_summary (p_game_state, &p_game_state->current_bid);

      perudo_bid_s next_best_bid = find_next_best_bid (p_game_state);
      bid_summary_s next_bid_summary
          = generate_bid_summary (p_game_state, &next_best_bid);

      print_bid_summary_table (&current_bid_summary, &next_bid_summary);
      success = true;
    }
  else
    {
      fprintf (stderr, "Provided parameters are not a valid Perudo game.\n");
    }

  return success;
}
