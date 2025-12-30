/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/30 12:47:09 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

void	*ft_free_data(t_fdf *data)
{
	if (data->mlx_ptr)
		free(data->mlx_ptr);
	if (data)
	{
		free(data);
		data = NULL;
	}
	return (NULL);
}

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

t_fdf	*ft_init_data(t_map *map)
{
	t_fdf	*data;

	data = malloc(sizeof(t_fdf));
	if (!data)
		return (NULL);
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (ft_free_data(data));
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, \
		"FDF vbleskin");
	if (!data->win_ptr)
		return (ft_free_data(data));
	data->img_ptr = mlx_new_image(data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	data->addr = mlx_get_data_addr(data->img_ptr, &data->bits_per_pixel, \
		&data->line_length, &data->endian);
	data->map = map;
	return (data);
}

void	ft_draw_map(t_fdf *data)
{
	(void)data;
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
