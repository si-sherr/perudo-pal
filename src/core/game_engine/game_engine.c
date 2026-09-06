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

/***************************************************************
 * Defines
 **************************************************************/

/**
 * @brief Dice value considered "wild"
 *
 */
#define WILDCARD_DICE_VALUE 1U

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
              // Bid moving to ones, quantity must be at least half of previous
              // bid
              is_legal
                  = (2 * p_candidate_bid->quantity >= p_current_bid->quantity);
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
  // Due to the implementation of `cumulative_binomial_probability()`, it makes
  // more sense to calculate `probability_bid_incorrect()` first and then
  // subtract the value from 1, rather than the other way round (as this would
  // involve an additional subtraction from 1).
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
      // following a binomial distribution, and calculates the probability that
      // the unknown dice do not satisfy the provided bid
      unsigned int n_unknown_dice
          = p_game_state->n_total_dice - p_game_state->hand_size;

      unsigned int n_unknown_dice_to_satisfy_bid
          = unknown_dice_to_satisy_bid (p_game_state, p_candidate_bid);

      double dice_probability
          = dice_roll_probability (p_game_state, p_candidate_bid);

      // Cumulative probability function gives P(X<=k), Probabilty that bid is
      // incorrect is P(X<k), where X = `n_unknown_dice` and k =
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