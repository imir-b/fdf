/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:18 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 23:00:32 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

int	ft_direction(int pos1, int pos2)
{
	if (pos1 < pos2)
		return (1);
	return (-1);
}

int	ft_is_tag(const char *str, const char *tag)
{
	if (!str || !tag)
		return (0);
	return (!ft_strncmp(str, tag, ft_strlen(tag)));
}
