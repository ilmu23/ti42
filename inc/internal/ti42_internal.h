// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ti42_internal.h>>

#pragma once

#include <linux/limits.h>

#include "ti42.h"

#define in_range(x, lower, upper)	((x) >= (lower) && (x) <= (upper))

typedef struct {
	uint16_t	bit_count;
	uint16_t	name_length;
	struct {
		uint16_t	bools;
		uint16_t	nums;
		uint16_t	strs;
	}			cap_counts;
	uint16_t	next_free;
}	header;

typedef struct {
	size_t			size;
	void			*data;
	const uint8_t	*bools;
	union {
		const uint32_t	*u32;
		const uint16_t	*u16;
	}				nums;
	const uint16_t	*offs;
	const char		*term_names;
	const char		*strs;
	header			header;
}	entry;

typedef struct {
	entry	entry;
	char	name[NAME_MAX + 1];
	uint8_t	loaded;
}	description;

typedef uint8_t		boolean_cap;
typedef int32_t		numeric_cap;
typedef const char	*string_cap;

typedef struct {
	boolean_cap	boolean[TI42_BOOLEAN_CAPS];
	numeric_cap	numeric[TI42_NUMERIC_CAPS];
	string_cap	string[TI42_STRING_CAPS];
}	capabilites;

extern description	desc;
extern capabilites	caps;
