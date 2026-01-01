/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 01:55:35 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_draw_line(t_fdf *data, t_point p1, t_point p2)
{
	t_pixel		pixel;
	t_bresenham	graphics;
	int			err2;

	ft_transform(&p1, data);
	ft_transform(&p2, data);
	graphics = ft_init_graphics(p1, p2);
	while (TRUE)
	{
		pixel = (t_pixel){p1.x, p1.y};
		my_mlx_pixel_put(data, pixel, 0xFFFFFF);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		err2 = graphics.err * 2;
		if (err2 > -graphics.diff_y)
		{
			graphics.err -= graphics.diff_y;
			p1.x += graphics.step_x;
		}
		if (err2 < graphics.diff_x)
		{
			graphics.err += graphics.diff_x;
			p1.y += graphics.step_y;
		}
	}
}

static void	ft_draw_map(t_fdf *data)
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
				p2 = (t_point){p1.x + 1, p1.y, data->map->grid[p1.y][p1.x + 1]};
				ft_draw_line(data, p1, p2);
			}
			if (p1.y < data->map->height - 1)
			{
				p2 = (t_point){p1.x, p1.y + 1, data->map->grid[p1.y + 1][p1.x]};
				ft_draw_line(data, p1, p2);
			}
			p1.x++;
		}
		p1.y++;
	}
}

void	ft_render(t_fdf *data)
{
	ft_bzero(data->addr, WIN_WIDTH * WIN_HEIGHT * (data->bits_per_pixel / 8));
	ft_draw_map(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
}

int	ft_process_fdf(t_map *map)
{
	t_fdf		*data;
	t_camera	camera;

	camera = (t_camera){WIN_WIDTH / 2, WIN_HEIGHT / 2, 20, 1.0, \
		RADIAN_30, RADIAN_30};
	data = ft_init_data(map, &camera);
	if (!data)
		return (ERROR);
	ft_render(data);
	ft_events(data);
	mlx_loop(data->mlx_ptr);
	ft_free_data(data);
	return (SUCCESS);
}
