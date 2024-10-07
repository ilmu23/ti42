/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tparm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:09:31 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/07 19:08:14 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define _PRMCOUNT	9
#define _VARCOUNT	26

#define	_STACKSIZE	50
#define _BUFSIZE	1024

#define _IOUT		i[0]
#define _ISEQ		i[1]
#define _ISTK		i[2]
#define _IPRM		i[3]
#define _ITMP		i[4]

#define _N1			numbers[0]
#define _N2			numbers[1]

#define _P1			0x001U
#define _P2			0x002U
#define _P3			0x004U
#define _P4			0x008U
#define _P5			0x010U
#define _P6			0x020U
#define _P7			0x040U
#define _P8			0x080U
#define _P9			0x100U

#define _DVARS_INIT	0x1U
#define _PRM_INCRMT	0x2U

#define	init_i		(_IOUT = 0, _ISEQ = 0, _ISTK = 0, _IPRM = 0, _ITMP = 0)
#define	init_dvars	(memset(dvars, 0, sizeof(dvars)), set(_DVARS_INIT))

#define addparam(x)	((__params |= x), (*prmc)++)
#define nparam(x)	((x < _PRMCOUNT) ? x++ : x)

#define set(x)		(flags |= x)
#define is_set(x)	(flags & x)

#define push(x)		((_ISTK < _STACKSIZE) ? stack[_ISTK++] = (uintptr_t)x : x)
#define pop			((_ISTK > 0) ? stack[--_ISTK] : stack[_ISTK])

static size_t		i[5];
static uintptr_t	svars[_VARCOUNT];

static inline const char	*_expand(const char *seq, const uintptr_t arg);
static inline uint8_t		_paramcount(const char *seq, uint8_t * const prmc);

const char		*ft_ti_tparm(const char *seq, ...)
{
	size_t		len;
	va_list		args;
	uint8_t		prmc;
	uint8_t		flags;
	uintptr_t	numbers[2];
	uintptr_t	params[_PRMCOUNT];
	uintptr_t	stack[_STACKSIZE];
	uintptr_t	dvars[_VARCOUNT];
	const char	*tmp;
	static char	out[_BUFSIZE];

	if (!_paramcount(seq, &prmc))
		return NULL;
	va_start(args, seq);
	for (_IPRM = 0; _IPRM < prmc; _IPRM++)
		params[_IPRM] = va_arg(args, uintptr_t);
	va_end(args);
	for (init_i, flags = 0; seq[_ISEQ] && _IOUT < _BUFSIZE; _ISEQ++)
	{
		if (seq[_ISEQ] == '%')
		{
			switch (seq[++_ISEQ])
			{
				case 'c':
					out[_IOUT++] = (uint8_t)pop;
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
					tmp = _expand(seq, pop);
					if (!tmp)
						return NULL;
					for (len = strlen(tmp), _ITMP = 0; _ITMP < len && _IOUT < _BUFSIZE; _IOUT++, _ITMP++)
						out[_IOUT] = tmp[_ITMP];
					ft_ti_flist_free(tmp);
					break ;
				case 'i':
					if (!is_set(_PRM_INCRMT))
					{
						params[0]++;
						params[1]++;
						set(_PRM_INCRMT);
					}
					break ;
				case 'p':
					_ISEQ++;
					if (isdigit(seq[_ISEQ]) && seq[_ISEQ] != '0')
						push(params[seq[_ISEQ] - '1']);
					else
						push(params[nparam(_IPRM)]);
					break ;
				case 'P':
					if (islower(seq[++_ISEQ]))
						dvars[seq[_ISEQ] - 'a'] = (!is_set(_DVARS_INIT)) ? init_dvars, pop :  pop;
					else
						svars[seq[_ISEQ] - 'A'] = pop;
					break ;
				case 'g':
					if (islower(seq[++_ISEQ]))
						push(((!is_set(_DVARS_INIT)) ? init_dvars, dvars[seq[_ISEQ] - 'a'] : dvars[seq[_ISEQ] - 'a']));
					else
						push(svars[seq[_ISEQ] - 'A']);
					break ;
				case '\'':
					push((uint8_t)seq[++_ISEQ]);
					_ISEQ++;
					break ;
				case '{':
					for (_N1 = 0, _ISEQ++; isdigit(seq[_ISEQ]); _ISEQ++)
						_N1 = _N1 * 10 + (seq[_ISEQ] -'0');
					push(_N1);
					break ;
				case 'l':
					len = strlen((const char *)pop);
					push(len);
					break ;
				case '+':
					_N2 = pop;
					_N1 = pop;
					push(_N1 + _N2);
					break ;
				case '-':
					_N2 = pop;
					_N1 = pop;
					push(_N1 - _N2);
					break ;
				case '*':
					_N2 = pop;
					_N1 = pop;
					push(_N1 * _N2);
					break ;
				case '/':
					_N2 = pop;
					_N1 = pop;
					push(_N1 / _N2);
					break ;
				case 'm':
					_N2 = pop;
					_N1 = pop;
					push(_N1 % _N2);
					break ;
				case '&':
					_N2 = pop;
					_N1 = pop;
					push(_N1 & _N2);
					break ;
				case '|':
					_N2 = pop;
					_N1 = pop;
					push(_N1 | _N2);
					break ;
				case '^':
					_N2 = pop;
					_N1 = pop;
					push(_N1 ^ _N2);
					break ;
				case '=':
					_N2 = pop;
					_N1 = pop;
					push(_N1 == _N2);
					break ;
				case '>':
					_N2 = pop;
					_N1 = pop;
					push(_N1 > _N2);
					break ;
				case '<':
					_N2 = pop;
					_N1 = pop;
					push(_N1 < _N2);
					break ;
				case 'A':
					_N2 = pop;
					_N1 = pop;
					push(_N1 && _N2);
					break ;
				case 'O':
					_N2 = pop;
					_N1 = pop;
					push(_N1 || _N2);
					break ;
				case '!':
					_N1 = pop;
					push(!_N1);
					break ;
				case '~':
					_N1 = pop;
					push(~_N1);
					break ;
				case '?':
					break ;
				case 't':
					_N1 = pop;
					if (!_N1)
						for (_ISEQ++, _N2 = 0; seq[_ISEQ]; _ISEQ++)
						{
							if (seq[_ISEQ] == '%')
							{
								if (seq[++_ISEQ] == '?')
									_N2++;
								else if (seq[_ISEQ] == ';')
								{
									if (_N2 == 0)
										break ;
									_N2--;
								}
								else if (seq[_ISEQ] == 'e' && _N2 == 0)
									break ;
							}
						}
					break ;
				case 'e':
					for (_ISEQ++, _N2 = 0; seq[_ISEQ]; _ISEQ++)
					{
						if (seq[_ISEQ] == '%')
						{
							if (seq[++_ISEQ] == '?')
								_N2++;
							else if (seq[_ISEQ] == ';')
							{
								if (_N2 == 0)
									break ;
								_N2--;
							}
						}
					}
					break ;
				case ';':
					break ;
				case '%':
					out[_IOUT++] = '%';
			}
		}
		else
			out[_IOUT++] = seq[_ISEQ];
	}
	out[_IOUT] = '\0';
	return out;
}

static inline const char	*_expand(const char *seq, const uintptr_t val)
{
	char	c;
	char	*fmt;
	char	out[_BUFSIZE];

	_ISEQ += (seq[_ISEQ] == ':') ? 1 : 0;
	fmt = (char *)&seq[_ISEQ - 1];
	for (_ITMP = 1; strchr("-+# 0123456789.", seq[_ISEQ]) && seq[_ISEQ]; _ISEQ++, _ITMP++)
		;
	c = fmt[_ITMP + 1];
	fmt[_ITMP + 1] = '\0';
	switch (seq[_ISEQ])
	{
		case 'd':
			snprintf(out, _BUFSIZE, fmt, (const int32_t)val);
			break ;
		case 'o':
		case 'x':
		case 'X':
			snprintf(out, _BUFSIZE, fmt, (const uint32_t)val);
			break ;
		case 's':
			snprintf(out, _BUFSIZE, fmt, (const char *)val);
			break ;
	}
	fmt[_ITMP + 1] = c;
	return ft_ti_strdup(out);
}

static inline uint8_t	_paramcount(const char *seq, uint8_t * const prmc)
{
	int16_t	__params;

	for (*prmc = 0, __params = 0; *seq; seq++)
	{
		if (*seq == 'p')
		{
			switch (*(++seq))
			{
				case '1':
					if (!(__params & _P1))
						addparam(_P1);
					break ;
				case '2':
					if (!(__params & _P2))
						addparam(_P2);
					break ;
				case '3':
					if (!(__params & _P3))
						addparam(_P3);
					break ;
				case '4':
					if (!(__params & _P4))
						addparam(_P4);
					break ;
				case '5':
					if (!(__params & _P5))
						addparam(_P5);
					break ;
				case '6':
					if (!(__params & _P6))
						addparam(_P6);
					break ;
				case '7':
					if (!(__params & _P7))
						addparam(_P7);
					break ;
				case '8':
					if (!(__params & _P8))
						addparam(_P8);
					break ;
				case '9':
					if (!(__params & _P9))
						addparam(_P9);
					break ;
				default:
					(*prmc)++;
			}
		}
	}
	*prmc = (*prmc <= _PRMCOUNT) ? *prmc : _PRMCOUNT;
	return (*prmc > 0) ? 1 : 0;
}
