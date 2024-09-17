/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 01:44:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/17 20:05:25 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

const char	*ft_ti_getstr(const char *code)
{
	const char	*out;
	size_t		i;

	out = ft_ti_hmap_get(strcaps, code);
	if (out)
		return out;
	for (i = 0; i < STRCOUNT; i++)
		if (strcmp(strcodes[i], code) == 0)
			break ;
	return (i < STRCOUNT) ? ABS_STR : NOT_STR;
}
