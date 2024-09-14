/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 22:08:42 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 23:28:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define _HMAP_DEF_SIZE 100
#define _HMAP_REMOVED &__sentinel

#define _SALT 2903ULL

#define swap(x, y) x ^= y, y ^= x, x ^= y

#define isvalid(x) x && x != _HMAP_REMOVED

#define __freepair(x) ft_ti_flist_free(x->key), ft_ti_flist_free(x->val), ft_ti_flist_free(x)

static const pair_t	__sentinel;

static inline const pair_t	*_newpair(const char *key, const char *val);
static inline uint64_t		_hash(const char *s);
static inline uint64_t		_nextprime(uint64_t	n);
static inline uint8_t		_isprime(const uint64_t n);
static inline uint8_t		_replace(hmap_t *map, const pair_t *pair, const size_t i);
static inline hmap_t		*_newmap(const size_t bsize);
static inline hmap_t		*_grow(hmap_t *map);
static inline double		_floor(const double n);
static inline double		_sqrt(const double n);

const char	*ft_ti_hmap_get(const hmap_t *map, const char *key)
{
	const pair_t	*cur;
	size_t			i;
	size_t			j;

	if (!map)
		return 0;
	i = _hash(key) % map->size;
	for (j = 0, cur = map->items[i]; isvalid(cur); cur = map->items[i + ++j])
		if (strcmp(cur->key, key) == 0)
			break ;
	return (isvalid(cur)) ? cur->val : NULL;
}

uint8_t		ft_ti_hmap_add(hmap_t *map, const char *key, const char *val)
{
	const pair_t	*pair;
	const pair_t	*cur;
	size_t			i;
	size_t			j;

	if (!map || ((map->count * 100) / map->size > 70 && !_grow(map)))
		return 0;
	pair = _newpair(key, val);
	if (!pair)
		return 0;
	i = _hash(key) % map->size;
	for (j = 0, cur = map->items[i]; isvalid(cur); cur = map->items[i + ++j])
		if (_replace(map, pair, i))
			return 1;
	map->items[i + j] = pair;
	map->count++;
	return 1;
}

hmap_t		*ft_ti_hmap_new(void)
{
	return _newmap(_HMAP_DEF_SIZE);
}

void	ft_ti_hmap_del(const hmap_t *map)
{
	size_t	i;

	for (i = 0; i < map->size; i++)
		if (isvalid(map->items[i]))
			__freepair(map->items[i]);
	ft_ti_flist_free(map->items);
	ft_ti_flist_free(map);
}

static inline const pair_t	*_newpair(const char *key, const char *val)
{
	pair_t	*out;

	if (!key)
		goto err;
	out = ft_ti_alloc(sizeof(*out));
	if (!out)
		goto err;
	out->key = ft_ti_strdup(key);
	if (!out->key)
		goto ferr1;
	out->val = ft_ti_strdup(val);
	if (!out->val)
		goto ferr2;
	return out;
	ferr2:
	ft_ti_flist_free(out->key);
	ferr1:
	ft_ti_flist_free(out);
	err:
	return NULL;
}

static inline uint64_t	_hash(const char *s)
{
	uint64_t	out;
	size_t		slen;
	size_t		i;

	if (!s)
		return (0);
	slen = strlen(s);
	for (i = 0, out = 0; i < slen; i++)
		out += (_SALT * (slen - i + 1 * _SALT)) * s[i];
	return (out);
}

static inline uint64_t	_nextprime(uint64_t	n)
{
	while (!_isprime(n))
		n++;
	return n;
}

static inline uint8_t	_isprime(const uint64_t n)
{
	size_t	i;
	double	root;

	if (n < 2 || n % 2 == 0)
		return 0;
	if (n < 4)
		return 1;
	for (i = 3, root = _floor(_sqrt(n)); i < root; i += 2)
		if (n % i == 0)
			return 0;
	return 1;
}

static inline uint8_t	_replace(hmap_t *map, const pair_t *pair, const size_t i)
{
	if (strcmp(map->items[i]->key, pair->key) == 0)
	{
		__freepair(map->items[i]);
		map->items[i] = pair;
	}
	return (map->items[i] == pair) ? 1 : 0;
}

static inline hmap_t	*_newmap(const size_t bsize)
{
	hmap_t	*out;

	out = ft_ti_alloc(sizeof(*out));
	if (!out)
		goto err;
	out->bsize = bsize;
	out->size = _nextprime(bsize);
	out->items = ft_ti_alloc(out->size * sizeof(pair_t *));
	if (!out->items)
		goto ferr;
	out->count = 0;
	return out;
	ferr:
	ft_ti_flist_free(out);
	err:
	return NULL;
}

static inline hmap_t	*_grow(hmap_t *map)
{
	const pair_t	**items;
	hmap_t			*tmp;
	size_t			i;

	tmp = _newmap(map->bsize * 2);
	if (!tmp)
		goto err;
	for (i = 0; i < map->size; i++)
		if (map->items[i] && map->items[i] != _HMAP_REMOVED)
			goto ferr;
	map->bsize = tmp->bsize;
	map->count = tmp->count;
	swap(map->size, tmp->size);
	items = map->items;
	map->items = tmp->items;
	tmp->items = items;
	ft_ti_hmap_del(tmp);
	return map;
	ferr:
	ft_ti_hmap_del(tmp);
	err:
	return NULL;
}

static inline double	_floor(const double n)
{
	return (double)(int64_t)n;
}

static inline double	_sqrt(const double n)
{
	double	d1;
	double	d2;
	double	e;
	double	i;
	size_t	j;

	if (n <= 0.0)
		return 0.0;
	i = 1;
	d1 = i * i;
	if (n < d1)
		e = i;
	else
	{
		for (i++, d2 = i * i; n > d2; i++, d2 = i * i)
			d1 = d2;
		e = (d2 - n > n - d1) ? i - 1 : i;
	}
	for (j = 0; j < 5; j++)
		e = 0.5 * (e + (n / e));
	return e;
}
