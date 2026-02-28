/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_anim_at_time.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:34:58 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:35:55 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_process_geo_vertices(t_fdf *data, t_geometry *geo, int *g_idx)
{
	t_model	*model;
	t_vec3	new_pos;
	int		i;

	model = find_model_for_geo(data->fbx->model, geo);
	i = 0;
	while (i < geo->obj->nb_vertices)
	{
		new_pos = ft_get_new_pos(geo, model, i);
		data->object->vertices[*g_idx + i] = new_pos;
		i++;
	}
	*g_idx += geo->obj->nb_vertices;
}

/**
 * Met à jour les vertices du maillage à partir des animations.
 * Pour les modèles avec deformers (skinning squelettique),
 * applique la formule de skinning par vertex.
 * Pour les modèles sans deformers, applique le transform rigide.
 */
void	ft_update_mesh_from_animation(t_fdf *data)
{
	t_list		*curr_geo;
	t_geometry	*geo;
	int			global_index;

	if (!data->fbx || !data->object)
		return ;
	global_index = 0;
	curr_geo = data->fbx->geo;
	while (curr_geo)
	{
		geo = (t_geometry *)curr_geo->content;
		if (geo->obj)
			ft_process_geo_vertices(data, geo, &global_index);
		curr_geo = curr_geo->next;
	}
}

/**
 * 
 */
void	ft_reset_models_to_base(t_fdf *data)
{
	t_list	*models;
	t_model	*mdl;

	if (!data || !data->fbx || !data->fbx->model)
		return ;
	models = data->fbx->model;
	while (models)
	{
		mdl = (t_model *)models->content;
		mdl->pos = mdl->base_pos;
		mdl->rot = mdl->base_rot;
		mdl->scale = mdl->base_scale;
		models = models->next;
	}
}
