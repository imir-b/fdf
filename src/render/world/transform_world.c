/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_world.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:00:57 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:53:18 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * 
 */
t_model	*find_model_for_geo(t_list *models, t_geometry *target_geo)
{
	t_model	*model;

	while (models)
	{
		model = (t_model *)models->content;
		if (model->geo == target_geo)
			return (model);
		models = models->next;
	}
	return (NULL);
}

/**
 * 
 */
static void	ft_scale(t_vec3 *p, t_properties s)
{
	t_transform_data	d;

	d.sx = 1.0;
	d.sy = 1.0;
	d.sz = 1.0;
	if (s.x != 0.0)
		d.sx = s.x;
	if (s.y != 0.0)
		d.sy = s.y;
	if (s.z != 0.0)
		d.sz = s.z;
	p->x *= d.sx;
	p->y *= d.sy;
	p->z *= d.sz;
}

static void	ft_rotate(t_vec3 *p, t_properties r)
{
	t_transform_data	d;
	double				tmp;

	d.rx = to_rad(r.x);
	d.ry = to_rad(r.y);
	d.rz = to_rad(r.z);
	tmp = p->y * cos(d.rx) - p->z * sin(d.rx);
	p->z = p->y * sin(d.rx) + p->z * cos(d.rx);
	p->y = tmp;
	tmp = p->x * cos(d.ry) + p->z * sin(d.ry);
	p->z = -p->x * sin(d.ry) + p->z * cos(d.ry);
	p->x = tmp;
	tmp = p->x * cos(d.rz) - p->y * sin(d.rz);
	p->y = p->x * sin(d.rz) + p->y * cos(d.rz);
	p->x = tmp;
}

t_vec3	apply_transform(t_vec3 point, t_properties p, t_properties r,
	t_properties s)
{
	ft_scale(&point, s);
	ft_rotate(&point, r);
	point.x += p.x;
	point.y += p.y;
	point.z += p.z;
	return (point);
}

t_vec3	ft_get_world_transform(t_vec3 point, t_model *model)
{
	t_vec3	curr_point;

	curr_point = point;
	while (model)
	{
		curr_point = apply_transform(curr_point, model->pos, model->rot,
				model->scale);
		model = model->parent;
	}
	return (curr_point);
}
