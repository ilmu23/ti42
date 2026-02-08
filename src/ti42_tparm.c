// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ti42_tparm.c>>

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "internal/ti42_internal.h"

#define _SVAR_MAX		26
#define _DVAR_MAX		26
#define _STACK_SIZE		64
#define _PARAM_COUNT	9

#define _BUFFER_SIZE	4096

#define _FLAG_DVARS_INIT_DONE	0x1U
#define _FLAG_INCREMENT_P12		0x2U
#define _FLAG_IN_CONDITIONAL	0x4U

#define flag_set(flags, f)		(flags |= f)
#define flag_unset(flags, f)	(flags &= ~f)
#define flag_is_set(flags, f)	(flags & f)

#define stack_pop(stk)		(stk.data[(stk.size != 0) ? --stk.size : stk.size])
#define stack_push(stk, x)	(stk.data[(stk.size != _STACK_SIZE - 1) ? stk.size++ : stk.size] = x)

static inline uint8_t	_sprintf(const char **seq, char buf[_BUFFER_SIZE + 1], const uintptr_t val);

const char	*ti42_tparm(const char *seq, const uintptr_t arg1, const uintptr_t arg2, const uintptr_t arg3, const uintptr_t arg4, const uintptr_t arg5, const uintptr_t arg6, const uintptr_t arg7, const uintptr_t arg8, const uintptr_t arg9) {
	unsigned _BitInt(3)	flags;
	static uintptr_t	svars[_SVAR_MAX];
	static char			seq_buf[_BUFFER_SIZE + 1];
	uintptr_t			dvars[_DVAR_MAX];
	uintptr_t			params[_PARAM_COUNT];
	struct {
		uintptr_t		data[_STACK_SIZE];
		size_t			size;
	}					stack;
	uintptr_t			x;
	uintptr_t			y;
	size_t				i;
	char				buf[_BUFFER_SIZE + 1];

	if (seq == TI42_ABS_STR || seq == TI42_NOT_STR)
		return NULL;
	stack.size = 0;
	params[0] = arg1;
	params[1] = arg2;
	params[2] = arg3;
	params[3] = arg4;
	params[4] = arg5;
	params[5] = arg6;
	params[6] = arg7;
	params[7] = arg8;
	params[8] = arg9;
	memset(seq_buf, 0, sizeof(seq_buf));
	for (i = flags = 0; *seq && i < _BUFFER_SIZE; seq++) {
		if (*seq == '%') switch (*(++seq)) {
			case 'c':
				seq_buf[i++] = (uint8_t)stack_pop(stack);
				break ;
			case 'd':
			case 'o':
			case 'x':
			case 'X':
			case 's':
			case ':':
			case ' ':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (!_sprintf(&seq, buf, stack_pop(stack)))
					goto _ti42_tparm_err_ret;
				i = strlcat(seq_buf, buf, _BUFFER_SIZE + 1);
				if (i >= sizeof(seq_buf))
					goto _ti42_tparm_err_ret;
				break ;
			case 'i':
				if (!flag_is_set(flags, _FLAG_INCREMENT_P12)) {
					params[0]++;
					params[1]++;
					flag_set(flags, _FLAG_INCREMENT_P12);
				}
				break ;
			case 'p':
				seq++;
				if (in_range(*seq, '1', '9'))
					stack_push(stack, params[*seq - '1']);
				else
					goto _ti42_tparm_err_ret;
				break ;
			case 'g':
				if (islower(*(++seq))) {
					if (!flag_is_set(flags, _FLAG_DVARS_INIT_DONE)) {
						memset(dvars, 0, sizeof(dvars));
						flag_set(flags, _FLAG_DVARS_INIT_DONE);
					}
					stack_push(stack, dvars[*seq - 'a']);
				} else
					stack_push(stack, svars[*seq - 'A']);
				break ;
			case '\'':
				stack_push(stack, *(++seq));
				seq++;
				break ;
			case '{':
				for (x = 0, seq++; isdigit(*seq); seq++)
					x = x * 10 + *seq - '0';
				stack_push(stack, x);
				break ;
			case 'l':
				x = strlen((const char *)stack_pop(stack));
				stack_push(stack, x);
				break ;
			case '+':
			case '-':
			case '*':
			case '/':
			case 'm':
			case '|':
			case '^':
			case '=':
			case '>':
			case '<':
			case 'A':
			case 'O':
				x = stack_pop(stack);
				y = stack_pop(stack);
				switch (*seq) {
					case '+':
						stack_push(stack, y + x);
						break ;
					case '-':
						stack_push(stack, y - x);
						break ;
					case '*':
						stack_push(stack, y * x);
						break ;
					case '/':
						stack_push(stack, y / x);
						break ;
					case 'm':
						stack_push(stack, y % x);
						break ;
					case '|':
						stack_push(stack, y | x);
						break ;
					case '^':
						stack_push(stack, y ^ x);
						break ;
					case '=':
						stack_push(stack, y == x);
						break ;
					case '>':
						stack_push(stack, y > x);
						break ;
					case '<':
						stack_push(stack, y < x);
						break ;
					case 'A':
						stack_push(stack, y && x);
						break ;
					case 'O':
						stack_push(stack, y || x);
				}
				break ;
			case '!':
			case '~':
				x = stack_pop(stack);
				stack_push(stack, (*seq != '!') ? !x : ~x);
				break ;
			case '?':
				if (flag_is_set(flags, _FLAG_IN_CONDITIONAL))
					goto _ti42_tparm_err_ret;
				flag_set(flags, _FLAG_IN_CONDITIONAL);
				break ;
			case 't':
				if (!flag_is_set(flags, _FLAG_IN_CONDITIONAL))
					goto _ti42_tparm_err_ret;
				x = stack_pop(stack);
				if (!x) {
					for (seq++, y = 0; ; seq++) {
						if (*seq == '%') switch (*(++seq)) {
							case '?':
								y++;
								break ;
							case ';':
							case 'e':
								if (y == 0) {
									if (*seq == ';')
										flag_unset(flags, _FLAG_IN_CONDITIONAL);
									goto _ti42_tparm_continue;
								}
								y -= (*seq == ':') ? 1 : 0;
								break ;
							case '\x0':
								goto _ti42_tparm_continue;
						}
					}
				}
				break ;
			case 'e':
				if (!flag_is_set(flags, _FLAG_IN_CONDITIONAL))
					goto _ti42_tparm_err_ret;
				for (seq++, x = 0; ; seq++) {
					if (*seq == '%') switch (*(++seq)) {
						case '?':
							x++;
							break ;
						case ';':
							if (x == 0) {
								flag_unset(flags, _FLAG_IN_CONDITIONAL);
								goto _ti42_tparm_continue;
							}
							x--;
						case '\x0':
							goto _ti42_tparm_continue;
					}
				}
				break;
			case ';':
				if (!flag_is_set(flags, _FLAG_IN_CONDITIONAL))
					goto _ti42_tparm_err_ret;
				flag_unset(flags, _FLAG_IN_CONDITIONAL);
				break ;
			case '%':
				seq_buf[i++] = '%';
		} else
			seq_buf[i++] = *seq;
_ti42_tparm_continue:
		;
	}
	seq_buf[i] = '\x0';
	return (i < _BUFFER_SIZE) ? seq_buf : NULL;
_ti42_tparm_err_ret:
	return NULL;
}

static inline uint8_t	_sprintf(const char **seq, char buf[_BUFFER_SIZE + 1], const uintptr_t val) {
	const char	*tmp;
	ssize_t		rv;
	size_t		i;
	char		fmt[_BUFFER_SIZE + 1];

	tmp = *seq;
	if (*tmp == ':')
		tmp++;
	for (i = 1, *fmt = '%'; i < _BUFFER_SIZE && strchr("-*# 0123456789.", *tmp) && *tmp; tmp++)
		fmt[i] = *tmp;
	if (i == _BUFFER_SIZE)
		return 0;
	fmt[i++] = *tmp;
	fmt[i] = '\x0';
	switch (fmt[i - 1]) {
		case 'd':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const int32_t)val);
			break ;
		case 'o':
		case 'x':
		case 'X':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const uint32_t)val);
			break ;
		case 's':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const char *)val);
			break ;
		default:
			rv = 0;
	}
	*seq = tmp;
	return (rv != -1 && rv < _BUFFER_SIZE) ? 1 : 0;
}
