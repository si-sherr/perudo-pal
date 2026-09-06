/**
 * @file probability.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Implementation file for general probability functions
 *
 */

/***************************************************************
 * Implements
 **************************************************************/
#include "probability.h"

/***************************************************************
 * Includes
 **************************************************************/
#include <math.h>

/***************************************************************
 * Public Function Definitions
 **************************************************************/
double
binomial_probability (double p, unsigned int k, unsigned int n)
{
  double prob = 0.0f;

  if (k <= n)
    {
      unsigned int choose_term = choose (n, k);

      prob = choose_term * pow (p, (double)k)
             * pow ((1.0 - p), (double)(n - k));
    }

  return prob;
}

double
cumulative_binomial_probability (double p, unsigned int k, unsigned int n)
{
  double prob = 0.0;

  // P(X <= k)
  if (k <= n)
    {
      // This for loop iterates over k values 0 -> k, so case is handled where
      // p = 0 and k = 0, which should give 1
      for (int i = 0; i <= k; i++)
        {
          prob += binomial_probability (p, i, n);
        }
    }

  return prob;
}

unsigned int
choose (unsigned int n, unsigned int k)
{
  int retval = 0;

  if (k > n)
    {
      retval = 0;
    }
  else if (k == 0)
    {
      retval = 1;
    }
  else if (k == n)
    {
      retval = 1;
    }
  // Recursion
  else
    {
      retval = choose (n - 1, k - 1) + choose (n - 1, k);
    }

  return retval;
}