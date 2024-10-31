/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tputs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 03:25:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/31 18:39:07 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define BBYTE			9
#define _BUFSIZE		1024

#define DELAY_ALWAYS	0x1
#define DELAY_NORMAL	0x2
#define DELAY_MANDATORY	0x4

#define isvalid(x)		(x != NULL && x != NOT_STR)
#define strequals(x, y)	(x == y || (x && y && strcmp(x, y) == 0))

#define setpc			(pc = ft_ti_getstr("pc"), pc = (pc) ? pc : (const char *)&"\0")

#define storerv			(bout = (rv >= 0) ? bout + rv : -1)
#define __putc(c)		((bout >= 0) ? rv = putc(c), storerv : 0)
#define putc2(a, b)		((a) ? __putc(a) : a, (b) ? __putc(b) : b)

#define speed(x)		(speeds[x])
#define rate(x)			(speeds[x + SPEEDCOUNT])

static const char		*pc = NULL;

speed_t					ospeed = 0;

static inline int64_t	_brate(const speed_t speed);
static inline uint8_t	_sleep(const uint64_t ms, ssize_t (*putc)(const int8_t));
static inline uint8_t	_pad(const uint64_t ms, ssize_t (*putc)(const int8_t));

ssize_t	ft_ti_tputs(const char *s, const size_t affln, __UNUSED ssize_t (*putc)(const int8_t))
{
	uint8_t		delay;
	uint8_t		(*delayfn)(const uint64_t, ssize_t (*)(const int8_t));
	uint64_t	delayms;
	ssize_t		bout;
	ssize_t		rv;

	if (!isvalid(s))
		return -1;
	delay = 0;
	if (strequals(s, ft_ti_getstr("bel")) || strequals(s, ft_ti_getstr("flash")))
		delay |= DELAY_ALWAYS;
	if (!ft_ti_getflag("xon") && ft_ti_getnum("pb") != ABS_NUM && _brate(ospeed) >= ft_ti_getnum("pb"))
		delay |= DELAY_NORMAL;	
	delayfn = (ft_ti_getflag("npc")) ? _sleep : _pad;
	for (bout = 0, rv = 0; *s && bout >= 0 && rv >= 0; s++)
	{
		if (*s != '$')
			__putc(*s);
		else
		{
			if (*(++s) != '<')
				putc2('$', *s);
			else
			{
				s++;
				if ((!isdigit(*s) && *s != '.') || !strchr(s, '>'))
				{
					putc2('$', '<');
					continue ;
				}
				for (delayms = 0; isdigit(*s); s++)
					delayms = delayms * 10 + (*s - '0');
				if (*s == '.')
				{
					delayms *= 10;
					if (isdigit(*s))
						delayms += *s++ - '0';
					while (isdigit(*s))
						s++;
				}
				for (; *s == '*' || *s == '/'; s++)
				{
					if (*s == '*')
						delayms *= affln;
					else
						delay |= DELAY_MANDATORY;
				}
				if (delayms > 0 && delay)
					bout = (delayfn(delayms, putc)) ? bout : -1;
			}
		}
	}
	return (rv >= 0) ? bout : -1;
}

static inline int64_t	_brate(const speed_t speed)
{
	size_t	i;

	for (i = 0; i < SPEEDCOUNT; i++)
	{
		if (speed(i) > speed)
			break ;
		if (speed(i) == speed)
			return rate(i);
	}
	return -1;
}

static inline uint8_t	_sleep(const uint64_t ms, __UNUSED ssize_t (*putc)(const int8_t))
{
	struct timespec	total;
	struct timespec	remaining;

	errno = 0;
	total = (struct timespec){.tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000};
	while (nanosleep(&total, &remaining) == -1 && errno == EINTR)
		total = remaining;
	return (!errno || errno == EINTR) ? 1 : 0;
}

static inline uint8_t	_pad(const uint64_t ms, ssize_t (*putc)(const int8_t))
{
	uint64_t	ncount;
	ssize_t		bout;
	ssize_t		rv;

	if (!pc)
		setpc;
	ncount = (ms * _brate(ospeed)) / (BBYTE * 1000);
	for (bout = 0; ncount && bout >= 0; ncount--)
		__putc(*pc);
	return (bout >= 0) ? 1 : 0;
}
