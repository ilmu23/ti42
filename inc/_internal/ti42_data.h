/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ti42_data.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:11:21 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 13:09:51 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef TI42_DATA_H
# define TI42_DATA_H
# include "ti42.h"
# include "_internal/ti42_defines.h"
# include <termios.h>

typedef struct s_list	list_t;
typedef struct s_hmap	hmap_t;
typedef struct s_pair	pair_t;
typedef struct s_flist	flist_t;
typedef struct s_entry	entry_t;
typedef struct s_header	header_t;

struct s_list
{
	const char		*val;
	list_t			*next;
	list_t			*prev;
};

struct s_hmap
{
	size_t			size;
	size_t			bsize;
	size_t			count;
	const pair_t	**items;
};

struct s_pair
{
	const char		*key;
	const char		*val;
};

struct s_flist
{
	const void		*blk;
	flist_t			*next;
};

struct s_header
{
	const uint16_t	numbits;
	const uint16_t	namelen;
	const uint16_t	boolmax;
	const uint16_t	nummax;
	const uint16_t	strmax;
	const uint16_t	nfree;
};

struct s_entry
{
	const char		*term_names;
	const char		*str_table;
	const uint8_t	*bools;
	const uint32_t	*numbers;
	const uint16_t	*offsets;
	const header_t	header;
};

// codes.c
extern const speed_t	speeds[SPEEDCOUNT * 2];
extern const char		*tidirs[4];
extern const char		*boolcodes[BOOLCOUNT];
extern const char		*numcodes[NUMCOUNT];
extern const char		*strcodes[STRCOUNT];

// load.c
extern hmap_t	*boolcaps;
extern hmap_t	*numcaps;
extern hmap_t	*strcaps;

// tputs.c
extern speed_t	ospeed;

#endif
