/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:18 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/28 04:42:06 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_count_words_sep(char *str, char sep)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (*str && *str != '\n')
		{
			count++;
			while (*str && *str != sep && *str != '\n')
				str++;
		}
		else if (*str == '\n')
			str++;
	}
	return (count);
}

static int	get_hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

int	ft_atoi_hexa(char *str)
{
	int	result;
	int	value;

	result = 0;
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
		str += 2;
	while (*str)
	{
		value = get_hex_value(*str);
		if (value == -1)
			break ;
		result = (result * 16) + value;
		str++;
	}
	return (result);
}

int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	ft_direction(int pos1, int pos2)
{
	if (pos1 < pos2)
		return (1);
	return (-1);
}

double	ft_atof(const char *s)
{
	double	res;
	double	fact;
	int		sign;

	res = 0.0;
	fact = 1.0;
	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	while (*s && *s != '.')
		res = res * 10.0 + (*s++ - '0');
	if (*s == '.')
		s++;
	while (*s)
	{
		fact /= 10.0;
		res += (*s++ - '0') * fact;
	}
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
