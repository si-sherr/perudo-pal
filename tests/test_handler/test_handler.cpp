#include "fake_game_engine.h"
#include "handler.h"
#include <fff.h>
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

#define TEST_DEFAULT_TOTAL_DICE 30U
#define TEST_DEFAULT_FACES 6U
#define TEST_DEFAULT_HAND_SIZE 6U

#define TEST_DEFAULT_CURRENT_BID_VALUE 5U
#define TEST_DEFAULT_CURRENT_BID_QUANTITY 7U

const perudo_bid_s kDefaultCurrentBid
    = { TEST_DEFAULT_CURRENT_BID_QUANTITY, TEST_DEFAULT_CURRENT_BID_VALUE };

const unsigned int kDefaultHandArray[TEST_DEFAULT_HAND_SIZE]
    = { 1, 2, 3, 4, 5, 6 };

const bool kDefaultPalificoFlag = false;

const perudo_game_state_s kDefaultGameState
    = { TEST_DEFAULT_TOTAL_DICE, TEST_DEFAULT_FACES,
        TEST_DEFAULT_HAND_SIZE,  (unsigned int *)&kDefaultHandArray,
        kDefaultCurrentBid,      kDefaultPalificoFlag };

class HandlerTestFixture : public ::testing::Test
{
public:
  void
  SetUp ()
  {
    reset_fake_game_engine ();
    FFF_RESET_HISTORY ();
  }
};

TEST_F (HandlerTestFixture, HandleInvalidGameStateReturnsFalse)
{
  is_game_valid_fake.return_val = false;
  ASSERT_FALSE (handle_game_state (&kDefaultGameState));
}

TEST_F (HandlerTestFixture, HandleValidGameStateReturnsTrue)
{
  is_game_valid_fake.return_val = true;
  ASSERT_TRUE (handle_game_state (&kDefaultGameState));
}

TEST_F (HandlerTestFixture, HandleValidGameStateQueriesCurrentBidProbabilities)
{
  is_game_valid_fake.return_val = true;
  probability_bid_correct_fake.return_val = 0.1;
  probability_bid_incorrect_fake.return_val = 0.2;
  probability_bid_exact_fake.return_val = 0.3;

  ASSERT_TRUE (handle_game_state (&kDefaultGameState));
  ASSERT_EQ (probability_bid_correct_fake.arg0_history[0], &kDefaultGameState);
  ASSERT_EQ (probability_bid_correct_fake.arg1_history[0],
             &(kDefaultGameState.current_bid));
  ASSERT_EQ (probability_bid_exact_fake.arg0_history[0], &kDefaultGameState);
  ASSERT_EQ (probability_bid_exact_fake.arg1_history[0],
             &(kDefaultGameState.current_bid));
  ASSERT_EQ (probability_bid_incorrect_fake.arg0_history[0],
             &kDefaultGameState);
  ASSERT_EQ (probability_bid_incorrect_fake.arg1_history[0],
             &(kDefaultGameState.current_bid));
}
