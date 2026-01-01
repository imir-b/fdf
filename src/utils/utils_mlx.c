/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 18:23:51 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 19:46:51 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_fdf *data, t_pixel pxl, int color)
{
	char	*dst;

	if (pxl.x >= 0 && pxl.x < WIN_WIDTH && pxl.y >= 0 && pxl.y < WIN_HEIGHT)
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
