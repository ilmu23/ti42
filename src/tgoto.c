/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tgoto.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:56:48 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/07 19:02:53 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

#define _FLAG_I		0x1U

#define _STACKSIZE	50
#define _BUFSIZE	1024

#define _IOUT		i[0]
#define _ISEQ		i[1]
#define _IPRM		i[2]
#define _INUM		i[3]

#define _INIT_I		_IOUT = 0, _ISEQ = 0, _IPRM = 0, _INUM = 0

#define iparam(x)	((x == '1') ? row : ((x == '2') ? col : -1))
#define push(x)		((_IPRM < _STACKSIZE) ? params[_IPRM++] = (intptr_t)x : x)
#define pop(x)		((_IPRM > 0) ? (x)params[--_IPRM] : (x)params[_IPRM])

const char	*ft_ti_tgoto(const char *seq, const int32_t row, const int32_t col)
{
	size_t		i[4];
	size_t		len;
	uint8_t		flags;
	intptr_t	params[_STACKSIZE];
	const char	*n;
	static char	out[_BUFSIZE];

	for (_INIT_I, flags = 0; seq[_ISEQ] && _IOUT < _BUFSIZE; _ISEQ++)
	{
		if (seq[_ISEQ] == '%')
		{
			switch ((seq)[++_ISEQ])
			{
				case 'i':
					flags |= _FLAG_I;
					break ;
				case 'p':
					++_ISEQ;
					push(iparam(seq[_ISEQ]));
					break ;
				case 'd':
					n = ft_ti_itoa(pop(int32_t) + ((flags & _FLAG_I) ? 1 : 0));
					if (!n)
						return NULL;
					for (len = strlen(n), _INUM = 0; _INUM < len && _IOUT < _BUFSIZE; _IOUT++, _INUM++)
						out[_IOUT] = n[_INUM];
					ft_ti_flist_free(n);
					break ;
			}
		}
		else
			out[_IOUT++] = seq[_ISEQ];
	}
	out[_IOUT] = '\0';
	return out;
}
