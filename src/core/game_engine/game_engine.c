/**
 * @file game_engine.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Implementation file for the Perudo game engine
 *
 */

/***************************************************************
 * Implements
 **************************************************************/
#include "game_engine.h"

/***************************************************************
 * Includes
 **************************************************************/
#include "probability.h"
#include <stdlib.h>

/***************************************************************
 * Defines
 **************************************************************/

/**
 * @brief Dice value considered "wild"
 *
 */
#define WILDCARD_DICE_VALUE 1U

/**
 * @brief Maximum number of candidates to evaluate when finding the next best
 * bid
 *
 */
#define MAX_NUM_BID_CANDIDATES 3U

/***************************************************************
 * Private Function Definitions
 **************************************************************/

/**
 * @brief Calculate the number of unknown dice (dice in play that are not in
 * the player's current hand) that are required to satisfy the provided bid.
 * @details This function takes into account number dice satisfying the bid
 * that are in the player's current hand, and sutracts them from the candidate
 * bid's quantity
 * @param p_game_state Pointer to the game state struct
 * @param p_candidate_bid Pointer to the bid struct
 * @return The number of unknown dice to satisfy the provided bid
 */
unsigned int
unknown_dice_to_satisy_bid (const perudo_game_state_s *p_game_state,
                            const perudo_bid_s *p_candidate_bid)
{
  unsigned int n_required_dice = p_candidate_bid->quantity;

  for (size_t i = 0; i < p_game_state->hand_size; i++)
    {
      // Count wildcard dice alongside bid value, only if not Palefico round
      if ((p_game_state->hand_array[i] == p_candidate_bid->value)
          || ((p_game_state->hand_array[i] == WILDCARD_DICE_VALUE)
              && !p_game_state->palefico_flag))
        {
          // If we have a dice of the bid value in our hand, reduce the total
          // number of required dice
          n_required_dice -= 1;
        }
    }

  return n_required_dice;
}

/**
 * @brief Calculate the probability that a single dice roll contributes to the
 * provided bid.
 * @details This function takes into account whether the wildcard value is
 * counted
 *
 * @param p_game_state Pointer to the game state struct
 * @param p_candidate_bid Pointer to the bid struct
 * @return double Probability that a single dice roll contributes to the
 * provided bid
 */
double
dice_roll_probability (const perudo_game_state_s *p_game_state,
                       const perudo_bid_s *p_candidate_bid)
{
  // Probability that a single dice roll lands on a particular
  // value
  double dice_probability = 1.0 / ((double)p_game_state->n_faces);

  // Wildcard counts for anything, so this effectively doubles the probability
  // of getting any a given value (unless Palefico round is in play, where
  // wildcard is not applied, or we are already bidding the wildcard)
  if (!(p_game_state->palefico_flag
        || p_candidate_bid->value == WILDCARD_DICE_VALUE))
    {
      dice_probability *= 2.0;
    }

  return dice_probability;
}

/**
 * @brief Comparison function pointer passed to `qsort()` by
 * `unique_desc_quant_sort()`. This function will sort an array of
 * `perudo_bid_s` by the `quantity` member in **descending** order.
 */
int
compare_bids_by_quantity (const void *a, const void *b)
{
  perudo_bid_s arg_a = *(const perudo_bid_s *)a;
  perudo_bid_s arg_b = *(const perudo_bid_s *)b;
  return ((arg_a.quantity < arg_b.quantity)
          - (arg_a.quantity > arg_b.quantity));
}

/**
 * @brief Sort the unique values of the hand array according to the number of
 * times each one appears, in descending order.
 * @note This function ignores wildcard values
 * @param in_array Array to sort
 * @param len  Length of array to sort
 * @param out_array Array to store sorted values
 * @return size_t Number of unique sorted values
 *
 */
size_t
sort_hand_values_by_priority (const unsigned int *in_array, size_t len,
                              unsigned int *out_array)
{
  size_t sorted_len = 0;

  // Allocate an array of bids. While sorting does not involve bids, this
  // struct already provides the key/value pair construct which is needed for
  // this algorithm
  perudo_bid_s *sorted_item_array = malloc (len * sizeof (perudo_bid_s));

  // Iterate over input array, tracking values and their number of occurences
  for (size_t i = 0; i < len; i++)
    {
      bool item_found = false;
      for (size_t j = 0; j < sorted_len; j++)
        {
          if (in_array[i] == sorted_item_array[j].value)
            {
              item_found = true;
              sorted_item_array[j].quantity++;
            }
        }
      if (!item_found && (in_array[i] != WILDCARD_DICE_VALUE))
        {
          sorted_item_array[sorted_len].value = in_array[i];
          sorted_item_array[sorted_len].quantity = 1;
          sorted_len++;
        }
    }

  qsort (sorted_item_array, sorted_len, sizeof (sorted_item_array[0]),
         compare_bids_by_quantity);

  for (size_t i = 0; i < sorted_len; i++)
    {
      out_array[i] = sorted_item_array[i].value;
    }

  free (sorted_item_array);

  return sorted_len;
}

void
append_to_bid_candidates (const perudo_bid_s *p_bid,
                          perudo_bid_s *bid_candidates,
                          size_t *p_num_candidates)
{
  bid_candidates[*p_num_candidates] = *p_bid;
  (*p_num_candidates)++;
}

size_t
select_best_bid (const perudo_game_state_s *p_game_state,
                 const perudo_bid_s *bid_candidates, size_t num_candidates)
{
  size_t best_bid_index = 0;
  double best_bid_prob = 0.0;

  for (size_t i = 0; i < num_candidates; i++)
    {
      double trial_bid_prob
          = probability_bid_correct (p_game_state, &bid_candidates[i]);

      if ((trial_bid_prob > best_bid_prob)
          && (is_next_bid_legal (p_game_state, &bid_candidates[i])))
        {
          best_bid_prob = trial_bid_prob;
          best_bid_index = i;
        }
    }

  return best_bid_index;
}

/***************************************************************
 * Public Function Definitions
 **************************************************************/

bool
is_game_valid (const perudo_game_state_s *p_game_state)
{
  bool is_valid = true;

  // Null check
  if ((p_game_state != nullptr) && (p_game_state->hand_array != nullptr))
    {
      // Check for zero values
      if ((p_game_state->n_total_dice == 0) || (p_game_state->n_faces == 0)
          || (p_game_state->hand_size == 0))
        {
          is_valid = false;
        }

      // Cannot have invalid current bid
      if (!is_bid_valid (p_game_state, &(p_game_state->current_bid)))
        {
          is_valid = false;
        }

      // Cannot have hand size exceeding total dice
      if (p_game_state->hand_size > p_game_state->n_total_dice)
        {
          is_valid = false;
        }

      // Cannot have any dice value in hand exceeding number of dice faces
      for (size_t i = 0; i < p_game_state->hand_size; i++)
        {
          if (p_game_state->hand_array[i] > p_game_state->n_faces)
            {
              is_valid = false;
            }
        }
    }
  else
    {
      is_valid = false;
    }

  return is_valid;
}

bool
is_bid_valid (const perudo_game_state_s *p_game_state,
              const perudo_bid_s *p_candidate_bid)
{
  bool is_valid = false;

  if ((p_game_state != nullptr) && (p_candidate_bid != nullptr))
    {
      is_valid = ((p_candidate_bid->quantity <= p_game_state->n_total_dice)
                  && (p_candidate_bid->value <= p_game_state->n_faces));
    }

  return is_valid;
}

bool
is_next_bid_legal (const perudo_game_state_s *p_game_state,
                   const perudo_bid_s *p_candidate_bid)
{
  bool is_legal = false;

  // Ensure the candidate bid is valid
  if (is_bid_valid (p_game_state, p_candidate_bid))
    {
      // Using pointer to current bid makes the comparisions more visually
      // consistent
      const perudo_bid_s *p_current_bid = &(p_game_state->current_bid);

      // Handle Palefico case separately
      if (p_game_state->palefico_flag)
        {
          // In the Palefico round, value of the dice cannot change, so the
          // quantity can only increase
          is_legal
              = ((p_candidate_bid->value == p_current_bid->value)
                 && (p_candidate_bid->quantity > p_current_bid->quantity));
        }
      // Handle current bid wildcard case
      else if (p_current_bid->value == WILDCARD_DICE_VALUE)
        {
          if (p_candidate_bid->value == WILDCARD_DICE_VALUE)
            {
              // Bid staying on 1s, quantity can only increase
              is_legal = (p_candidate_bid->quantity > p_current_bid->quantity);
            }
          else
            {
              // Bid moving off ones, quantity must exceed double that of
              // current bid
              is_legal = (p_candidate_bid->quantity
                          > (2 * p_current_bid->quantity));
            }
        }
      // Handle candidate bid wildcard case
      else if (p_candidate_bid->value == WILDCARD_DICE_VALUE)
        {
          if (p_current_bid->value == WILDCARD_DICE_VALUE)
            {
              // Bid staying on 1s, quantity can only increase
              is_legal = (p_candidate_bid->quantity > p_current_bid->quantity);
            }
          else
            {
              // Bid moving to ones, quantity must be at least half of
              // previous bid
              is_legal = ((2 * p_candidate_bid->quantity)
                          >= p_current_bid->quantity);
            }
        }
      // Handle standard case
      else
        {
          if (p_candidate_bid->quantity > p_current_bid->quantity)
            {
              // Quantity increased, bid legal
              is_legal = true;
            }
          else if ((p_candidate_bid->quantity == p_current_bid->quantity)
                   && (p_candidate_bid->value > p_current_bid->value))
            {
              // Quantity unchanged but value increased, bid legal
              is_legal = true;
            }
        }
    }

  return is_legal;
}

double
probability_bid_correct (const perudo_game_state_s *p_game_state,
                         const perudo_bid_s *p_candidate_bid)
{
  // Due to the implementation of `cumulative_binomial_probability()`, it
  // makes more sense to calculate `probability_bid_incorrect()` first and
  // then subtract the value from 1, rather than the other way round (as this
  // would involve an additional subtraction from 1).
  return (1.0 - probability_bid_incorrect (p_game_state, p_candidate_bid));
}

double
probability_bid_incorrect (const perudo_game_state_s *p_game_state,
                           const perudo_bid_s *p_candidate_bid)
{
  // If game or bid aren't valid, then default to 100% chance being incorrect
  double prob = 1.0;

  if (is_game_valid (p_game_state)
      && is_bid_valid (p_game_state, p_candidate_bid))
    {
      // This function takes into account all of the dice in the player's
      // current hand, treats the states of all the unknown dice in play as
      // following a binomial distribution, and calculates the probability
      // that the unknown dice do not satisfy the provided bid
      unsigned int n_unknown_dice
          = p_game_state->n_total_dice - p_game_state->hand_size;

      unsigned int n_unknown_dice_to_satisfy_bid
          = unknown_dice_to_satisy_bid (p_game_state, p_candidate_bid);

      double dice_probability
          = dice_roll_probability (p_game_state, p_candidate_bid);

      // Cumulative probability function gives P(X<=k), Probabilty that bid
      // is incorrect is P(X<k), where X = `n_unknown_dice` and k =
      // `n_unknown_dice_to_satisfy_bid` which is also equal to P(X<=k-1).
      prob = cumulative_binomial_probability (
          dice_probability, n_unknown_dice_to_satisfy_bid - 1, n_unknown_dice);
    }

  return prob;
}

double
probability_bid_exact (const perudo_game_state_s *p_game_state,
                       const perudo_bid_s *p_candidate_bid)
{
  // If came or bid aren't valid, default to 0% chance of being exact
  double prob = 0.0;

  if (is_game_valid (p_game_state)
      && is_bid_valid (p_game_state, p_candidate_bid))
    {
      unsigned int n_unknown_dice
          = p_game_state->n_total_dice - p_game_state->hand_size;

      unsigned int n_unknown_dice_to_satisfy_bid
          = unknown_dice_to_satisy_bid (p_game_state, p_candidate_bid);

      double dice_probability
          = dice_roll_probability (p_game_state, p_candidate_bid);

      prob = binomial_probability (
          dice_probability, n_unknown_dice_to_satisfy_bid, n_unknown_dice);
    }

  return prob;
}

perudo_bid_s
find_next_best_bid (const perudo_game_state_s *p_game_state)
{
  perudo_bid_s next_best_bid = { 0, 0 };

  if (is_game_valid (p_game_state))
    {
      // Case: Palefico rules
      if (p_game_state->palefico_flag)
        {
          // Under palefico rules we cannot change the value, so our best
          // option is trivial
          next_best_bid.value = p_game_state->current_bid.value;
          next_best_bid.quantity = p_game_state->current_bid.quantity + 1;
        }
      else
        {
          // Static buffer for bid candidates. In practice we expect to
          // evaluate either 2 or 3, so no faffing with dynamic allocation
          // for now
          perudo_bid_s bid_candidates[MAX_NUM_BID_CANDIDATES] = { 0 };
          size_t num_bid_candidates = 0;

          // Sort unique hand values by quantity (ignoring ones). The best
          // possible bids are going to be using the values with the highest
          // quantity, so we want to priorities these values when creating
          // candidates
          unsigned int *value_priority_array
              = malloc (p_game_state->hand_size * p_game_state->hand_array[0]);

          size_t value_priority_len = sort_hand_values_by_priority (
              p_game_state->hand_array, p_game_state->hand_size,
              value_priority_array);

          // Case: current bid is wildcard bid
          if (p_game_state->current_bid.value == WILDCARD_DICE_VALUE)
            {
              // Increment quantity for next best wildcard bid
              perudo_bid_s wildcard_bid
                  = { p_game_state->current_bid.quantity + 1,
                      WILDCARD_DICE_VALUE };
              append_to_bid_candidates (&wildcard_bid, bid_candidates,
                                        &num_bid_candidates);

              // Choose highest-quantity hand value for best non-wildcard bid
              perudo_bid_s non_wildcard_bid
                  = { (p_game_state->current_bid.quantity * 2) + 1,
                      value_priority_array[0] };
              append_to_bid_candidates (&non_wildcard_bid, bid_candidates,
                                        &num_bid_candidates);
            }

          // Case: non-wildcard normal bid
          else
            {
              perudo_bid_s wildcard_bid
                  = { (p_game_state->current_bid.quantity + 1) / 2,
                      WILDCARD_DICE_VALUE };
              append_to_bid_candidates (&wildcard_bid, bid_candidates,
                                        &num_bid_candidates);

              // In finding the best bid at the same quantity, the highest
              // priority value may not be a legal bid. Iterate over the
              // values in order of priority and take the first legal bid
              perudo_bid_s same_quantity_bid
                  = { p_game_state->current_bid.quantity, 0 };

              for (size_t i = 0; i < value_priority_len; i++)
                {
                  same_quantity_bid.value = value_priority_array[i];
                  if (is_next_bid_legal (p_game_state, &same_quantity_bid))
                    {
                      append_to_bid_candidates (&same_quantity_bid,
                                                bid_candidates,
                                                &num_bid_candidates);
                      break;
                    }
                }

              perudo_bid_s increment_quantity_bid
                  = { p_game_state->current_bid.quantity + 1,
                      value_priority_array[0] };
              append_to_bid_candidates (&increment_quantity_bid,
                                        bid_candidates, &num_bid_candidates);
            }

          // `select_best_bid()` filters all bids for legality
          next_best_bid = bid_candidates[select_best_bid (
              p_game_state, bid_candidates, num_bid_candidates)];
          free (value_priority_array);
        }
    }

  return next_best_bid;
}
