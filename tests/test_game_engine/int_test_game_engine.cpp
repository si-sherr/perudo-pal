#include "game_engine.h"
#include <gtest/gtest.h>

#define TEST_DEFAULT_FACES 6

typedef struct
{
  std::vector<unsigned int> hand;
  unsigned int total_dice;
  perudo_bid_s current_bid;
  perudo_bid_s expected_next_best_bid;
  bool palifico;
} NextBestBidTestParam;

class NextBestBidParametrisedTestFixture
    : public ::testing::TestWithParam<NextBestBidTestParam>
{
};

TEST_P (NextBestBidParametrisedTestFixture, NextBestBidFindsExpectedBid)
{
  NextBestBidTestParam param = GetParam ();
  perudo_game_state_s game_state = { .n_total_dice = param.total_dice,
                                     .n_faces = TEST_DEFAULT_FACES,
                                     .hand_size = std::size (param.hand),
                                     .hand_array = &param.hand[0],
                                     .current_bid = param.current_bid,
                                     .palifico_flag = param.palifico };

  perudo_bid_s actual_next_best_bid = find_next_best_bid (&game_state);

  ASSERT_TRUE (is_game_valid (&game_state));
  ASSERT_TRUE (is_next_bid_legal (&game_state, &actual_next_best_bid));
  ASSERT_EQ (actual_next_best_bid.quantity,
             param.expected_next_best_bid.quantity);
  ASSERT_EQ (actual_next_best_bid.value, param.expected_next_best_bid.value);
}

INSTANTIATE_TEST_SUITE_P (
    NextBestBidTests, NextBestBidParametrisedTestFixture,
    ::testing::Values (
        NextBestBidTestParam{ .hand = { 1, 2, 3 }, // Split to ones
                              .total_dice = 10,
                              .current_bid = { 5, 6 },
                              .expected_next_best_bid = { 3, 1 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 4, 6, 6 }, // Increment current bid
                              .total_dice = 10,
                              .current_bid = { 5, 6 },
                              .expected_next_best_bid = { 6, 6 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 4, 4, 4, 6, 6 }, // Increment quantity
                              .total_dice = 10,
                              .current_bid = { 4, 6 },
                              .expected_next_best_bid = { 5, 4 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 1, 2, 3 }, // Stay on ones
                              .total_dice = 10,
                              .current_bid = { 3, 1 },
                              .expected_next_best_bid = { 4, 1 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 2, 2, 2, 2, 2, 2 }, // Move off ones
                              .total_dice = 15,
                              .current_bid = { 3, 1 },
                              .expected_next_best_bid = { 7, 2 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 1, 2, 3 }, // Bid value not in hand
                              .total_dice = 10,
                              .current_bid = { 2, 4 },
                              .expected_next_best_bid = { 3, 4 },
                              .palifico = true },
        NextBestBidTestParam{ .hand = { 1, 4, 6 },
                              .total_dice = 10,
                              .current_bid = { 5, 6 },
                              .expected_next_best_bid = { 3, 1 },
                              .palifico = false },
        NextBestBidTestParam{ .hand = { 1, 6, 6 },
                              .total_dice = 10,
                              .current_bid = { 5, 6 },
                              .expected_next_best_bid = { 6, 6 },
                              .palifico = false }));
