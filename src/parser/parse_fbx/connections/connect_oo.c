/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_oo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:58:17 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:48:21 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_geometry	*ft_find_geo_with_deformer(t_fbx *data, long skin_id)
{
	t_list	*geos;
	t_list	*defs;

	geos = data->geo;
	while (geos)
	{
		defs = ((t_geometry *)geos->content)->deformers;
		while (defs)
		{
			if (((t_deformer *)defs->content)->id == skin_id)
				return ((t_geometry *)geos->content);
			defs = defs->next;
		}
		geos = geos->next;
	}
	return (NULL);
}

static int	ft_try_connect_geo_model(t_fbx *data, long *ids)
{
	void	*src;
	void	*dst;

	dst = ft_get_by_id(data->model, ids[1]);
	if (!dst)
		return (FALSE);
	src = ft_get_by_id(data->geo, ids[0]);
	if (src)
	{
		((t_model *)dst)->geo = (t_geometry *)src;
		return (1);
	}
	src = ft_get_by_id(data->model, ids[0]);
	if (src)
	{
		((t_model *)src)->parent = (t_model *)dst;
		return (TRUE);
	}
	return (FALSE);
}

static int	ft_try_connect_anim(t_fbx *data, long *ids)
{
	void	*src;
	void	*dst;

	src = ft_get_by_id(data->anim_layer, ids[0]);
	if (src)
	{
		dst = ft_get_by_id(data->anim_stack, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_stack *)dst)->layers, ft_lstnew(src));
		return (TRUE);
	}
	src = ft_get_by_id(data->anim_node, ids[0]);
	if (src)
	{
		dst = ft_get_by_id(data->anim_layer, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_layer *)dst)->nodes, ft_lstnew(src));
		return (TRUE);
	}
	return (FALSE);
}

static int	ft_try_connect_def_bone(t_fbx *data, long *ids)
{
	void		*src;
	void		*dst;
	t_geometry	*geo;

	src = ft_get_by_id(data->deformer, ids[0]);
	if (src)
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
		return (TRUE);
	}
	src = ft_get_by_id(data->model, ids[0]);
	dst = ft_get_by_id(data->deformer, ids[1]);
	if (!src || !dst)
		return (FALSE);
	((t_deformer *)dst)->bone = (t_model *)src;
	return (TRUE);
}

void	ft_connect_obj_to_obj(t_fbx *data, long *ids)
{
	if (ft_try_connect_geo_model(data, ids))
		return ;
	if (ft_try_connect_anim(data, ids))
		return ;
	ft_try_connect_def_bone(data, ids);
}
