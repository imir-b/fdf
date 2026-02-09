/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 04:36:29 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/09 04:44:37 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_camera	*ft_init_camera(t_object *obj)
{
	t_camera	*cam;

	cam = malloc(sizeof(t_camera));
	if (!cam)
		return (NULL);
	if (obj->width > 0)
		cam->zoom = WIN_WIDTH / obj->width / 2;
	else
		cam->zoom = 20;
	if (cam->zoom < 1)
		cam->zoom = 1;
	cam->angle_x = RADIAN_30;
	cam->angle_y = RADIAN_30;
	cam->z_scale = 1.0;
	cam->shift_x = WIN_WIDTH / 2;
	cam->shift_y = WIN_HEIGHT / 2;
	cam->projection = ISOMETRIC;
	return (cam);
}

int	ft_loop_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	ft_render_image(data);
	return (SUCCESS);
}

int	ft_process_fdf(t_object *obj, t_fbx *fbx)
{
	t_fdf		*data;
	t_camera	*cam;

	cam = ft_init_camera(obj);
	if (!cam)
		return (ERROR);
	data = ft_init_data(obj, cam, fbx);
	if (!data)
		return (ERROR);
	ft_render_image(data);
	ft_events(data);
	mlx_loop_hook(data->mlx_ptr, &ft_loop_hook, data);
	mlx_loop(data->mlx_ptr);
	ft_free_data(data);
	return (SUCCESS);
}
