/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:34:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/07 19:01:58 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define STRBUF				1024

#define MASK16				0xFFFF
#define MASK32				0xFFFFFFFF

#define __free(x)			(ft_ti_flist_free(x), x = NULL)
#define __freeentry(x)		(__free(x.term_names), __free(x.bools), \
							__free(x.numbers), __free(x.offsets), __free(x.str_table))

#define createpath			(snprintf(path, STRBUF, "%s/%c/%s", dir, *term, term))

#define validnum(x, mask)	(x != (ABS_NUMBER & mask) && x != (CAN_NUMBER & mask))
#define validstr(x)			(x != INVALID_STR)

#define getnum(x)			((entry->header.numbits == NUM16B) ? _get16(x, (uint16_t *)entry->numbers) : _get32(x, entry->numbers))

#define BOOLLIST			(caps[0])
#define NUMLIST				(caps[1])
#define STRLIST				(caps[2])

#define TERM				struct termios

hmap_t	*boolcaps = NULL;
hmap_t	*numcaps = NULL;
hmap_t	*strcaps = NULL;

static inline const entry_t	*_getentry(const int32_t fd);
static inline const int32_t	*_true(void);
static inline const int32_t	*_get32(const char *val, const uint32_t *nums);
static inline const int32_t	*_get16(const char *val, const uint16_t *nums);
static inline const char	*_getstr(const char *val, const entry_t *entry);
static inline const char	**_getdirs(char *dirs);
static inline int32_t		_open(const char *term);
static inline uint8_t		_alloc(entry_t *entry);
static inline uint8_t		_read(const int32_t fd, entry_t *entry);

uint8_t	ft_ti_loadinfo(const char *term)
{
	static const char	*cur = NULL;
	const entry_t		*entry;
	list_t				*caps[3] = {NULL, NULL, NULL};
	size_t				i;
	TERM				t;

	if (cur && strcmp(cur, term) == 0)
		return 1;
	entry = _getentry(_open(term));
	if (!entry)
		return 0;
	for (i = 0; i < entry->header.boolmax; i++)
		if (entry->bools[i])
			ft_ti_list_add(&BOOLLIST, ft_ti_list_new(boolcodes[i]));
	if (entry->header.numbits == NUM16B)
	{
		for (i = 0; i < entry->header.nummax; i++)
			if (validnum(((uint16_t *)entry->numbers)[i], MASK16))
				ft_ti_list_add(&NUMLIST, ft_ti_list_new(numcodes[i]));
	}
	else
	{
		for (i = 0; i < entry->header.nummax; i++)
			if (validnum(entry->numbers[i], MASK32))
				ft_ti_list_add(&NUMLIST, ft_ti_list_new(numcodes[i]));
	}
	for (i = 0; i < entry->header.strmax; i++)
		if (validstr(entry->offsets[i]))
			ft_ti_list_add(&STRLIST, ft_ti_list_new(strcodes[i]));
	ft_ti_hmap_del(boolcaps);
	ft_ti_hmap_del(numcaps);
	ft_ti_hmap_del(strcaps);
	boolcaps = ft_ti_hmap_new();
	numcaps = ft_ti_hmap_new();
	strcaps = ft_ti_hmap_new();
	for (; BOOLLIST; ft_ti_list_del(&BOOLLIST, BOOLLIST))
		ft_ti_hmap_add(boolcaps, BOOLLIST->val, _true());
	for (; NUMLIST; ft_ti_list_del(&NUMLIST, NUMLIST))
		ft_ti_hmap_add(numcaps, NUMLIST->val, getnum(NUMLIST->val));
	for (; STRLIST; ft_ti_list_del(&STRLIST, STRLIST))
		ft_ti_hmap_add(strcaps, STRLIST->val, _getstr(STRLIST->val, entry));
	tcgetattr(0, &t);
	ospeed = cfgetospeed(&t);
	return 1;
}

static inline const entry_t	*_getentry(const int32_t fd)
{
	static entry_t	entry;
	const uint16_t	*ptr;

	if (fd == -1)
		return NULL;
	for (ptr = &entry.header.numbits; ptr <= &entry.header.nfree; ptr++)
		if (read(fd, (void *)ptr, sizeof(*ptr)) == -1)
			goto err;
	if (entry.term_names)
		__freeentry(entry);
	if (!_alloc(&entry))
		goto err;
	if (!_read(fd, &entry))
		goto ferr;
	close(fd);
	return &entry;
	ferr:
	__freeentry(entry);
	err:
	close(fd);
	return NULL;
}

static inline const int32_t	*_true(void)
{
	int32_t	*out;

	out = ft_ti_alloc(sizeof(*out));
	if (out)
		*out = 1;
	return out;
}

static inline int32_t	_open(const char *term)
{
	size_t		i;
	ssize_t		rv;
	const char	*dir;
	const char	**dirs;
	char		path[STRBUF + 1];

	if (!term)
		return -1;
	dir = getenv("TERMINFO");
	if (dir)
	{
		rv = snprintf(path, STRBUF, "%s/%s", dir, term);
		if (rv != -1 && access(path, F_OK) != 0)
			rv = snprintf(path, STRBUF, "%s/%c/%s", dir, *term, term);
	}
	else if (access(ft_ti_strjoin(getenv("HOME"), "/.terminfo"), F_OK) == 0)
		rv = snprintf(path, STRBUF, "%s/.terminfo/%c/%s", getenv("HOME"), *term, term);
	else
	{
		if (getenv("TERMINFO_DIRS"))
			dirs = _getdirs(ft_ti_strdup(getenv("TERMINFO_DIRS")));
		else
			dirs = tidirs;
		if (!dirs)
			return -1;
		for (i = 0, dir = dirs[i], rv = createpath; rv >= 0 && dir && access(path, R_OK); dir = dirs[++i])
			rv = createpath;
		ft_ti_flist_free(*dirs);
		ft_ti_flist_free(dirs);
	}
	return (rv >= 0 && rv <= STRBUF) ? open(path, O_RDONLY) : -1;
}

static inline const int32_t	*_get32(const char *val, const uint32_t *nums)
{
	size_t	i;
	int32_t	*out;

	for (i = 0; i < NUMCOUNT; i++)
		if (strcmp(val, numcodes[i]) == 0)
			break ;
	out = (i < NUMCOUNT) ? ft_ti_alloc(sizeof(*out)) : NULL;
	if (out)
		*out = nums[i];
	return out;
}

static inline const int32_t	*_get16(const char *val, const uint16_t *nums)
{
	size_t	i;
	int32_t	*out;

	for (i = 0; i < NUMCOUNT; i++)
		if (strcmp(val, numcodes[i]) == 0)
			break ;
	out = (i < NUMCOUNT) ? ft_ti_alloc(sizeof(*out)) : NULL;
	if (out)
		*out = nums[i];
	return out;
}

static inline const char	*_getstr(const char *val, const entry_t *entry)
{
	size_t	i;

	for (i = 0; i < STRCOUNT; i++)
		if (strcmp(val, strcodes[i]) == 0)
			break ;
	return (i < STRCOUNT) ? ft_ti_strdup(entry->str_table + entry->offsets[i]) : NULL;
}

static inline const char	**_getdirs(char *dirs)
{
	char	**out;
	size_t		dircount;
	size_t		i;

	if (!dirs)
		goto err;
	for (i = 0, dircount = 1; dirs[i]; i++)
		dircount += (dirs[i] == ':') ? 1 : 0;
	out = ft_ti_alloc((dircount + 1) * sizeof(*out));
	if (!out)
		goto ferr;
	for (i = 0, out[i] = dirs; *dirs && i < dircount; dirs++)
	{
		if (*dirs == ':')
		{
			out[++i] = dirs + 1;
			*dirs = '\0';
		}
	}
	out[i] = NULL;
	return (const char **)out;
	ferr:
	ft_ti_flist_free(dirs);
	err:
	return NULL;
}

static inline uint8_t	_alloc(entry_t *entry)
{
	entry->term_names = ft_ti_alloc(entry->header.namelen * sizeof(*entry->term_names));
	if (!entry->term_names)
		goto err;
	entry->bools = ft_ti_alloc(entry->header.boolmax * sizeof(*entry->bools));
	if (!entry->bools)
		goto ferr1;
	entry->numbers = ft_ti_alloc(entry->header.nummax * sizeof(*entry->numbers));
	if (!entry->numbers)
		goto ferr2;
	entry->offsets = ft_ti_alloc(entry->header.strmax * sizeof(*entry->offsets));
	if (!entry->offsets)
		goto ferr3;
	entry->str_table = ft_ti_alloc(((entry->header.nfree - 1) * 2) * sizeof(*entry->str_table));
	if (!entry->str_table)
		goto ferr4;
	return 1;
	ferr4:
	__free(entry->offsets);
	ferr3:
	__free(entry->numbers);
	ferr2:
	__free(entry->bools);
	ferr1:
	__free(entry->term_names);
	err:
	return 0;
}

static inline uint8_t	_read(const int32_t fd, entry_t *entry)
{
	uint8_t	c;

	if (read(fd, (void *)entry->term_names, entry->header.namelen) == -1)
		return 0;
	if (read(fd, (void *)entry->bools, entry->header.boolmax) == -1)
		return 0;
	if ((entry->header.namelen + entry->header.boolmax) % 2 != 0 && read(fd, &c, 1) == -1)
		return 0;
	if (read(fd, (void *)entry->numbers, entry->header.nummax * ((entry->header.numbits == NUM16B) ? 2 : 4)) == -1)
		return 0;
	if (read(fd, (void *)entry->offsets, entry->header.strmax * 2) == -1)
		return 0;
	if (read(fd, (void *)entry->str_table, (entry->header.nfree - 1) * 2) == -1)
		return 0;
	return 1;
}
