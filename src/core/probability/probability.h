/**
 * @file probability.h
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Header file for general probability functions
 *
 */

#ifndef PROBABILITY_H
#define PROBABILITY_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************
 * Public Function Prototypes
 **************************************************************/

/**
 * @brief Compute the binomial probability, P(X=k), of the provided parameters
 *
 * @param p Probability of success for each trial
 * @param k Number of successes
 * @param n Number of trials
 * @return float Probability of `k` successes
 */
float binomial_probability (float p, int k, int n);

/**
 * @brief Compute the cumulative binomial probability, P(X<=k), of the provided
 * parameters
 *
 * @param p Probability of success for each trial
 * @param k Number of successes
 * @param n Number of trials
 * @return float Cumulative probability of `k` or fewer successes
 */
float cumulative_binomial_probability (float p, int k, int n);

/**
 * @brief Compute n choose k
 *
 * @param n Total number of elements
 * @param k Number of elements to choose
 * @return int Number of combinations
 */
int choose (int n, int k);

#ifdef __cplusplus
}
#endif

#endif // PROBABILITY_H