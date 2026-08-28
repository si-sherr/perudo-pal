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
 * Public Function Definitions
 **************************************************************/
float
binomial_probability (float p, int k, int n)
{
  return 0.0f;
}

float
cumulative_binomial_probability (float p, int k, int n)
{
  return 0.0f;
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