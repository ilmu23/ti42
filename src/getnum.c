/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getnum.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 01:41:45 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/15 01:46:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

int32_t	ft_ti_getnum(const char *code)
{
	const int32_t	*val;
	size_t			i;

	val = ft_ti_hmap_get(numcaps, code);
	if (val)
		return *val;
	for (i = 0; i < NUMCOUNT; i++)
		if (strcmp(numcodes[i], code) == 0)
			break ;
	return (i < NUMCOUNT) ? ABS_NUM : NOT_NUM;
}
