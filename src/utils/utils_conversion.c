/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_conversion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:58:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 23:02:27 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_parse_decimal(const char **s, double res)
{
	double	fact;

	if (**s != '.')
		return (res);
	(*s)++;
	fact = 1.0;
	while (**s && ft_isdigit(**s))
	{
		fact /= 10.0;
		res += (**s - '0') * fact;
		(*s)++;
	}
	return (res);
}

static double	ft_parse_exponent(const char *s, double res)
{
	int	exp_sign;
	int	exp;

	s++;
	exp_sign = 1;
	if (*s == '-')
	{
		exp_sign = -1;
		s++;
	}
	else if (*s == '+')
		s++;
	exp = 0;
	while (*s && ft_isdigit(*s))
		exp = exp * 10 + (*s++ - '0');
	return (res * pow(10.0, exp_sign * exp));
}

double	ft_atof(const char *s)
{
	double	res;
	int		sign;

	res = 0.0;
	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	while (*s && ft_isdigit(*s))
		res = res * 10.0 + (*s++ - '0');
	res = ft_parse_decimal(&s, res);
	if (*s == 'e' || *s == 'E')
		res = ft_parse_exponent(s, res);
	return (res * sign);
}

long long	ft_atoll(const char *s)
{
	long long	ret;
	int			sign;

	ret = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		ret = ret * 10 + (*s - '0');
		s++;
	}
	return (ret * sign);
}

long	ft_atol(const char *s)
{
	long	ret;
	int		sign;

	ret = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		ret = ret * 10 + (*s - '0');
		s++;
	}
	return (ret * sign);
}
