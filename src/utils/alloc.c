/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:22:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/17 20:05:11 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

static inline void	___exit(void);

void	*ft_ti_alloc(const size_t n)
{
	static uint8_t	init = 0;
	flist_t			*node;

	if (!init)
	{
		atexit(___exit);
		init = 1;
	}
	if (!n)
		goto eret;
	node = malloc(sizeof(*node));
	if (!node)
		goto eret;
	*node = (flist_t){.blk = malloc(n), .next = NULL};
	if (!node->blk)
		goto efree;
	ft_ti_flist_add(node);
	memset((void *)node->blk, 0, n);
	return (void *)node->blk;
	efree:
	free(node);
	eret:
	return NULL;
}

static inline void	___exit(void)
{
	ft_ti_flist_clear();
}
