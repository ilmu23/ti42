/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ti42_defines.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:43:53 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 21:47:27 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef TI42_DEFINES_H
# define TI42_DEFINES_H

// capability counts
# define BOOLCOUNT		44
# define NUMCOUNT		39
# define STRCOUNT		424

// number size identifiers
# define NUM16B			0432
# define NUM32B			01036

// special capability values
# define ABS_NUM16B		0xFFFF
# define CAN_NUM16B		0xFFFE
# define ABS_NUM32B		0xFFFFFFFF
# define CAN_NUM32B		0xFFFFFFFE
# define INVALID_STR	0xFFFF

#endif
