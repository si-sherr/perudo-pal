#include "game_engine.h"
#include <fff.h>

#ifdef __cplusplus
extern "C"
{
#endif

FAKE_VALUE_FUNC (bool, is_game_valid, const perudo_game_state_s *);
FAKE_VALUE_FUNC (bool, is_bid_valid, const perudo_game_state_s *,
                 const perudo_bid_s *);
FAKE_VALUE_FUNC (bool, is_next_bid_legal, const perudo_game_state_s *,
                 const perudo_bid_s *);
FAKE_VALUE_FUNC (double, probability_bid_correct, const perudo_game_state_s *,
                 const perudo_bid_s *);
FAKE_VALUE_FUNC (double, probability_bid_incorrect,
                 const perudo_game_state_s *, const perudo_bid_s *);
FAKE_VALUE_FUNC (double, probability_bid_exact, const perudo_game_state_s *,
                 const perudo_bid_s *);
FAKE_VALUE_FUNC (perudo_bid_s, find_next_best_bid,
                 const perudo_game_state_s *);

inline void
reset_fake_game_engine (void)
{
  RESET_FAKE (is_game_valid);
  RESET_FAKE (is_bid_valid);
  RESET_FAKE (is_next_bid_legal);
  RESET_FAKE (probability_bid_correct);
  RESET_FAKE (probability_bid_incorrect);
  RESET_FAKE (probability_bid_exact);
  RESET_FAKE (find_next_best_bid);
}

#ifdef __cplusplus
}
#endif
