/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:08:41 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/13 21:00:02 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LE(x, y) x[0] = y % 0xFF, x[1] = y / 0xFF

typedef struct s_entry	entry_t;
typedef struct s_header	header_t;

struct s_entry
{
	char		*term_names;
	char		*str_table;
	uint8_t		*bools;
	uint16_t	*numbers;
	char		**strings;
};

struct s_header
{
	uint16_t	namelen;
	uint16_t	boolmax;
	uint16_t	nummax;
	uint16_t	strmax;
	uint16_t	nfree;
};

static inline header_t	_getheader(const int32_t fd);

int	main(int argc, char **argv)
{
	size_t		i;
	int32_t		fd;
	entry_t		entry;
	header_t	header;

	fd = (argc > 1) ? open(argv[1], O_RDONLY) : -1;
	if (fd == -1)
		return (1);
	header = _getheader(fd);
	printf("HEADER:\n");
	printf("\tnamelen: %hu\n", header.namelen);
	printf("\tboolmax: %hu\n", header.boolmax);
	printf("\tnummax: %hu\n", header.nummax);
	printf("\tstrmax: %hu\n", header.strmax);
	printf("\tnfree: %hu\n", header.nfree);
	entry.term_names = malloc(header.namelen * sizeof(*entry.term_names));
	if (!entry.term_names || read(fd, entry.term_names, header.namelen) != header.namelen)
		return (1);
	entry.bools = malloc(header.boolmax * sizeof(*entry.bools));
	if (read(fd, entry.bools, header.boolmax) != header.boolmax)
		return (1);
	printf("ENTRY:\n");
	printf("\tterm_names: '%s'\n", entry.term_names);
	for (i = 0; i < header.boolmax; i++)
		printf("\tbools[%zu]: %hhu\n", i, entry.bools[i]);
//	entry.numbers = malloc((uint16_t)header[6] * sizeof(*entry.numbers));
//	entry.strings = malloc((uint16_t)header[8] * sizeof(*entry.strings));
	close(fd);
	return (0);
}

static inline header_t	_getheader(const int32_t fd)
{
	header_t	out;
	uint16_t	*ptr;
	
	ptr = &out.namelen;
	if (read(fd, ptr, sizeof(*ptr)) != sizeof(*ptr))
		exit(1);
	for (; ptr <= &out.nfree; ptr++)
	{
		if (read(fd, ptr, sizeof(*ptr)) != sizeof(*ptr))
			exit(1);
	}
	return (out);
}
