/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_infocmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:08:41 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/14 19:25:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_list	list_t;
typedef struct s_entry	entry_t;
typedef struct s_header	header_t;

#define BOOLCOUNT	44
#define NUMCOUNT	39
#define STRCOUNT	414

#define STRMAXLEN	1024
#define OUTMAXWIDTH	56

#define ABS_NUM16	0xFFFF
#define CAN_NUM16	0xFFFE
#define ABS_NUM32	0xFFFFFFFF
#define CAN_NUM32	0xFFFFFFFE
#define INVALID_STR	0xFFFF

#define NUM16		0432
#define NUM32		01036

#define HEX_START	100

static const char	*bools[BOOLCOUNT] = {"bw", "am", "xsb", "xhp", "xenl", "eo",
										"gn", "hc", "km", "hs", "in", "da", "db",
										"mir", "msgr", "os", "eslok", "xt", "hz",
										"ul", "xon", "nxon", "mc5i", "chts", "nrrmc",
										"npc", "ndscr", "ccc", "bce", "hls", "xhpa",
										"crxm", "daisy", "xvpa", "sam", "cpix", "lpix",
										"OTbs", "OTns", "OTnc", "OTMT", "OTNL", "OTpt",
										"OTXR"};

static const char	*numbers[NUMCOUNT] = {"cols", "it", "lines", "lm", "xmc", "pb",
										"vt", "wsl", "nlab", "lh", "lw", "ma", "wnum",
										"colors", "pairs", "ncv", "bufsz", "spinv",
										"spinh", "maddr", "mjump", "mcs", "mls", "npins",
										"orc", "orl", "orhi", "orvi", "cps", "widcs",
										"btns", "bitwin", "bitype", "OTug", "OTdC",
										"OTdN", "OTdB", "OTdT", "OTkn"};

static const char	*strings[STRCOUNT] = {"cbt", "bel", "cr", "csr", "tbc", "clear",
										"el", "ed", "hpa", "cmdch", "cup", "cud1", "home",
										"civis", "cub1", "mrcup", "cnorm", "cuf1", "ll",
										"cuu1", "cvvis", "dch1", "dl1", "dsl", "hd", "smacs",
										"blink", "bold", "smcup", "smdc", "dim", "smir", 
										"invis", "prot", "rev", "smso", "smul", "ech", "rmacs",
										"sgr0", "rmcup", "rmdc", "rmir", "rmso", "rmul", "flash",
										"ff", "fsl", "is1", "is2", "is3", "if", "ich1", "il1",
										"ip", "kbs", "ktbc", "kclr", "kctab", "kdch1", "kdl1",
										"kcud1", "krmir", "kel", "ked", "kf0", "kf1", "kf10",
										"kf2", "kf3", "kf4", "kf5", "kf6", "kf7", "kf8", "kf9",
										"khome", "kich1", "kil1", "kcub1", "kll", "knp", "kpp",
										"kcuf1", "kind", "kri", "khts", "kcuu1", "rmkx", "smkx",
										"lf0", "lf1", "lf10", "lf2", "lf3", "lf4", "lf5", "lf6",
										"lf7", "lf8", "lf9", "rmm", "smm", "nel", "pad", "dch",
										"dl", "cud", "ich", "indn", "il", "cub", "cuf", "rin",
										"cuu", "pfkey", "pfloc", "pfx", "mc0", "mc4", "mc5",
										"rep", "rs1", "rs2", "rs3", "rf", "rc", "vpa", "sc",
										"ind", "ri", "sgr", "hts", "wind", "ht", "tsl", "uc",
										"hu", "iprog", "ka1", "ka3", "kb2", "kc1", "kc3",
										"mc5p", "rmp", "acsc", "pln", "kcbt", "smxon", "rmxon",
										"smam", "rmam", "xonc", "xoffc", "enacs", "smln","rmln",
										"kbeg", "kcan", "kclo", "kcmd", "kcpy", "kcrt", "kend",
										"kent", "kext", "kfnd", "khlp", "kmrk", "kmsg", "kmov",
										"knxt", "kopn", "kopt", "kprv", "kprt", "krdo", "kref",
										"krfr", "krpl", "krst", "kres", "ksav", "kspd", "kund",
										"kBEG", "kCAN", "kCMD", "kCPY", "kCRT", "kDC", "kDL",
										"kslt", "kEND", "kEOL", "kEXT", "kFND", "kHLP", "kHOM",
										"kIC", "kLFT", "kMSG", "kMOV", "kNXT", "kOPT", "kPRV",
										"kPRT", "kRDO", "kRPL", "kRIT", "kRES", "kSAV", "kSPD",
										"kUND", "rfi", "kf11", "kf12", "kf13", "kf14", "kf15",
										"kf16", "kf17", "kf18", "kf19", "kf20", "kf21", "kf22",
										"kf23", "kf24", "kf25", "kf26", "kf27", "kf28", "kf29",
										"kf30", "kf31", "kf32", "kf33", "kf34", "kf35", "kf36",
										"kf37", "kf38", "kf39", "kf40", "kf41", "kf42", "kf43",
										"kf44", "kf45", "kf46", "kf47", "kf48", "kf49", "kf50",
										"kf51", "kf52", "kf53", "kf54", "kf55", "kf56", "kf57",
										"kf58", "kf59", "kf60", "kf61", "kf62", "kf63", "el1",
										"mgc", "smgl", "smgr", "fln", "sclk", "dclk", "rmclk",
										"cwin", "wingo", "hup", "dial", "qdial", "tone", "pulse",
										"hook", "pause", "wait", "u0", "u1", "u2", "u3", "u4",
										"u5", "u6", "u7", "u8", "u9", "op", "oc", "initc", "initp",
										"scp", "setf", "setb", "cpi", "lpi", "chr", "cvr", "defc",
										"swidm", "sdrfq", "sitm", "slm", "smicm", "snlq", "snrmq",
										"sshm", "ssubm", "ssupm", "sum", "rwidm", "ritm", "rlm",
										"rmicm", "rshm", "rsubm", "rsupm", "rum", "mhpa", "mcud1",
										"mcub1", "mcuf1", "mvpa", "mcuu1", "porder", "mcud", "mcub",
										"mcuf", "mcuu", "scs", "smgb", "smgbp","smglp", "smgrp",
										"smgt", "smgtp", "sbim", "scsd", "rbim", "rcsd", "subcs",
										"supcs", "docr", "zerom", "csnm", "kmous", "minfo", "reqmp",
										"getm", "setaf", "setab", "pfxl", "devt", "csin", "s0ds",
										"s1ds", "s2ds", "s3ds", "smglr", "smgtb", "birep", "binel",
										"bicr", "colornm", "defbi", "endbi", "setcolor", "slines",
										"dispc", "smpch", "rmpch", "smsc", "rmsc", "pctrm", "scesc",
										"scesa", "ehhlm", "elhlm", "elohlm", "erhlm", "ethlm", "evhlm",
										"sgr1", "slength", "OTi2", "OTrs", "OTnl", "OTbc", "OTko",
										"OTma", "OTG2", "OTG3", "OTG1", "OTG4", "OTGR", "OTGL", "OTGU",
										"OTGD", "OTGH", "OTGV", "OTGC", "meml", "memu", "box1",};

struct s_list
{
	const char	*val;
	list_t		*next;
	list_t		*prev;
};

struct s_entry
{
	char		*term_names;
	char		*str_table;
	uint8_t		*bools;
	uint32_t	*numbers;
	uint16_t	*offsets;
};

struct s_header
{
	uint16_t	numbits;
	uint16_t	namelen;
	uint16_t	boolmax;
	uint16_t	nummax;
	uint16_t	strmax;
	uint16_t	nfree;
};

static inline const uint16_t	_getnum16(const char *val, const uint16_t *nums);
static inline const uint32_t	_getnum32(const char *val, const uint32_t *nums);
static inline const char		*_getstr(const entry_t *entry, const char *val);
static inline const char		*_escape(const char *str);
static inline header_t			_getheader(const int32_t fd);
static inline list_t			*_listnew(const char *val);
static inline void				_listadd(list_t **list, list_t *nlist);
static inline void				_listsort(list_t **list);

int	main(int argc, char **argv)
{
	size_t		i;
	int32_t		fd;
	int32_t		val;
	entry_t		entry;
	header_t	header;
	list_t		*bvals;
	list_t		*nvals;
	list_t		*svals;
	char		fname[256];

	if (argc == 1)
		return (1);
	snprintf(fname, 255, "/usr/share/terminfo/%c/%s%c", *argv[1], argv[1], '\0');
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		return (1);
	header = _getheader(fd);
	entry.term_names = malloc(header.namelen * sizeof(*entry.term_names));
	if (!entry.term_names || read(fd, entry.term_names, header.namelen) == -1)
		return (1);
	entry.bools = malloc(header.boolmax * sizeof(*entry.bools));
	if (!entry.bools || read(fd, entry.bools, header.boolmax) == -1)
		return (1);
	if ((header.namelen + header.boolmax) % 2 != 0  && read(fd, &i, 1) == -1)
		return (1);
	entry.numbers = malloc(header.nummax * sizeof(*entry.numbers));
	if (!entry.numbers || read(fd, entry.numbers, header.nummax * ((header.numbits == NUM16) ? 2 : 4)) == -1)
		return (1);
	entry.offsets = malloc(header.strmax * sizeof(*entry.offsets));
	if (!entry.offsets || read(fd, entry.offsets, header.strmax * 2) == -1)
		return (1);
	entry.str_table = malloc(((header.nfree - 1) * 2) * sizeof(*entry.str_table));
	if (!entry.str_table || read(fd, entry.str_table, ((header.nfree - 1) * 2)) == -1)
		return (1);
	bvals = NULL;
	nvals = NULL;
	svals = NULL;
	for (i = 0; i < header.boolmax; i++)
	{
		if (entry.bools[i])
			_listadd(&bvals, _listnew(bools[i]));
	}
	if (header.numbits == NUM16)
	{
		for (i = 0; i < header.nummax; i++)
			if (((uint16_t *)entry.numbers)[i] != ABS_NUM16 && ((uint16_t *)entry.numbers)[i] != CAN_NUM16)
				_listadd(&nvals, _listnew(numbers[i]));
	}
	else
	{
		for (i = 0; i < header.nummax; i++)
		{
			if (entry.numbers[i] != ABS_NUM32 && entry.numbers[i] != CAN_NUM32)
				_listadd(&nvals, _listnew(numbers[i]));
		}
	}
	for (i = 0; i < header.strmax; i++)
	{
		if (entry.offsets[i] != INVALID_STR)
			_listadd(&svals, _listnew(strings[i]));
	}
	_listsort(&bvals);
	_listsort(&nvals);
	_listsort(&svals);
	printf("ENTRY:\n");
	printf("%s,\n", entry.term_names);
	for (; bvals; bvals = bvals->next)
		printf("\t%s,\n", bvals->val);
	for (; nvals; nvals = nvals->next)
	{
		if (header.numbits == NUM16)
			val = _getnum16(nvals->val, (uint16_t *)entry.numbers);
		else
			val = _getnum32(nvals->val, entry.numbers);
		if (val > HEX_START)
			printf("\t%s#%#x,\n", nvals->val, val);
		else
			printf("\t%s#%u,\n", nvals->val, val);
	}
	for (; svals; svals = svals->next)
		printf("\t%s=%s,\n", svals->val, _escape(_getstr(&entry, svals->val)));
	close(fd);
	return (0);
}

static inline const uint16_t	_getnum16(const char *val, const uint16_t *nums)
{
	size_t	i;

	for (i = 0; i < NUMCOUNT; i++)
		if (strcmp(val, numbers[i]) == 0)
			break ;
	if (i == NUMCOUNT)
		exit(1);
	return (nums[i]);
}

static inline const uint32_t	_getnum32(const char *val, const uint32_t *nums)
{
	size_t	i;

	for (i = 0; i < NUMCOUNT; i++)
		if (strcmp(val, numbers[i]) == 0)
			break ;
	if (i == NUMCOUNT)
		exit(1);
	return (nums[i]);
}

static inline const char	*_getstr(const entry_t *entry, const char *val)
{
	size_t	i;

	for (i = 0; i < STRCOUNT; i++)
		if (strcmp(val, strings[i]) == 0)
			break ;
	if (i == STRCOUNT)
		exit(1);
	return (entry->str_table + entry->offsets[i]);
}

static inline const char	*_escape(const char *str)
{
	static char	buf[STRMAXLEN + 1];
	size_t		i;
	size_t		j;

	memset(buf, 0, STRMAXLEN + 1);
	for (i = 0, j = 0; i < STRMAXLEN && str[j]; i++, j++)
	{
		switch (str[j])
		{
			case '\n':
				buf[i] = '\\';
				buf[++i] = 'n';
				break ;
			case '\r':
				buf[i] = '\\';
				buf[++i] = 'r';
				break ;
			case '\e':
				buf[i] = '\\';
				buf[++i] = 'E';
				break ;
			default:
				buf[i] = (isprint(str[j])) ? str[j] : '^';
				if (!isprint(str[j]))
				{
					if (str[j] == 0x7F)
						buf[++i] = '?';
					else
						buf[++i] = str[j] + 0x40;
				}
		}
	}
	return (buf);
}

static inline header_t	_getheader(const int32_t fd)
{
	header_t	out;
	uint16_t	*ptr;
	
	ptr = &out.numbits;
	for (; ptr <= &out.nfree; ptr++)
	{
		if (read(fd, ptr, sizeof(*ptr)) == -1)
			exit(1);
	}
	return (out);
}

static inline list_t	*_listnew(const char *val)
{
	list_t	*out;

	out = (val) ? malloc(sizeof(*out)) : NULL;
	if (!out)
		return (NULL);
	out->val = val;
	return (out);
}

static inline void	_listadd(list_t **list, list_t *nlist)
{
	if (!list || !nlist)
		return ;
	if (*list)
		(*list)->prev = nlist;
	nlist->next = *list;
	nlist->prev = NULL;
	*list = nlist;
}

static inline void	_listsort(list_t **list)
{
	list_t	*cur;
	list_t	*tmp;
	list_t	*start;
	list_t	*smallest;

	start = *list;
	while (start)
	{
		cur = start->next;
		smallest = start;
		while (cur)
		{
			if (strcmp(cur->val, smallest->val) < 0)
				smallest = cur;
			cur = cur->next;
		}
		if (smallest != start)
		{
			smallest->prev->next = smallest->next;
			if (smallest->next)
				smallest->next->prev = smallest->prev;
			smallest->prev = start->prev;
			smallest->next = start;
			if (start->prev)
				start->prev->next = smallest;
			start->prev = smallest;
		}
		else
			start = start->next;
	}
	cur = smallest;
	while (cur->prev)
		cur = cur->prev;
	*list = cur;
}
