/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:29:21 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 13:28:15 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

static inline size_t	_intlen(int64_t n);

const char	*ft_ti_itoa(int64_t n)
{
	size_t	i;
	int8_t	neg;
	char	*out;

	if (n == INT64_MIN)
		return ft_ti_strdup("-9223372036854775808");
	i = _intlen(n);
	out = ft_ti_alloc((i + 1) * sizeof(*out));
	if (!out)
		return NULL;
	neg = n < 0;
	if (neg)
		n  = -n;
	for (; n > 9; n /= 10)
		out[--i] = (n % 10) + '0';
	out[--i] = n + '0';
	if (neg)
		out[i] = '-';
	return out;
}

const char	*ft_ti_strjoin(const char *s1, const char *s2)
{
	size_t	l1;
	size_t	l2;
	char	*out;

	if (!s1 || !s2)
		goto err;
	l1 = strlen(s1);
	l2 = strlen(s2);
	out = ft_ti_alloc(l1 + l2 + 1);
	if (!out)
		goto err;
	if (snprintf(out, l1 + l2, "%s%s", s1, s2) == -1)
		goto ferr;
	return out;
	ferr:
	ft_ti_flist_free(out);
	err:
	return NULL;
}

char	*ft_ti_strdup(const char *s)
{
	size_t	asize;
	char	*out;

	if (!s)
		return NULL;
	asize = strlen(s) + 1;
	out = ft_ti_alloc(asize);
	if (out)
		strcpy(out, s);
	return out;
}

static inline size_t	_intlen(int64_t n)
{
	size_t	digits;

	digits = 1;
	if (n < 0)
	{
		n = -((n == INT64_MIN) ? n - 1 : n);
		digits++;
	}
	for (; n > 9; n /= 10)
		digits++;
	return digits;
}
