/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 22:38:00 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/18 15:07:41 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "keys.h"

static int	ft_mouse_down(int button, int x, int y, t_fdf *data)
{
	if (button == LEFT_CLICK)
	{
		data->mouse.is_pressed = TRUE;
		data->mouse.x = x;
		data->mouse.y = y;
	}
	if (button == WHEEL_UP)
		data->camera->zoom += 1;
	else if (button == WHEEL_DOWN)
	{
		if (data->camera->zoom > 1)
			data->camera->zoom -= 1;
	}
	if (button == WHEEL_UP || button == WHEEL_DOWN)
		ft_render_image(data);
	return (SUCCESS);
}

static int	ft_mouse_up(int button, int x, int y, t_fdf *data)
{
	(void)x;
	(void)y;
	if (button == LEFT_CLICK)
		data->mouse.is_pressed = FALSE;
	return (SUCCESS);
}

static int	ft_mouse_move(int x, int y, t_fdf *data)
{
	if (data->mouse.is_pressed)
	{
		data->camera->angle_x += (x - data->mouse.x) * 0.005;
		data->camera->angle_y += (y - data->mouse.y) * 0.005;
		data->mouse.x = x;
		data->mouse.y = y;
		ft_render_image(data);
	}
	return (SUCCESS);
}

static int	ft_num_hook(int keycode, t_fdf *data)
{
	if (keycode == NUM0)
	{
		data->camera->angle_x = -0.615;
		data->camera->angle_y = -0.785;
		data->camera->projection = ISOMETRIC;
	}
	if (keycode == NUM1)
		ft_parallel_project(data, 0, 0);
	if (keycode == NUM2)
		ft_parallel_project(data, 0, RADIAN_90);
	if (keycode == NUM3)
		ft_parallel_project(data, RADIAN_90, 0);
	if (keycode == NUM4)
		ft_parallel_project(data, RADIAN_90, RADIAN_90);
	if (keycode == NUM5)
		ft_parallel_project(data, 0, -RADIAN_90);
	if (keycode == NUM6)
		ft_parallel_project(data, -RADIAN_90, 0);
	if (keycode == NUM7)
		ft_parallel_project(data, -RADIAN_90, -RADIAN_90);
	return (SUCCESS);
}

static int	ft_key_hook(int keycode, t_fdf *data)
{
	if (keycode == PLUS)
		data->camera->z_scale += 0.01;
	if (keycode == MINUS)
		data->camera->z_scale -= 0.01;
	if (keycode == ESC)
		ft_close_window(data);
	if (keycode == LEFT)
		data->camera->shift_x += 10;
	if (keycode == RIGHT)
		data->camera->shift_x -= 10;
	if (keycode == UP)
		data->camera->shift_y += 10;
	if (keycode == DOWN)
		data->camera->shift_y -= 10;
	ft_num_hook(keycode, data);
	ft_render_image(data);
	return (SUCCESS);
}

int	ft_refresh_image(t_fdf *data)
{
	if (data->win_ptr && data->img_ptr)
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, \
			data->img_ptr, 0, 0);
	return (SUCCESS);
}

void	ft_events(t_fdf *data)
{
	mlx_hook(data->win_ptr, 2, 1L << 0, ft_key_hook, data);
	mlx_hook(data->win_ptr, 4, 1L << 2, ft_mouse_down, data);
	mlx_hook(data->win_ptr, 5, 1L << 3, ft_mouse_up, data);
	mlx_hook(data->win_ptr, 6, 1L << 6, ft_mouse_move, data);
	mlx_hook(data->win_ptr, 12, 1L << 15, ft_refresh_image, data);
	mlx_hook(data->win_ptr, 17, 0, ft_close_window, data);
}
