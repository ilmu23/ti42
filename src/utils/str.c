/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:29:21 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 23:31:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

char	*ft_ti_strdup(const char *s)
{
	size_t	asize;
	char	*out;

	if (!s)
		return NULL;
	asize = strlen(s) + 1;
	out = ft_ti_alloc(asize);
	if (out)
		strcpy(out, s);
	return out;
}
