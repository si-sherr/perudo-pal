/**
 * @file parser.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Implementation file for CLI parser
 *
 */

/***************************************************************
 * Implements
 **************************************************************/
#include "parser.h"

/***************************************************************
 * Includes
 **************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/***************************************************************
 * Defines
 **************************************************************/
#define DEFAULT_PALEFICO_FLAG false
#define DEFAULT_N_FACES 6U

#define MAX_TOTAL_DICE_ARG_LEN 8U
#define MAX_HAND_ARG_LEN 32U
#define MAX_BID_ARG_LEN 16U
#define MAX_FACE_ARG_LEN 8U

/***************************************************************
 * Option Definitions
 **************************************************************/
static const struct option OPTIONS_LONG[]
    = { { "help", no_argument, nullptr, 'h' },
        { "version", no_argument, nullptr, 'v' },
        { "total-dice", required_argument, nullptr, 't' },
        { "hand", required_argument, nullptr, 'd' },
        { "bid", required_argument, nullptr, 'b' },
        { "palefico", no_argument, nullptr, 'p' },
        { "faces", required_argument, nullptr, 'f' },
        { nullptr, 0, nullptr, 0 } };

static const char OPTIONS_SHORT[] = ":hvt:d:b:pf:";

/***************************************************************
 * Private Function Definitions
 **************************************************************/

/**
 * @brief Parse an unsigned int from a string (decimal formatted)
 *
 * @param str Input string
 * @param ul_out Pointer to output variable
 * @return true String parsed successfully
 * @return false Could not parse string
 */
bool
parse_ul_decimal (char *str, unsigned int *ul_out)
{
  bool success = false;

  if ((str != nullptr) && (*str != '\0'))
    {
      errno = 0;
      char *endptr;

      unsigned int ul = strtoul (str, &endptr, 10);

      // If **endptr is '\0' on return, the entire string is valid (assuming
      // str is not '\0')
      if (*endptr == '\0')
        {
          *ul_out = ul;
          success = true;
        }
    }

  return success;
}

/**
 * @brief Parse a string representing the player's hand into an array of
 * unsigned integers
 * @details This function expects a string of the form "1,2,3,4,5,6"
 *
 * @param hand_str Input string to parse
 * @param hand_array_out Pointer to output array
 * @param hand_size_out Pointer to the size of the parsed string
 * @return true Complete string parsed successfully
 * @return false Could not parse complete string
 */
bool
parse_hand_str (char *hand_str, unsigned int **hand_array_out,
                size_t *hand_size_out)
{
  bool success = false;
  unsigned int *hand_array = malloc (0);
  size_t hand_size = 0;
  const char delim[] = ",";
  char *token, *saveptr;
  size_t num_tokens = 0;

  token = strtok_r (hand_str, delim, &saveptr); // first call to reentrant fn
  for (; token != nullptr; num_tokens++)
    {
      unsigned int parsed_int;
      if (parse_ul_decimal (token, &parsed_int))
        {
          // Realloc to incremented hand_size and add parsed value to end
          hand_array
              = reallocarray (hand_array, ++hand_size, sizeof (hand_array[0]));
          hand_array[hand_size - 1] = parsed_int;
        }
      token = strtok_r (nullptr, delim, &saveptr); // reentrant fn calls
    }

  // Success only if every single token was parsed into array
  if ((hand_size != 0) && (hand_size == num_tokens))
    {
      *hand_array_out = hand_array;
      *hand_size_out = hand_size;
      success = true;
    }

  return success;
}

/**
 * @brief Parse a string representing the players bid into a bid struct
 * @details Expects a string of the following format "q,v" where "q" is the
 * decimal value of the bid quantity, and "v" the decimal value of the bid
 * value.
 * @param bid_str Input string to parse
 * @param bid_out Pointer to output bid struct
 * @return true Complete string parsed successfully
 * @return false Could not parse complete string
 */
bool
parse_bid_str (char *bid_str, perudo_bid_s *bid_out)
{
  bool success = false;
  perudo_bid_s bid;
  const char delim[] = ",";
  char *quantity_token = nullptr;
  char *value_token = nullptr;
  char *saveptr;

  quantity_token = strtok_r (bid_str, delim, &saveptr);
  value_token = strtok_r (nullptr, delim, &saveptr);

  if ((strtok_r (nullptr, delim, &saveptr) == nullptr) // no more tokens
      && parse_ul_decimal (quantity_token, &bid.quantity)
      && parse_ul_decimal (value_token, &bid.value))
    {
      success = true;
      *bid_out = bid;
    }

  return success;
}

/***************************************************************
 * Publc Function Definitions
 **************************************************************/
parse_result_s
parser (int argc, char *argv[])
{
  char total_dice_arg_str[MAX_TOTAL_DICE_ARG_LEN] = { '\0' };
  char hand_arg_str[MAX_HAND_ARG_LEN] = { '\0' };
  char bid_arg_str[MAX_BID_ARG_LEN] = { '\0' };
  char face_arg_str[MAX_FACE_ARG_LEN] = { '\0' };
  parse_result_s result = { true, false, false, { 0 } };

  opterr = 0; // suppress `getopt_long()` default error message
  optind = 1; // not strictly necessary, but prevents issues from multiple
              // calls in unit tests
  int opt;

  while (1)
    {
      opt = getopt_long (argc, argv, OPTIONS_SHORT, OPTIONS_LONG, nullptr);

      // End of options
      if (opt == -1)
        {
          break;
        }

      switch (opt)
        {
        case 0:
          // No options expected to set a flag
          break;

        case 'h':
          result.help_flag = true;
          break;

        case 'v':
          result.version_flag = true;
          break;

        case 't':
          if (strlen (optarg) < MAX_TOTAL_DICE_ARG_LEN)
            {
              strcpy (total_dice_arg_str, optarg);
            }
          break;

        case 'd':
          if (strlen (optarg) < MAX_HAND_ARG_LEN)
            {
              strcpy (hand_arg_str, optarg);
            }
          break;

        case 'b':
          if (strlen (optarg) < MAX_BID_ARG_LEN)
            {
              strcpy (bid_arg_str, optarg);
            }
          break;

        case 'p':
          result.game_state.palefico_flag = true;
          break;

        case 'f':
          if (strlen (optarg) < MAX_FACE_ARG_LEN)
            {
              strcpy (face_arg_str, optarg);
            }
          break;

        case ':':
          // Ignore for now, will be caught at a later stage by argument
          // buffers being empty
          break;

        case '?':
          // Ignore for now
          break;

        default:
          break;
        }
    }

  if (face_arg_str[0] == '\0')
    {
      // Face arg not provided, ignore and set to default
      result.game_state.n_faces = DEFAULT_N_FACES;
    }
  else if (!parse_ul_decimal (face_arg_str, &result.game_state.n_faces))
    {
      fprintf (stderr, "Invalid argument for optional '-f/--faces' option\n");
      result.parse_success = false;
    }

  if (!parse_ul_decimal (total_dice_arg_str, &result.game_state.n_total_dice))
    {
      fprintf (stderr, "No valid argument for required '-t/--total' option\n");
      result.parse_success = false;
    }

  if (!parse_bid_str (bid_arg_str, &result.game_state.current_bid))
    {
      fprintf (stderr, "No valid argument for required '-b/--bid' option\n");
      result.parse_success = false;
    }

  if (!parse_hand_str (hand_arg_str, &result.game_state.hand_array,
                       &result.game_state.hand_size))
    {
      fprintf (stderr, "No valid argument for required '-d/--hand' option\n");
      result.parse_success = false;
    }

  return result;
}
