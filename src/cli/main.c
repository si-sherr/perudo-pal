/**
 * @file main.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief CLI application entrypoint
 * @details Handles basic argument & integer parsing before calling
 * application-specific handlers.
 *
 */

// TODO: define short opt strings to avoid duplication

/***************************************************************
 * Includes
 **************************************************************/
#include <ctype.h>
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
#define HAND_STR_BUFFER_SIZE 32U
#define BID_STR_BUFFER_SIZE 8U
/***************************************************************
 * Variables
 **************************************************************/
static bool version_flag = false;
static bool help_flag = false;
static bool palefico_flag = false;
static unsigned total_dice = 0;
static char hand_str[HAND_STR_BUFFER_SIZE] = { '\0' };
static char bid_str[BID_STR_BUFFER_SIZE] = { '\0' };
static unsigned int faces = 0;

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
 * Private Function definitions
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

void
on_parse_failure (int opt, char *optarg)
{
  fprintf (stderr, "Error parsing option '%c' with argument '%s'\n", opt,
           optarg);
  // TODO: Print usage message to stderr here.
  exit (EXIT_FAILURE);
}

void
on_missing_arg (int opt)
{
  fprintf (stderr, "Missing required argument for option '%c'\n", opt);
  // TODO: Print usage message to stderr here.
  exit (EXIT_FAILURE);
}

/***************************************************************
 * Entrypoint
 **************************************************************/
int
main (int argc, char *argv[])
{
  opterr = 0; // suppress default error message
  int opt;

  while (1)
    {
      bool parse_failure_flag = false;
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
          help_flag = true;
          break;

        case 'v':
          version_flag = true;
          break;

        case 't':
          if (!parse_ul_decimal (optarg, &total_dice))
            {
              parse_failure_flag = true;
            }
          break;

        case 'd':
          if (strlen (optarg) < HAND_STR_BUFFER_SIZE)
            {
              strcpy (hand_str, optarg);
            }
          else
            {
              parse_failure_flag = true;
            }

          break;

        case 'b':
          if (strlen (optarg) < BID_STR_BUFFER_SIZE)
            {
              strcpy (bid_str, optarg);
            }
          else
            {
              parse_failure_flag = true;
            }
          break;

        case 'p':
          palefico_flag = true;
          break;

        case 'f':
          if (!parse_ul_decimal (optarg, &faces))
            {
              parse_failure_flag = true;
            }
          break;

        case ':':
          on_missing_arg (optopt);
          break;

        case '?':
          printf ("invalid option\n");
          break;

        default:
          exit (EXIT_FAILURE);
        }

      if (parse_failure_flag)
        {
          on_parse_failure (opt, optarg);
        }
    }

  // Check required args are given
  if (total_dice == 0)
    {
      on_missing_arg ('t');
    }
  if (hand_str[0] == '\0')
    {
      on_missing_arg ('d');
    }
  if (bid_str[0] == '\0')
    {
      on_missing_arg ('b');
    }

  printf ("help_flag: %d\n", help_flag);
  printf ("version_flag %d\n", version_flag);
  printf ("palefico_flag %d\n", palefico_flag);
  printf ("total_dice: %d\n", total_dice);
  printf ("hand_str: %s\n", hand_str);
  printf ("bid_str: %s\n", bid_str);
  printf ("faces: %d\n", faces);
}
