/**
 * @file game_engine.h
 * @author Simon Sherriff (simon.sherrif.uk@gmail.com)
 * @brief Header file for the Perudo game engine
 * logic
 *
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************
 * Includes
 **************************************************************/
#include <stdbool.h>
#include <stddef.h>

/***************************************************************
 * typedefs
 **************************************************************/

typedef struct
{
  unsigned int quantity;
  unsigned int value;
} perudo_bid_s;

typedef struct
{
  unsigned int n_total_dice;
  unsigned int n_faces;
  size_t hand_size;
  unsigned int *hand_array;
  perudo_bid_s current_bid;
  bool palefico_flag;

} perudo_game_state_s;

/***************************************************************
 * Public Function Prototypes
 **************************************************************/

/**
 * @brief Evaluate if the provided game state is valid
 * @note This includes evaluating the hand and current bid
 * @param p_game_state Pointer to the game state struct
 * @return true If the game state is valid
 * @return false If the game state is invalid
 */
bool is_game_valid (const perudo_game_state_s *p_game_state);

/**
 * @brief Evaluate if the provided bid is valid, given the provided game
 * state
 * @note This function differs from `is_next_bid_legal()` in that it sanity
 * checks whether a bid is logically valid, rather than comparing it to a
 * previous bid
 * @param p_game_state Pointer to the game state struct. This value should be
 * evaluated for validity by `is_game_valid()`
 * @param p_bid_candidate Pointer to the bid candidate struct to evaluate
 * @return true If the bid is valid
 * @return false If the bid is not valid
 */
bool is_bid_valid (const perudo_game_state_s *p_game_state,
                   const perudo_bid_s *p_candidate_bid);

/**
 * @brief Evaluate if the provided bid candidate is legal against the current
 * bid, given the provided game state
 * @note This function also calls `is_bid_valid()` on `p_bid_candidate`
 * @param p_game_state Pointer to game state struct. This value should be
 * evaluated for validity by `is_game_valid()`
 * @param p_bid_candidate Pointer to bid candidate struct to evaluate
 * @return true If the bid is legal
 * @return false If the bid is not legal
 */
bool is_next_bid_legal (const perudo_game_state_s *p_game_state,
                        const perudo_bid_s *p_candidate_bid);

/**
 * @brief Calculate the probability that the provided bid candidate is correct,
 * given the provided game state
 * @param p_game_state Pointer to the game state struct
 * @param p_candidate_bid Pointer to the bid candidate struct for which to
 * calculate the probability of being correct
 * @return double Probability that the bid is correct
 */
double probability_bid_correct (const perudo_game_state_s *p_game_state,
                                const perudo_bid_s *p_candidate_bid);

/**
 * @brief Calculate the probability that the provided bid candidate is
 * incorrect, given the provided game state
 * @param p_game_state Pointer to the game state struct
 * @param p_candidate_bid Pointer to the bid candidate struct for which to
 * calculate the probability of being incorrect
 * @return double Probability that the bid is incorrect
 */
double probability_bid_incorrect (const perudo_game_state_s *p_game_state,
                                  const perudo_bid_s *p_candidate_bid);

/**
 * @brief Calculate the probability that the provided bid candidate is
 * exactly true, given the provided game state
 * @param p_game_state Pointer to the game state struct
 * @param p_candidate_bid Pointer to the bid candidate struct for which to
 * calculate the probability of being exactly true
 * @return double Probability that the bid is exactly true
 */
double probability_bid_exact (const perudo_game_state_s *p_game_state,
                              const perudo_bid_s *p_candidate_bid);

bool find_next_best_bid (const perudo_game_state_s *p_game_state,
                         perudo_bid_s *p_bid_out, double *p_bid_prob);

#ifdef __cplusplus
}
#endif

#endif // GAME_ENGINE_H