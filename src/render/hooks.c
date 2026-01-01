/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 22:38:00 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 17:12:38 by vbleskin         ###   ########.fr       */
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
		ft_render(data);
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
		data->camera->angle_x += (x - data->mouse.x) * 0.1;
		data->camera->angle_y += (y - data->mouse.y) * 0.1;
		data->mouse.x = x;
		data->mouse.y = y;
		ft_render(data);
	}
	return (SUCCESS);
}
#include <stdio.h>

static int	ft_num_hook(int keycode, t_fdf *data)
{
	if (keycode == NUM0)
	{
		data->camera->angle_x = 0;
		data->camera->angle_y = 0;
		data->camera->projection = ISOMETRIC;
	}
	if (keycode == NUM1 || keycode == NUM2 || keycode == NUM3 \
		|| keycode == NUM4 || keycode == NUM5 || keycode == NUM6)
		data->camera->projection = PARALLEL;
	if (keycode == NUM1)
	{
		data->camera->angle_x = 0;
		data->camera->angle_y = 0;
	}
	if (keycode == NUM2)
	{
		data->camera->angle_x = 0;
		data->camera->angle_y = RADIAN_90;
	}
	if (keycode == NUM3)
	{
		data->camera->angle_x = RADIAN_90;
		data->camera->angle_y = 0;
	}
	if (keycode == NUM4)
	{
		data->camera->angle_x = -RADIAN_90;
		data->camera->angle_y = -RADIAN_90;
	}
	if (keycode == NUM5)
	{
		data->camera->angle_x = -RADIAN_90;
		data->camera->angle_y = 0;
	}
	if (keycode == NUM6)
	{
		data->camera->angle_x = RADIAN_90;
		data->camera->angle_y = RADIAN_90;
	}
	return (SUCCESS);
}

static int	ft_key_hook(int keycode, t_fdf *data)
{
	printf("Touche appuyée : %d\n", keycode);
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
	ft_render(data);
	return (SUCCESS);
}

void	ft_events(t_fdf *data)
{
	mlx_hook(data->win_ptr, 2, 1L << 0, ft_key_hook, data);
	mlx_hook(data->win_ptr, 4, 1L << 2, ft_mouse_down, data);
	mlx_hook(data->win_ptr, 5, 1L << 3, ft_mouse_up, data);
	mlx_hook(data->win_ptr, 6, 1L << 6, ft_mouse_move, data);
	mlx_hook(data->win_ptr, 17, 0, ft_close_window, data);
}
