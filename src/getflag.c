/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getflag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 01:35:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/15 01:38:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

int32_t	ft_ti_getflag(const char *code)
{
	const int32_t	*val;
	size_t			i;

	val = ft_ti_hmap_get(boolcaps, code);
	if (val)
		return *val;
	for (i = 0; i < BOOLCOUNT; i++)
		if (strcmp(boolcodes[i], code) == 0)
			break ;
	return (i < BOOLCOUNT) ? 0 : NOT_BOOL;
}
