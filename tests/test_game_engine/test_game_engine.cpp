#include "fake_probability.h"
#include "fff.h"
#include "game_engine.h"
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

#define TEST_DEFAULT_TOTAL_DICE 30U
#define TEST_DEFAULT_FACES 6U
#define TEST_DEFAULT_HAND_SIZE 6U

#define TEST_DEFAULT_CURRENT_BID_VALUE 5U
#define TEST_DEFAULT_CURRENT_BID_QUANTITY 7U

#define WILDCARD_VALUE 1U

const perudo_bid_s kDefaultCurrentBid
    = { TEST_DEFAULT_CURRENT_BID_QUANTITY, TEST_DEFAULT_CURRENT_BID_VALUE };

const perudo_bid_s kDefaultNextBid = {
  TEST_DEFAULT_CURRENT_BID_QUANTITY + 1,
  TEST_DEFAULT_CURRENT_BID_VALUE,
};

const unsigned int kDefaultHandArray[TEST_DEFAULT_HAND_SIZE]
    = { 1, 2, 3, 4, 5, 6 };

const bool kDefaultPaleficoFlag = false;

const perudo_game_state_s kDefaultGameState
    = { TEST_DEFAULT_TOTAL_DICE, TEST_DEFAULT_FACES,
        TEST_DEFAULT_HAND_SIZE,  (unsigned int *)&kDefaultHandArray,
        kDefaultCurrentBid,      kDefaultPaleficoFlag };

class GameEngineTestFixture : public ::testing::Test
{
public:
  void
  SetUp ()
  {
    reset_fake_probability ();
    FFF_RESET_HISTORY ();
  }
};

/***************************************************************
 * is_game_valid() tests
 **************************************************************/

TEST_F (GameEngineTestFixture, IsGameValidSensibleDefaultsReturnsTrue)
{
  ASSERT_TRUE (is_game_valid (&kDefaultGameState));
}

TEST_F (GameEngineTestFixture, IsGameValidNullPtrReturnsFalse)
{
  ASSERT_FALSE (is_game_valid (nullptr));
}

TEST_F (GameEngineTestFixture, IsGameValidHandArrayNullPtrReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.hand_array = nullptr;

  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidTotalDiceZeroReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.n_total_dice = 0;

  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidHandSizeZeroReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.hand_size = 0;

  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidZeroFacesReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.n_faces = 0;

  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidInvalidCurrentBidReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  perudo_bid_s invalid_bid
      = { game_state.n_faces + 1, TEST_DEFAULT_CURRENT_BID_QUANTITY };
  game_state.current_bid = invalid_bid;

  ASSERT_FALSE (is_bid_valid (&game_state, &invalid_bid));
  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidHandSizeTooLargeReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.hand_size = game_state.n_total_dice + 1;

  ASSERT_FALSE (is_game_valid (&game_state));
}

TEST_F (GameEngineTestFixture, IsGameValidHandValueExceedsFacesReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  unsigned int invalid_hand_array[game_state.hand_size]
      = { 1, 2, 3, 4, 5, (game_state.n_faces + 1) };
  game_state.hand_array = invalid_hand_array;

  ASSERT_FALSE (is_game_valid (&game_state));
}

/***************************************************************
 * is_bid_valid() tests
 **************************************************************/

TEST_F (GameEngineTestFixture, IsBidValidSensibleDefaultReturnsTrue)
{
  ASSERT_TRUE (is_bid_valid (&kDefaultGameState, &kDefaultCurrentBid));
}

TEST_F (GameEngineTestFixture, IsBidValidNullPtrsReturnsFalse)
{
  ASSERT_FALSE (is_bid_valid (nullptr, nullptr));
  ASSERT_FALSE (is_bid_valid (&kDefaultGameState, nullptr));
  ASSERT_FALSE (is_bid_valid (nullptr, &kDefaultCurrentBid));
}

TEST_F (GameEngineTestFixture, IsBidValidQuantityExceedsTotalDiceReturnsFalse)
{
  perudo_bid_s valid_bid = kDefaultCurrentBid;
  perudo_bid_s invalid_bid = kDefaultCurrentBid;

  valid_bid.quantity = TEST_DEFAULT_TOTAL_DICE;
  invalid_bid.quantity = TEST_DEFAULT_TOTAL_DICE + 1;

  ASSERT_TRUE (is_bid_valid (&kDefaultGameState, &valid_bid));
  ASSERT_FALSE (is_bid_valid (&kDefaultGameState, &invalid_bid));
}

TEST_F (GameEngineTestFixture, IsBidValidValueExceedsFacesReturnsFalse)
{
  perudo_bid_s valid_bid = kDefaultCurrentBid;
  perudo_bid_s invalid_bid = kDefaultCurrentBid;

  valid_bid.value = TEST_DEFAULT_FACES;
  invalid_bid.value = TEST_DEFAULT_FACES + 1;

  ASSERT_TRUE (is_bid_valid (&kDefaultGameState, &valid_bid));
  ASSERT_FALSE (is_bid_valid (&kDefaultGameState, &invalid_bid));
}

/***************************************************************
 * is_next_bid_legal() tests
 **************************************************************/

typedef struct
{
  perudo_bid_s current_bid;
  perudo_bid_s next_bid;
  bool is_expected_legal;
} BidLegalTestParam;

class BidLegalParametrisedTestFixture
    : public ::testing::TestWithParam<BidLegalTestParam>
{
};

TEST_P (BidLegalParametrisedTestFixture, IsBidLegalReturnsExpectedValues)
{
  BidLegalTestParam param = GetParam ();
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.current_bid = param.current_bid;

  bool is_legal = is_next_bid_legal (&game_state, &(param.next_bid));

  ASSERT_EQ (is_legal, param.is_expected_legal);
}

INSTANTIATE_TEST_SUITE_P (
    GameEngineTestFixture, BidLegalParametrisedTestFixture,
    ::testing::Values (
        BidLegalTestParam{ .current_bid = { 5, 5 },
                           .next_bid = { 5, kDefaultGameState.n_faces + 1 },
                           .is_expected_legal = false },
        BidLegalTestParam{
            { 5, 5 }, { kDefaultGameState.n_total_dice + 1, 6 }, false },

        BidLegalTestParam{ { 5, 5 }, { 5, 6 }, true },
        BidLegalTestParam{ { 5, 5 }, { 6, 5 }, true },
        BidLegalTestParam{ { 5, 5 }, { 6, 6 }, true },
        BidLegalTestParam{ { 5, 5 }, { 5, 4 }, false },
        BidLegalTestParam{ { 5, 5 }, { 4, 6 }, false },
        BidLegalTestParam{ { 5, 5 }, { 6, 4 }, true },
        BidLegalTestParam{ { 5, 5 }, { 3, WILDCARD_VALUE }, true },
        BidLegalTestParam{ { 5, 5 }, { 2, WILDCARD_VALUE }, false },
        BidLegalTestParam{ { 4, 5 }, { 2, WILDCARD_VALUE }, true },
        BidLegalTestParam{ { 4, 5 }, { 1, WILDCARD_VALUE }, false },
        BidLegalTestParam{ { 4, 5 }, { 3, WILDCARD_VALUE }, true },
        BidLegalTestParam{
            { 2, WILDCARD_VALUE }, { 3, WILDCARD_VALUE }, true },
        BidLegalTestParam{
            { 2, WILDCARD_VALUE }, { 2, WILDCARD_VALUE }, false },
        BidLegalTestParam{
            { 2, WILDCARD_VALUE }, { 1, WILDCARD_VALUE }, false },
        BidLegalTestParam{ { 2, WILDCARD_VALUE }, { 4, 5 }, false },
        BidLegalTestParam{ { 2, WILDCARD_VALUE }, { 5, 5 }, true },
        BidLegalTestParam{ { 3, WILDCARD_VALUE }, { 6, 5 }, false },
        BidLegalTestParam{ { 3, WILDCARD_VALUE }, { 7, 5 }, true }));

/***************************************************************
 * is_next_bid_legal() Palefico case tests
 **************************************************************/

TEST_F (GameEngineTestFixture,
        IsNextBidLegalPaleficoIncreasedQuantitySameValueReturnsTrue)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.palefico_flag = true;

  perudo_bid_s legal_bid = kDefaultCurrentBid;
  legal_bid.quantity += 1;

  ASSERT_TRUE (is_next_bid_legal (&kDefaultGameState, &legal_bid));
}

TEST_F (GameEngineTestFixture,
        IsNextBidLegalPaleficoIncreasedQuantityIncreasedValueReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.palefico_flag = true;

  perudo_bid_s increased_value_and_quantity_bid = kDefaultCurrentBid;
  increased_value_and_quantity_bid.quantity += 1;
  increased_value_and_quantity_bid.value += 1;

  ASSERT_FALSE (
      is_next_bid_legal (&game_state, &increased_value_and_quantity_bid));
}

TEST_F (GameEngineTestFixture,
        IsNextBidLegalPaleficoSameQuantityIncreasedValueReturnsFalse)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.palefico_flag = true;

  perudo_bid_s increased_value_same_quantity_bid = kDefaultCurrentBid;
  increased_value_same_quantity_bid.value += 1;

  ASSERT_FALSE (
      is_next_bid_legal (&game_state, &increased_value_same_quantity_bid));
}

/***************************************************************
 * General bid probability tests
 **************************************************************/
TEST_F (GameEngineTestFixture, ExpectedBidProbIfInvalidGameState)
{
  perudo_game_state_s game_state = kDefaultGameState;
  game_state.n_total_dice = game_state.hand_size - 1;

  ASSERT_FALSE (is_game_valid (&game_state));

  ASSERT_DOUBLE_EQ (
      probability_bid_correct (&game_state, &(game_state.current_bid)), 0.0f);
  ASSERT_DOUBLE_EQ (
      probability_bid_incorrect (&game_state, &(game_state.current_bid)),
      1.0f);
  ASSERT_DOUBLE_EQ (
      probability_bid_exact (&game_state, &(game_state.current_bid)), 0.0f);

  ASSERT_EQ (binomial_probability_fake.call_count, 0);
  ASSERT_EQ (cumulative_binomial_probability_fake.call_count, 0);
}

TEST_F (GameEngineTestFixture, ExpectedBidProbIfInvalidBid)
{
  perudo_game_state_s game_state = kDefaultGameState;
  perudo_bid_s bid = kDefaultCurrentBid;
  bid.quantity = game_state.n_total_dice + 1;

  ASSERT_FALSE (is_bid_valid (&game_state, &bid));

  ASSERT_DOUBLE_EQ (probability_bid_correct (&game_state, &bid), 0.0);
  ASSERT_DOUBLE_EQ (probability_bid_incorrect (&game_state, &bid), 1.0);
  ASSERT_DOUBLE_EQ (probability_bid_exact (&game_state, &bid), 0.0);

  ASSERT_EQ (binomial_probability_fake.call_count, 0);
  ASSERT_EQ (cumulative_binomial_probability_fake.call_count, 0);
}

typedef struct
{
  unsigned int total_dice;
  perudo_bid_s bid;
  bool palefico_flag;
  double expected_binomial_p_arg;
  unsigned int expected_binomial_k_arg;
  unsigned int expected_binomial_n_arg;

} BidProbTestParam;

class BidProbParametrisedTestFixture
    : public ::testing::TestWithParam<BidProbTestParam>
{
protected:
  BidProbTestParam param;
  perudo_game_state_s testGameState;
  perudo_bid_s testBid;
  const double kProbMockRetval = 0.42;

  void
  SetUp ()
  {
    param = GetParam ();

    // There is a huge amount of parameter combinations we can test against. To
    // keep the tests simple, the game state (except for palefico) and hand are
    // fixed, and only the bid is parametrised
    unsigned int testHand[6] = { 1, 1, 2, 2, 3, 4 };

    testGameState = { .n_total_dice = param.total_dice,
                      .n_faces = 6,
                      .hand_size = sizeof (testHand) / sizeof (testHand[0]),
                      .hand_array = testHand,
                      .palefico_flag = param.palefico_flag };

    testBid = param.bid;

    // Prepare mock return values
    cumulative_binomial_probability_fake.return_val = kProbMockRetval;
    binomial_probability_fake.return_val = kProbMockRetval;

    ASSERT_TRUE (is_game_valid (&testGameState));
    ASSERT_TRUE (is_bid_valid (&testGameState, &testBid));
  }
};

/***************************************************************
 * probability_bid_incorrect() tests
 **************************************************************/
// It is easier to think about mocking `cumulative_binomial_probability()` when
// invoking `probability_bid_incorrect()` than the other way round.

// Since `probability_bid_correct()` = 1 - `probability_bid_incorrect()`, these
// tests only focus on `probability_bid_incorrect()` and it is assumed they are
// valid for `probability_bid_correct()`.
class ProbBidIncorrectTestFixture : public BidProbParametrisedTestFixture
{
};

TEST_P (ProbBidIncorrectTestFixture,
        ProbBidIncorrectUsesExpectedBinomialPValue)
{
  double actual_retval
      = probability_bid_incorrect (&(testGameState), &testBid);

  ASSERT_DOUBLE_EQ (cumulative_binomial_probability_fake.arg0_val,
                    param.expected_binomial_p_arg);
}

TEST_P (ProbBidIncorrectTestFixture,
        BProbBidIncorrectUsesExpectedBinomialKValue)
{
  double actual_retval = probability_bid_incorrect (&testGameState, &testBid);

  ASSERT_EQ (cumulative_binomial_probability_fake.arg1_val,
             param.expected_binomial_k_arg);
}

TEST_P (ProbBidIncorrectTestFixture,
        ProbBidIncorrectUsesExpectedBinomialNValue)
{
  double actual_retval
      = probability_bid_incorrect (&testGameState, &(testBid));

  ASSERT_EQ (cumulative_binomial_probability_fake.arg2_val,
             param.expected_binomial_n_arg);
}

INSTANTIATE_TEST_SUITE_P (
    GameEngineTestFixture, ProbBidIncorrectTestFixture,
    ::testing::Values (
        BidProbTestParam{ .total_dice = 30,
                          .bid = { 10, 2 },
                          .palefico_flag = false,
                          .expected_binomial_p_arg = (1.0 / 3.0),
                          .expected_binomial_k_arg = 5,
                          .expected_binomial_n_arg = 24 },
        BidProbTestParam{ 20, { 10, 5 }, false, (1.0 / 3.0), 7, 14 },
        BidProbTestParam{ 20, { 5, 5 }, true, (1.0 / 6.0), 4, 14 },
        BidProbTestParam{ 30, { 10, 1 }, false, (1.0 / 6.0), 7, 24 },
        BidProbTestParam{ 30, { 5, 2 }, true, (1.0 / 6.0), 2, 24 },
        BidProbTestParam{ 30, { 5, 1 }, true, (1.0 / 6.0), 2, 24 }));

/***************************************************************
 * probability_bid_correct() tests
 **************************************************************/
// Assumes `probability_bid_incorrect()` fully tested

TEST_F (GameEngineTestFixture, ProbabilityBidCorrectInverseOfIncorrect)
{
  const double kProbMockRetval = 0.1;
  cumulative_binomial_probability_fake.return_val = kProbMockRetval;

  double actual_prob_incorrect
      = probability_bid_incorrect (&kDefaultGameState, &kDefaultCurrentBid);
  double actual_prob_correct
      = probability_bid_correct (&kDefaultGameState, &kDefaultCurrentBid);

  ASSERT_EQ (actual_prob_incorrect, kProbMockRetval);
  ASSERT_EQ (actual_prob_correct, 1.0f - actual_prob_incorrect);
}

/***************************************************************
 * probability_bid_exact() tests
 **************************************************************/
class ProbBidExactTestFixture : public BidProbParametrisedTestFixture
{
};

TEST_P (ProbBidExactTestFixture, ProbBidExactUsesExpectedBinomialPValue)
{
  double actual_retval = probability_bid_exact (&(testGameState), &testBid);

  ASSERT_DOUBLE_EQ (binomial_probability_fake.arg0_val,
                    param.expected_binomial_p_arg);
}

TEST_P (ProbBidExactTestFixture, BProbBidExactUsesExpectedBinomialKValue)
{
  double actual_retval = probability_bid_exact (&testGameState, &testBid);

  ASSERT_EQ (binomial_probability_fake.arg1_val,
             param.expected_binomial_k_arg);
}

TEST_P (ProbBidExactTestFixture, ProbBidExactUsesExpectedBinomialNValue)
{
  double actual_retval = probability_bid_exact (&testGameState, &(testBid));

  ASSERT_EQ (binomial_probability_fake.arg2_val,
             param.expected_binomial_n_arg);
}

INSTANTIATE_TEST_SUITE_P (
    GameEngineTestFixture, ProbBidExactTestFixture,
    ::testing::Values (
        BidProbTestParam{ .total_dice = 30,
                          .bid = { 10, 2 },
                          .palefico_flag = false,
                          .expected_binomial_p_arg = (1.0 / 3.0),
                          .expected_binomial_k_arg = 6,
                          .expected_binomial_n_arg = 24 },
        BidProbTestParam{ 20, { 10, 5 }, false, (1.0 / 3.0), 8, 14 },
        BidProbTestParam{ 20, { 5, 5 }, true, (1.0 / 6.0), 5, 14 },
        BidProbTestParam{ 30, { 10, 1 }, false, (1.0 / 6.0), 8, 24 },
        BidProbTestParam{ 30, { 5, 2 }, true, (1.0 / 6.0), 3, 24 },
        BidProbTestParam{ 30, { 5, 1 }, true, (1.0 / 6.0), 3, 24 }));