// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ti42_tputs.c>>

#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "internal/ti42_internal.h"

#define _SPEED_COUNT	31

#define _DELAY_ALWAYS		0x1U
#define _DELAY_NORMAL		0x2U
#define _DELAY_MANDATORY	0x4U

#define bool_cap(cap)	(caps.boolean[cap - 1])
#define num_cap(cap)	(caps.numeric[(cap ^ 0x80) - 1])
#define	str_cap(cap)	(caps.string[(cap ^ 0x8000) - 1])

#define str_equals(s1, s2)	((s1) == (s2) || ((s1) && (s2) && strcmp((s1), (s2)) == 0))

typedef enum {
	NANOSLEEP,
	PAD
}	delay_type;

static const speed_t	speeds[_SPEED_COUNT * 2] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 4097, 4098, 4099, 4100,
	4101, 4102, 4103, 4104, 4105, 4106, 4107, 4108, 4109, 4110, 41111, 0, 50, 75,
	110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600,
	115200, 130400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000,
	2500000, 3000000, 3500000, 4000000
};

static speed_t	ospeed;

static inline int64_t	_get_baud_rate(void);
static inline uint8_t	_is_delay(const char *s);
static inline int8_t	_sleep(const uint64_t ms, const delay_type type, ssize_t (*putc)(const char));

ssize_t	ti42_tputs(const char *s, const size_t affln, ssize_t (*putc)(const char)) {
	unsigned _BitInt(3)	delay;
	delay_type			type;
	uint64_t			delay_ms;
	ssize_t				bytes_written;
	ssize_t				rv;

	if (s == TI42_ABS_STR || s == TI42_NOT_STR)
		return 0;
	delay = 0;
	if (str_equals(s, str_cap(ti42_bel)) || str_equals(s, str_cap(ti42_flash)))
		delay |= _DELAY_ALWAYS;
	if (!bool_cap(ti42_xon) && num_cap(ti42_pb) != (int32_t)TI42_ABS_NUM && _get_baud_rate() >= num_cap(ti42_pb))
		delay |= _DELAY_NORMAL;
	type = (bool_cap(ti42_npc)) ? NANOSLEEP : PAD;
	bytes_written = 0;
	do {
		if (_is_delay(s)) {
			s += 2;
			delay_ms = strtoul(s, (char **)&s, 10);
			if (*s == '.') {
				delay_ms *= 10;
				if (isdigit(*(++s)))
					delay_ms += *s++ - '0';
			}
			while (*s == '*' || *s == '/') {
				if (*s++ == '*')
					delay_ms *= affln;
				else
					delay |= _DELAY_MANDATORY;
			}
			s++;
			if (delay_ms && delay)
				rv = _sleep(delay_ms, type, putc);
		} else
			rv = putc(*s++);
		bytes_written += rv;
	} while (*s && rv == 1);
	return (rv != -1) ? bytes_written : -1;
}

static inline int64_t	_get_baud_rate(void) {
	size_t	i;

	for (i = 0; i < _SPEED_COUNT; i++) {
		if (speeds[i] >= ospeed)
			return (speeds[i] == ospeed) ? (int64_t)speeds[i + _SPEED_COUNT] : -1;
	}
	return -1;
}

static inline uint8_t	_is_delay(const char *s) {
	size_t	n;

	if (*s++ != '$')
		return 0;
	if (*s++ != '<')
		return 0;
	for (n = 0; isdigit(*s++); n++)
		;
	if (!n)
		return 0;
	if (*s == '.' && (!isdigit(*(++s)) || isdigit(*(++s))))
		return 0;
	if (*s == '*' || *s == '/')
		n = *s++;
	if ((*s == '*' || *s == '/') && *s++ == (const char)n)
		return 0;
	return (*s == '>') ? 1 : 0;
}

static inline int8_t	_sleep(const uint64_t ms, const delay_type type, ssize_t (*putc)(const char)) {
	static const char	*pad_char;
	struct timespec		alarm_clock;
	uint64_t			pads_needed;

	switch (type) {
		case NANOSLEEP:
			if (clock_gettime(CLOCK_MONOTONIC, &alarm_clock) == -1)
				return 0;
			errno = 0;
			alarm_clock.tv_nsec += ms * 1000000;
			while (alarm_clock.tv_nsec >= 1000000000) {
				alarm_clock.tv_nsec -= 1000000000;
				alarm_clock.tv_sec++;
			}
			while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &alarm_clock, NULL) != 0)
				if (errno != EINTR)
					return 0;
			break ;
		case PAD:
			if (!pad_char) {
				pad_char = str_cap(ti42_pad);
				if (pad_char == TI42_ABS_STR)
					pad_char = "\x0";
			}
			for (pads_needed = (ms * _get_baud_rate()) / 1000; pads_needed; pads_needed--) {
				if (putc(*pad_char) == -1)
					return 0;
			}
	}
	return 1;
}
