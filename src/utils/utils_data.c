/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 22:45:09 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/18 00:02:54 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_bresenham	ft_init_graphics(t_point p1, t_point p2)
{
	t_bresenham	graphics;

	graphics.diff_x = ft_absolute(p2.x - p1.x);
	graphics.diff_y = ft_absolute(p2.y - p1.y);
	graphics.step_x = ft_direction(p1.x, p2.x);
	graphics.step_y = ft_direction(p1.y, p2.y);
	graphics.err = graphics.diff_x - graphics.diff_y;
	return (graphics);
}

void	*ft_free_object(t_object *obj)
{
	int	i;

	if (!obj)
		return (NULL);
	if (obj->faces)
	{
		i = 0;
		while (i < obj->nb_faces)
		{
			if (obj->faces[i].indices)
				free(obj->faces[i].indices);
			i++;
		}
		free(obj->faces);
	}
	if (obj->vertices)
		free(obj->vertices);
	if (obj)
		free(obj);
	return (NULL);
}

void	*ft_free_data(t_fdf *data)
{
	if (data->img_ptr)
		mlx_destroy_image(data->mlx_ptr, data->img_ptr);
	if (data->win_ptr)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	if (data->mlx_ptr)
	{
		mlx_destroy_display(data->mlx_ptr);
		free(data->mlx_ptr);
	}
	if (data)
	{
		free(data);
		data = NULL;
	}
	return (NULL);
}

t_fdf	*ft_init_data(t_object *obj, t_camera *camera)
{
	t_fdf	*data;

	data = ft_calloc(1, sizeof(t_fdf));
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
	data->object = obj;
	data->camera = camera;
	data->mouse.is_pressed = FALSE;
	data->mouse.x = 0;
	data->mouse.y = 0;
	return (data);
}
