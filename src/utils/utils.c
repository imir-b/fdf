/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:18 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/31 23:39:40 by vbleskin         ###   ########.fr       */
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

void	my_mlx_pixel_put(t_fdf *data, t_pixel pxl, int color)
{
	char	*dst;

	if (pxl.x >= 0 && pxl.x < 1920 && pxl.y >= 0 && pxl.y < 1080)
	{
		dst = data->addr + (pxl.y * data->line_length + pxl.x * \
			(data->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

int	ft_close_window(t_fdf *data)
{
	ft_free_data(data);
	exit(SUCCESS);
	return (SUCCESS);
}

int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	ft_direction(int x1, int x2)
{
	if (x1 < x2)
		return (1);
	return (-1);
}
