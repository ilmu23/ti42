/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ti42_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:09:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 23:20:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef TI42_INTERNAL_H
# define TI42_INTERNAL_H
# include "ti42.h"
# include "_internal/ti42_data.h"
# include "_internal/ti42_defines.h"
# include <ctype.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/**  UTILS  **/

// alloc.c
void		*ft_ti_alloc(const size_t n);

// flist.c
void		ft_ti_flist_add(flist_t *node);
void		ft_ti_flist_free(const void *blk);
void		ft_ti_flist_clear(void);

// hmap.c
const char	*ft_ti_hmap_get(const hmap_t *map, const char *key);
uint8_t		ft_ti_hmap_add(hmap_t *map, const char *key, const char *val);
hmap_t		*ft_ti_hmap_new(void);
void		ft_ti_hmap_del(const hmap_t *map);

// list.c
list_t		*ft_ti_list_new(const char *val);
void		ft_ti_list_add(list_t **list, list_t *node);

// str.c
char		*ft_ti_strdup(const char *s);

/**  UTILS  **/

#endif