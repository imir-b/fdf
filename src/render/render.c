/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/31 07:09:17 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

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

int	ft_key_hook(int keycode, t_fdf *data)
{
	if (keycode == ESC)
	{
		ft_close_window(data);
	}
	return (SUCCESS);
}

int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

void	ft_draw_line(t_fdf *data, t_point p1, t_point p2)
{
	t_pixel	pxl;
	int		diff_x;
	int		diff_y;
	int		step_x;
	int		step_y;
	int		err;

	diff_x = ft_absolute(p2.x - p1.x);
	diff_y = ft_absolute(p2.y - p1.y);
	pxl = (t_pixel){p1.x, p1.y};
	if (p1.x < p2.x)
		step_x = 1;
	else
		step_x = -1;
	if (p1.y < p2.x)
		step_y = 1;
	else
		step_y = -1;
	err = diff_x - diff_y;
	while (TRUE)
	{
		my_mlx_pixel_put(data, pxl, 0xFFFFFF);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		if (err * 2 > -diff_y)
		{
			err -= diff_y;
			pxl.x += step_x;
		}
		if (err * 2 < diff_x)
		{
			err += diff_x;
			pxl.y += step_y;
		}
		pxl.x++;
	}
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
