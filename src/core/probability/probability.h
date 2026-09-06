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
 * @return double Probability of `k` successes
 */
double binomial_probability (double p, unsigned int k, unsigned int n);

/**
 * @brief Compute the cumulative binomial probability, P(X<=k), of the provided
 * parameters
 *
 * @param p Probability of success for each trial
 * @param k Number of successes
 * @param n Number of trials
 * @return double Cumulative probability of `k` or fewer successes
 */
double cumulative_binomial_probability (double p, unsigned int k,
                                        unsigned int n);

/**
 * @brief Compute n choose k
 *
 * @param n Total number of elements
 * @param k Number of elements to choose
 * @return unsigned int Number of combinations
 */
unsigned int choose (unsigned int n, unsigned int k);

#ifdef __cplusplus
}
#endif

#endif // PROBABILITY_H