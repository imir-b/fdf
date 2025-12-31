/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/31 06:05:15 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

void	my_mlx_pixel_put(t_fdf *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < 1920 && y >= 0 && y < 1080)
	{
		dst = data->addr + (y * data->line_length + x * \
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

int	ft_key_hook(int keycode, t_fdf *data)
{
	if (keycode == ESC)
	{
		ft_close_window(data);
	}
	return (SUCCESS);
}

void	ft_draw_line(t_fdf *data, t_point p1, t_point p2)
{
	
}

void	ft_draw_map(t_fdf *data)
{
	t_point	p1;
	t_point	p2;

	p1.y = 0;
	while (p1.y < data->map->height)
	{
		p1.x = 0;
		while (p1.x < data->map->width)
		{
			p1.z = data->map->grid[p1.y][p1.x];
			if (p1.x < data->map->width - 1)
			{
				p2 = (t_point){p1.x + 1, p1.y, p1.z};
				ft_draw_line(data, p1, p2);
			}
			if (p1.y < data->map->height - 1)
			{
				p2 = (t_point){p1.x, p1.y + 1, p1.z};
				ft_draw_line(data, p1, p2);
			}
			p1.x++;
		}
		p1.y++;
	}
}

int	ft_process_map(t_map *map)
{
	t_fdf	*data;

	data = ft_init_data(map);
	if (!data)
		return (ERROR);
	ft_draw_map(data);
	mlx_hook(data->win_ptr, 2, 1L << 0, ft_key_hook, data);
	mlx_hook(data->win_ptr, 17, 0, ft_close_window, data);
	mlx_loop(data->mlx_ptr);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
	ft_free_data(data);
	return (SUCCESS);
}
