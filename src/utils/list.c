/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:58:45 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/17 20:03:29 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

list_t	*ft_ti_list_new(const char *val)
{
	list_t	*out;

	out = (val) ? ft_ti_alloc(sizeof(*out)) : NULL;
	if (!out)
		return NULL;
	*out = (list_t){.val = val, .next = NULL, .prev = NULL};
	return out;
}

void	ft_ti_list_add(list_t **list, list_t *node)
{
	if (!list || !node)
		return ;
	if (*list)
		(*list)->prev = node;
	node->next = *list;
	*list = node;
}

void	ft_ti_list_del(list_t **list, const list_t *node)
{
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node == *list)
		*list = node->next;
	ft_ti_flist_free(node);
}
