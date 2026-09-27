/**
 * @file handler.h
 * @author Simon Sherriff (simon.sherriff.uk@gmail.com)
 * @brief Header file for CLI handler
 *
 */

#ifndef HANDLER_H
#define HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************
 * Includes
 **************************************************************/
#include "game_engine.h"

/***************************************************************
 * Public Function Prototypes
 **************************************************************/
/**
 * @brief Print the usage message either to stdout or stderr
 * @param is_error Whether the message should be an error message, which will
 * print to stderr rather than stdout
 */
void handle_usage (bool is_error);

/**
 * @brief Print the app version information to stdout
 *
 */
void handle_version (void);

/**
 * @brief Handle the parsed game state
 *
 * @param game_state Pointer to the game state struct
 * @return true Game state handled successfully
 * @return false Error in handling the game state
 */
bool handle_game_state (const perudo_game_state_s *p_game_state);

#ifdef __cplusplus
}
#endif

#endif // HANDLER_H