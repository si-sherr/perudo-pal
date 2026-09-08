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
#include "ppal_version.h"
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
#define HAND_STR_BUFFER_SIZE 32U
#define BID_STR_BUFFER_SIZE 8U

/***************************************************************
 * Variables
 **************************************************************/
static bool palefico_flag = DEFAULT_PALEFICO_FLAG;
static unsigned total_dice = 0;
static char hand_str[HAND_STR_BUFFER_SIZE] = { '\0' };
static char bid_str[BID_STR_BUFFER_SIZE] = { '\0' };
static unsigned int faces = DEFAULT_N_FACES;

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
bool
parse_ul_decimal (char *strptr, unsigned int *p_ul_out)
{
  bool success = false;

  errno = 0;
  char *endptr;
  unsigned int ul = strtoul (strptr, &endptr, 10);

  // If **endptr is '\0' on return, the entire string is valid (assuming strptr
  // is not '\0') not null)
  if (*endptr == '\0')
    {
      success = true;
      *p_ul_out = ul;
    }

  return success;
}

size_t
parse_hand_str (char *hand_str, unsigned int **hand_array_out)
{
  fprintf (stderr, "Failed to parse hand: %s\n", hand_str);
  return 0;
}

bool
parse_bid_str (char *bid_str, perudo_bid_s *bid_out)
{
  fprintf (stderr, "Failed to parse bid: %s\n", bid_str);
  return false;
}

bool
parse_game_state (unsigned int total_dice, char *hand_str, char *bid_str,
                  bool palefico_flag, unsigned int faces,
                  perudo_game_state_s *game_state_out)
{
  bool success = false;
  perudo_game_state_s parsed_game_state;

  // Parse easy integer/bool values first
  parsed_game_state.n_total_dice = total_dice;
  parsed_game_state.palefico_flag = palefico_flag;
  parsed_game_state.n_faces = faces;

  if (parse_bid_str (bid_str, &(parsed_game_state.current_bid)))
    {
      parsed_game_state.hand_size
          = parse_hand_str (hand_str, &(parsed_game_state.hand_array));

      if (parsed_game_state.hand_size != 0)
        {
          success = true;
        }
    }

  if (success)
    {
      *game_state_out = parsed_game_state;
    }

  return success;
}

/***************************************************************
 * Publc Function Definitions
 **************************************************************/
parser_data_s
parser (int argc, char *argv[])
{
  parser_data_s parse_result;

  opterr = 0; // suppress `getopt_long()` default error message
  int opt;

  while (1)
    {
      bool opt_arg_parse_fail = false;
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
          parse_result.help_flag = true;
          break;

        case 'v':
          parse_result.version_flag = true;
          break;

        case 't':
          if (!parse_ul_decimal (optarg, &total_dice))
            {
              opt_arg_parse_fail = true;
            }
          break;

        case 'd':
          if (strlen (optarg) < HAND_STR_BUFFER_SIZE)
            {
              strcpy (hand_str, optarg);
            }
          else
            {
              opt_arg_parse_fail = true;
            }
          break;

        case 'b':
          if (strlen (optarg) < BID_STR_BUFFER_SIZE)
            {
              strcpy (bid_str, optarg);
            }
          else
            {
              opt_arg_parse_fail = true;
            }
          break;

        case 'p':
          palefico_flag = true;
          break;

        case 'f':
          if (!parse_ul_decimal (optarg, &faces))
            {
              opt_arg_parse_fail = true;
            }
          break;

        case ':':
          fprintf (stderr, "Missing required option arguments\n");
          break;

        case '?':
          printf ("invalid option\n");
          break;

        default:
          break;
        }

      if (opt_arg_parse_fail)
        {
          fprintf (stderr, "Error parsing option '%c' with argument '%s'\n",
                   opt, optarg);
        }
    }

  parse_result.parse_success
      = parse_game_state (total_dice, hand_str, bid_str, palefico_flag, faces,
                          &(parse_result.game_state));

  printf ("palefico_flag %d\n", palefico_flag);
  printf ("total_dice: %d\n", total_dice);
  printf ("hand_str: %s\n", hand_str);
  printf ("bid_str: %s\n", bid_str);
  printf ("faces: %d\n", faces);

  return parse_result;
}
