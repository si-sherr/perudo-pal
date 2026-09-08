/**
 * @file main.c
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief CLI application entrypoint
 *
 */

/***************************************************************
 * Includes
 **************************************************************/
#include "parser.h"

/***************************************************************
 * `main()`
 **************************************************************/
int
main (int argc, char *argv[])
{
  parser_data_s parser_result = parser (argc, argv);
}
