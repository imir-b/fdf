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

t_camera	*ft_init_camera(t_object *obj, t_fbx *fbx)
{
	t_camera	*cam;
	double		span;
	int			i;
	t_vec3		v;
	t_list		*curr;
	t_geometry	*geo;
	t_model		*mdl;

	cam = malloc(sizeof(t_camera));
	if (!cam)
		return (NULL);
	span = 0;
	if (obj->width > 0)
		cam->zoom = WIN_WIDTH / obj->width / 2;
	else if (fbx && fbx->geo)
	{
		curr = fbx->geo;
		while (curr)
		{
			geo = (t_geometry *)curr->content;
			mdl = find_model_for_geo(fbx->model, geo);
			if (geo->obj)
			{
				i = -1;
				while (++i < geo->obj->nb_vertices)
				{
					if (mdl)
						v = ft_get_world_transform(geo->obj->vertices[i], mdl);
					else
						v = geo->obj->vertices[i];
					if (fabs(v.x) > span)
						span = fabs(v.x);
					if (fabs(v.y) > span)
						span = fabs(v.y);
					if (fabs(v.z) > span)
						span = fabs(v.z);
				}
			}
			curr = curr->next;
		}
		if (span > 0)
			cam->zoom = (WIN_WIDTH / 3.0) / span;
		else
			cam->zoom = 20;
	}
	else if (obj->nb_vertices > 0)
	{
		i = 0;
		while (i < obj->nb_vertices)
		{
			if (fabs(obj->vertices[i].x) > span)
				span = fabs(obj->vertices[i].x);
			if (fabs(obj->vertices[i].y) > span)
				span = fabs(obj->vertices[i].y);
			if (fabs(obj->vertices[i].z) > span)
				span = fabs(obj->vertices[i].z);
			i++;
		}
		if (span > 0)
			cam->zoom = (WIN_WIDTH / 3.0) / span;
		else
			cam->zoom = 20;
	}
	else
		cam->zoom = 20;
	if (cam->zoom < 0.001)
		cam->zoom = 0.001;
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

	cam = ft_init_camera(obj, fbx);
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
