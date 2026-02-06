// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ti42_load.c>>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "internal/ti42_internal.h"

#define _BUFFER_SIZE	4096

#define _BIT_COUNT_32	01036U

#define _CAN_NUM		0xFFFFFFFEU
#define _INVALID_STR	0xFFFFU

#define _OFF_BIT_COUNT		0
#define _OFF_NAME_LENGTH	2
#define _OFF_BOOL_COUNTS	4
#define _OFF_NUM_COUNTS		6
#define _OFF_STR_COUNTS		8
#define _OFF_NEXT_FREE		10
#define _OFF_BODY			12

#define _OPEN_DIRS_DEFAULT_SIZE	64

#define offset(p, n)	((void *)((uintptr_t)p + n))

#define is_present_u16(n, i)	(n.u16[i] != (uint16_t)TI42_ABS_NUM && n.u16[i] != (uint16_t)_CAN_NUM)
#define is_present_u32(n, i)	(n.u32[i] != (uint32_t)TI42_ABS_NUM && n.u32[i] != (uint32_t)_CAN_NUM)

static inline uint8_t	_extract_dirs(const char *list, const char ***dirs, size_t *i, size_t *size);
static inline uint8_t	_append_dir(const char ***dirs, const char *dir, size_t *i, size_t *size);
static inline uint8_t	_get_entry(const int fd, entry *entry);
static inline int		_open(const char *term);

capabilites	caps;
description	desc;

char	*(*ti42_getenv)(const char *) = getenv;

uint8_t	ti42_load(const char *term) {
	size_t	i;

	if (desc.loaded) {
		if (strcmp(desc.name, term) == 0)
			return 1;
		ti42_unload();
	}
	if (!_get_entry(_open(term), &desc.entry))
		return 0;
	for (i = 0; i < desc.entry.header.cap_counts.bools; i++)
		caps.boolean[i] = (boolean_cap)((desc.entry.bools[i]) ? 1 : 0);
	while (i < TI42_BOOLEAN_CAPS)
		caps.boolean[i++] = (boolean_cap)0;
	if (desc.entry.header.bit_count == 16) for (i = 0;  i < desc.entry.header.cap_counts.nums; i++)
		caps.numeric[i] = (numeric_cap)((is_present_u16(desc.entry.nums, i)) ? desc.entry.nums.u16[i] : TI42_ABS_NUM);
	else for (i = 0; i < desc.entry.header.cap_counts.nums; i++)
		caps.numeric[i] = (numeric_cap)((is_present_u32(desc.entry.nums, i)) ? desc.entry.nums.u32[i] : TI42_ABS_NUM);
	while (i < TI42_NUMERIC_CAPS)
		caps.numeric[i++] = (numeric_cap)TI42_ABS_NUM;
	for (i = 0; i < desc.entry.header.cap_counts.strs; i++)
		caps.string[i] = (string_cap)((desc.entry.offs[i] != _INVALID_STR) ? offset(desc.entry.strs, desc.entry.offs[i]) : TI42_ABS_STR);
	while (i < TI42_STRING_CAPS)
		caps.string[i++] = (string_cap)TI42_ABS_STR;
	strncpy(desc.name, term, sizeof(desc.name) - 1);
	desc.loaded = 1;
	return 1;
}

void	ti42_unload(void) {
	if (desc.loaded) {
		munmap(desc.entry.data, desc.entry.size);
		desc.loaded = 0;
	}
}

static inline uint8_t	_extract_dirs(const char *list, const char ***dirs, size_t *i, size_t *size) {
	size_t		j;
	size_t		k;
	char		buf[PATH_MAX + 1];

	for (j = k = 0; list[k]; k++) {
		if (list[k] == ':' && k > j) {
			if (k - j > PATH_MAX)
				goto __extract_dirs_err;
			memcpy(buf, &list[j], k - j);
			buf[k - j] = '\x0';
			if (!_append_dir(dirs, strdup(buf), i, size))
				return 0;
		}
		j = k + 1;
	}
	return 1;
__extract_dirs_err:
	while ((*i)--)
		free((void *)(*dirs)[*i]);
	free((void *)*dirs);
	return 0;
}

static inline uint8_t	_append_dir(const char ***dirs, const char *dir, size_t *i, size_t *size) {
	if (!dir) {
		while ((*i)--)
			free((void *)(*dirs)[*i]);
		free((void *)*dirs);
		return 0;
	}
	if (*i == *size) {
		*size *= 2;
		*dirs = realloc((void *)*dirs, *size);
		if (!*dirs)
			return 0;
	}
	(*dirs)[(*i)++] = dir;
	return 1;
}

static inline uint8_t	_get_entry(const int fd, entry *entry) {
	struct stat	st;
	size_t		names_offset;
	size_t		bools_offset;
	size_t		nums_offset;
	size_t		offs_offset;
	size_t		strs_offset;

	if (fd == -1 || fstat(fd, &st) == -1)
		return 0;
	entry->size = st.st_size;
	entry->data = mmap(NULL, entry->size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (entry->data == MAP_FAILED)
		return 0;
	entry->header.bit_count = (*(uint16_t *)offset(entry->data, _OFF_BIT_COUNT) == _BIT_COUNT_32) ? 32 : 16;
	entry->header.name_length = *(uint16_t *)offset(entry->data, _OFF_NAME_LENGTH);
	entry->header.cap_counts.bools = *(uint16_t *)offset(entry->data, _OFF_BOOL_COUNTS);
	entry->header.cap_counts.nums = *(uint16_t *)offset(entry->data, _OFF_NUM_COUNTS);
	entry->header.cap_counts.strs = *(uint16_t *)offset(entry->data, _OFF_STR_COUNTS);
	entry->header.next_free = *(uint16_t *)offset(entry->data, _OFF_NEXT_FREE);
	names_offset = _OFF_BODY;
	bools_offset = names_offset + (entry->header.name_length * sizeof(*entry->term_names));
	nums_offset = bools_offset + (entry->header.cap_counts.bools * sizeof(*entry->bools));
	nums_offset += nums_offset & 1;
	offs_offset = nums_offset + (entry->header.cap_counts.nums * ((entry->header.bit_count == 32) ? sizeof(uint32_t) : sizeof(uint16_t)));
	strs_offset = offs_offset + (entry->header.cap_counts.strs * sizeof(*entry->offs));
	entry->term_names = offset(entry->data, names_offset);
	entry->bools = offset(entry->data, bools_offset);
	entry->nums.u32 = offset(entry->data, nums_offset);
	entry->offs = offset(entry->data, offs_offset);
	entry->strs = offset(entry->data, strs_offset);
	return 1;
}

static inline int		_open(const char *term) {
	const char	**dirs;
	const char	*tmp;
	ssize_t		rv;
	size_t		i;
	size_t		size;
	char		path[PATH_MAX + 1];
	char		buf[PATH_MAX + 1];

	size = _OPEN_DIRS_DEFAULT_SIZE;
	dirs = malloc(size * sizeof(*dirs));
	if (!dirs)
		return -1;
	i = 0;
	tmp = ti42_getenv("TERMINFO");
	if (tmp && !_append_dir(&dirs, strdup(tmp), &i, &size))
		return -1;
	tmp = ti42_getenv("HOME");
	if (tmp && !_append_dir(&dirs, strdup(tmp), &i, &size))
		return -1;
	tmp = ti42_getenv("TERMINFO_DIRS");
	if (tmp && !_extract_dirs(tmp, &dirs, &i, &size))
		return -1;
	if (i + 2 >= size) {
		size += 2;
		dirs = realloc((void *)dirs, size);
		if (!dirs)
			return -1;
	}
	dirs[i++] = "/etc/terminfo";
	dirs[i++] = "/lib/terminfo";
	dirs[i] = "/usr/share/terminfo";
	for (size = i + 1, i = rv = 0; i < size; i++) {
		rv = snprintf(path, sizeof(buf), "%s/%s", dirs[i], term);
		if (rv == -1 || access(path, R_OK) == 0)
			break ;
		rv = snprintf(path, sizeof(buf), "%s/%c/%s", dirs[i], *term, term);
		if (rv == -1 || access(path, R_OK) == 0)
			break ;
	}
	i = size - 3;
	while (i--)
		free((void *)dirs[i]);
	free((void *)dirs);
	return (rv != -1) ? open(path, O_RDONLY) : -1;
}
