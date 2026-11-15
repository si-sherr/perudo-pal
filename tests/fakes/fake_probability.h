#include <fff.h>

#ifdef __cplusplus
extern "C"
{
#endif

FAKE_VALUE_FUNC (double, binomial_probability, double, unsigned int,
                 unsigned int);
FAKE_VALUE_FUNC (double, cumulative_binomial_probability, double, unsigned int,
                 unsigned int);

inline void
reset_fake_probability (void)
{
  RESET_FAKE (binomial_probability);
  RESET_FAKE (cumulative_binomial_probability);
}

#ifdef __cplusplus
}
#endif