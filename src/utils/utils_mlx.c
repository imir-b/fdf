/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 18:23:51 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 19:18:40 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_fdf *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = data->img.addr + (y * data->img.line_length + x * \
			(data->img.bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

int	ft_close_window(t_fdf *data)
{
	ft_free_data(data);
	exit(SUCCESS);
	return (SUCCESS);
}
