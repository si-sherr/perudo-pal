/**
 * @file probability.c
 * @author Simon Sherriff
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
float
binomial_probability (float p, int k, int n)
{
  float prob = 0.0f;

  if (k <= n)
    {
      int choose_term = choose (n, k);

      prob = choose_term * powf (p, (float)k)
             * powf ((1.0f - p), (float)(n - k));
    }

  return prob;
}

float
cumulative_binomial_probability (float p, int k, int n)
{
  float prob = 0.0f;

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

int
choose (int n, int k)
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