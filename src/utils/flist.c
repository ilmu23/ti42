/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:28:16 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 21:41:22 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define nextnode(x) flist_last = x, x = x->next
#define __free(x) free((void *)x->blk), free(x)

flist_t	*flist_head = NULL;
flist_t	*flist_last = NULL;

void	ft_ti_flist_add(flist_t *node)
{
	if (!flist_head)
		flist_head = node;
	else
		flist_last->next = node;
	flist_last = node;
}

void	ft_ti_flist_free(const void *blk)
{
	flist_t	*cur;
	flist_t	*prev;

	if (!blk)
		ft_ti_flist_clear();
	cur = flist_head;
	if (cur->blk != blk)
		nextnode(cur);
	for (; cur && cur->blk != blk; nextnode(cur))
		;
	if (cur)
	{
		if (cur != flist_head)
			prev->next = cur->next;
		else
			flist_head = cur->next;
		__free(cur);
	}
}

void	ft_ti_flist_clear(void)
{
	flist_t	*cur;
	flist_t	*prev;

	cur = flist_head;
	nextnode(cur);
	for(; cur; nextnode(cur))
		__free(prev);
	__free(prev);
}
