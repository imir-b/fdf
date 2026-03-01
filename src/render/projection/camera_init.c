/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:20:07 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:47:23 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * 
 */
static double	ft_get_obj_span(t_object *obj)
{
	double	span;
	int		i;

	span = 0;
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
	return (span);
}

static double	ft_calc_geo_span(t_geometry *geo, t_model *mdl, double span)
{
	t_vec3	v;
	int		i;

	i = -1;
	while (geo->obj && ++i < geo->obj->nb_vertices)
	{
		v = geo->obj->vertices[i];
		if (mdl)
			v = ft_get_world_transform(v, mdl);
		if (fabs(v.x) > span)
			span = fabs(v.x);
		if (fabs(v.y) > span)
			span = fabs(v.y);
		if (fabs(v.z) > span)
			span = fabs(v.z);
	}
	return (span);
}

static double	ft_get_fbx_span(t_fbx *fbx)
{
	t_list		*curr;
	t_geometry	*geo;
	t_model		*mdl;
	double		span;

	curr = fbx->geo;
	span = 0;
	while (curr)
	{
		geo = (t_geometry *)curr->content;
		mdl = find_model_for_geo(fbx->model, geo);
		span = ft_calc_geo_span(geo, mdl, span);
		curr = curr->next;
	}
	return (span);
}

/**
 * 
 */
static void	ft_set_cam_zoom(t_camera *cam, t_object *obj, t_fbx *fbx)
{
	double	span;

	if (obj->width > 0)
		cam->zoom = (WIN_WIDTH / obj->width) / 2;
	else if (fbx && fbx->geo)
	{
		span = ft_get_fbx_span(fbx);
		cam->zoom = 20;
		if (span > 0)
			cam->zoom = (WIN_WIDTH / 3.0) / span;
	}
	else if (obj->nb_vertices > 0)
	{
		span = ft_get_obj_span(obj);
		cam->zoom = 20;
		if (span > 0)
			cam->zoom = (WIN_WIDTH / 3.0) / span;
	}
	else
		cam->zoom = 20;
	if (cam->zoom < 0.001)
		cam->zoom = 0.001;
}

/**
 * Initialise la caméra (angles par défaut, position, zoom dynamique).
 * Le zoom est ajusté en fonction de la taille ("span") du modèle chargé
 * afin qu'il rentre correctement dans la fenêtre au lancement.
 */
t_camera	*ft_init_camera(t_object *obj, t_fbx *fbx)
{
	t_camera	*cam;

	cam = malloc(sizeof(t_camera));
	if (!cam)
		return (NULL);
	ft_set_cam_zoom(cam, obj, fbx);
	cam->angle_x = RADIAN_30;
	cam->angle_y = RADIAN_30;
	cam->z_scale = 1.0;
	cam->shift_x = WIN_WIDTH / 2;
	cam->shift_y = WIN_HEIGHT / 2;
	cam->projection = ISOMETRIC;
	return (cam);
}
