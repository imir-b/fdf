/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:58:17 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:19:04 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_geometry	*ft_find_geo_with_deformer(t_fbx *data, long skin_id)
{
	t_list		*geos;
	t_geometry	*geo;
	t_list		*defs;
	t_deformer	*def;

	geos = data->geo;
	while (geos)
	{
		geo = (t_geometry *)geos->content;
		defs = geo->deformers;
		while (defs)
		{
			def = (t_deformer *)defs->content;
			if (def->id == skin_id)
				return (geo);
			defs = defs->next;
		}
		geos = geos->next;
	}
	return (NULL);
}

void	ft_connect_obj_to_obj(t_fbx *data, long *ids)
{
	void		*src;
	void		*dst;
	t_geometry	*geo;

	src = ft_get_by_id(data->geo, ids[0]);
	dst = ft_get_by_id(data->model, ids[1]);
	if (src && dst)
		((t_model *)dst)->geo = (t_geometry *)src;
	else if ((src = ft_get_by_id(data->model, ids[0])) && dst)
		((t_model *)src)->parent = (t_model *)dst;
	else if ((src = ft_get_by_id(data->anim_layer, ids[0])))
	{
		dst = ft_get_by_id(data->anim_stack, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_stack *)dst)->layers, ft_lstnew(src));
	}
	else if ((src = ft_get_by_id(data->anim_node, ids[0])))
	{
		dst = ft_get_by_id(data->anim_layer, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_layer *)dst)->nodes, ft_lstnew(src));
	}
	else if ((src = ft_get_by_id(data->deformer, ids[0])))
	{
		dst = ft_get_by_id(data->geo, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_geometry *)dst)->deformers, ft_lstnew(src));
		else if (ft_get_by_id(data->deformer, ids[1]))
		{
			geo = ft_find_geo_with_deformer(data, ids[1]);
			if (geo)
				ft_lstadd_front(&geo->deformers, ft_lstnew(src));
		}
	}
	else if ((src = ft_get_by_id(data->model, ids[0])))
	{
		dst = ft_get_by_id(data->deformer, ids[1]);
		if (dst)
			((t_deformer *)dst)->bone = (t_model *)src;
	}
}

void	ft_connect_anim_to_model(t_fbx *data, char *line, long *ids)
{
	t_anim_node	*anim;
	t_model		*model;

	anim = (t_anim_node *)ft_get_by_id(data->anim_node, ids[0]);
	if (anim)
	{
		model = (t_model *)ft_get_by_id(data->model, ids[1]);
		if (model)
		{
			anim->target = model;
			line += 4;
			if (IS_TAG(line, "Translation"))
				model->anim_pos = anim;
			else if (IS_TAG(line, "Rotation"))
				model->anim_rot = anim;
			else if (IS_TAG(line, "Scaling"))
				model->anim_scale = anim;
		}
	}
}

void	ft_connect_anim_to_anim(t_fbx *data, char *line, long *ids)
{
	t_anim_curve	*anim_curve;
	t_anim_node		*anim_node;

	anim_curve = (t_anim_curve *)ft_get_by_id(data->anim_curve, ids[0]);
	if (anim_curve)
	{
		anim_node = (t_anim_node *)ft_get_by_id(data->anim_node, ids[1]);
		if (anim_node)
		{
			line += 2;
			if (IS_TAG(line, "X"))
				anim_node->x = anim_curve;
			else if (IS_TAG(line, "Y"))
				anim_node->y = anim_curve;
			else if (IS_TAG(line, "Z"))
				anim_node->z = anim_curve;
		}
	}
}
