/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:43:59 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/20 00:44:13 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "keys.h"

int	ft_mouse_down(int button, int x, int y, t_fdf *data)
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

int	ft_mouse_up(int button, int x, int y, t_fdf *data)
{
	(void)x;
	(void)y;
	if (button == LEFT_CLICK)
		data->mouse.is_pressed = FALSE;
	return (SUCCESS);
}

int	ft_mouse_move(int x, int y, t_fdf *data)
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
