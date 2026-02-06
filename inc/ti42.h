// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ti42.h>>

#pragma once

#include "ti42_caps.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// Invalid capability names
#define TI42_NOT_BOOL	-1
#define TI42_NOT_NUM	-2
#define TI42_NOT_STR	((const char *)-1)

// Absent or cancelled capability
#define TI42_ABS_NUM	0xFFFFFFFFU
#define TI42_ABS_STR	NULL

// Capability counts
#define TI42_BOOLEAN_CAPS	(ti42_OTXR)
#define TI42_NUMERIC_CAPS	(ti42_OTkn & ~0x80)
#define TI42_STRING_CAPS	(ti42_slength & ~0x8000)

// Environment value getter, getenv by default
extern char	*(*ti42_getenv)(const char *);

/** @brief Loads a terminal description
 *
 * Looks for a terminfo description corresponding to the
 * given terminal and loads it if one is found. If a description
 * is already loaded, unloads the old one only if a new one was found
 * @param term Name of the terminal
 * @returns @c <b>uint8_t</b> Non-zero on success,
 * 0 on failure
 */
uint8_t	ti42_load(const char *term);

/** @brief Unload the currently loaded description
 *
 * Frees all resource used for storing the
 * currently loaded description. Does nothing
 * if no description is loaded
 */
void	ti42_unload(void);

/** @brief Gets the terminal name string
 *
 * @returns @c <b>const char *</b> Terminal name string
 */
const char	*ti42_getname(void);

/** @brief Gets the value of a boolean capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>int32_t</b> 1 if present,
 * 0 if the capability is absent or cancelled,
 * TI42_NOT_BOOL if name was not a valid capability name
 */
int32_t	ti42_getflag(const uint8_t name);

/** @brief Gets the value of a numeric capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>int32_t</b> Value of the capability if present,
 * TI42_ABS_NUM if the capability is absent or cancelled,
 * TI42_NOT_NUM if name was not a valid capability name
 */
int32_t	ti42_getnum(const uint8_t name);

/** @brief Gets the value of a numeric capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>const char *</b> Value of the capability if present,
 * TI42_ABS_STR if the capability is absent or cancelled,
 * TI42_NOT_STR if name was not a valid capability name
 */
const char	*ti42_getstr(const uint16_t name);

/** @brief Returns an escape sequence for moving the cursor to the desired location
 *
 * @param seq Cursor move sequence (ti42_cup)
 * @param row Desired row, 1-based
 * @param col Desired column, 1-based
 * @returns @c <b>const char *</b> Sequence for moving to the desired location,
 * NULL if an error occurred
 */
const char	*ti42_tgoto(const char *seq, const int32_t row, const int32_t col);

/** @brief Instantiates an escape sequence with the given parameter(s)
 *
 * @param seq Sequence to apply the parameters on
 * @param ... Parameter(s) to apply
 * @returns @c </b>const char *</b> seq with the parameter(s) applied,
 * NULL if an error occurred
 */
const char	*ti42_tparm(const char *seq, ...);

/** @brief Outputs the string s, executing any delays found in it
 *
 * @param s Output string
 * @param affln Number of lines affected / 1 if not applicable
 * @param putc Function for outputting the charactes
 * @returns @c <b>ssize_t</b> Amount of bytes written,
 * -1 if an error occurred
 */
ssize_t	ti42_tputs(const char *s, const size_t affln, ssize_t (*putc)(const char));
