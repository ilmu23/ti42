/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ti42.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:37:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/12/16 21:57:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ti42.h
 */

#pragma once
#ifndef TI42_H
# define TI42_H
# include <stdint.h>
# include <stdlib.h>

// return values for nonexisting capabilities
# define NOT_BOOL -1
# define NOT_NUM -2
# define NOT_STR (const char *)-1

// return values for absent or cancelled capabilities
# define ABS_NUM -1
# define ABS_STR NULL

// getflag.c
/** @brief Returns the value of the capability referred to by code
 *
 * @param code Capability code to look for
 * @returns @c <b>int32_t</b> 1 if capability is present,
 * 0 if it is absent or cancelled,
 * NOT_BOOL if code was not a valid boolean capability
 */
int32_t			ft_ti_getflag(const char *code);

// getnum.c
/** @brief Returns the value of the capability referred to by code
 *
 * @param code Capability code to look for
 * @returns @c <b>int32_t</b> Value of the capability if it is present,
 * ABS_NUM if it is absent or cancelled,
 * NOT_NUM if code was not a valid numeric capability
 */
int32_t			ft_ti_getnum(const char *code);

// getstr.c
/** @brief Returns the value of the capability referred to by code
 *
 * @param code Capability code to look for
 * @returns @c <b>const char *</b> Value of the capability if it is present,
 * ABS_STR if it is absent or cancelled,
 * NOT_STR if code was not a valid string capability
 */
const char		*ft_ti_getstr(const char *code);

// tgoto.c
/** @brief Returns an escape sequence for moving the cursor to the desired location
 *
 * @param seq The move cursor sequence for the current terminal
 * @param row Row to move the cursor to
 * @param col Column to move the cursor to
 * @returns @c <b>const char *</b> Escape sequence for moving to the desired location
 */
const char		*ft_ti_tgoto(const char *seq, const int32_t row, const int32_t col);

// tparm.c
/** @brief Instantiates the string s with the parameters given
 *
 * @param seq String to apply parameters on
 * @param ... Parameters to apply
 * @returns @c <b>const char *</b> seq with the parameters applied
 */
const char		*ft_ti_tparm(const char *seq, ...);

// tputs.c
/** @brief Outputs the string s, executing any delays found in it
 *
 * @param s String to output
 * @param affln Number of lines affected / 1 if not applicable
 * @param putc putchar-like function for outputting the characters
 * @returns @c <b>ssize_t</b> amount of characters output, -1 if an error occurred
 */
ssize_t			ft_ti_tputs(const char *s, const size_t affln, ssize_t (*putc)(const int8_t));

// load.c
/** @brief Loads the terminal capabilities of term
 *
 * Loads the terminal description for the terminal term.
 * Calling this function is required for most other
 * ti42 functions to work, most notably all of the
 * capability getters require a loaded terminal description.
 * Calling this function again with a different terminal will
 * replace the current description with that of the new one.
 * @param term Name of the terminal whose description to load
 * @returns @c <b>uint8_t</b> 1 on success, 0 if no description was found
 */
uint8_t			ft_ti_loadinfo(const char *term);

#endif
