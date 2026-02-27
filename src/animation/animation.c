/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/02/27 13:25:42 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_get_value_at_time(t_anim_curve *curve, double current_time)
{
	int			i;
	double		t;
	double		t_start;
	double		t_end;

	if (!curve)
		return (0.0);
	if (curve->n_keys == 0 || !curve->time || !curve->value)
		return (0.0);
	if (curve->n_keys == 1)
		return (curve->value[0]);
	if (current_time >= curve->time[curve->n_keys - 1])
		return (curve->value[curve->n_keys - 1]);
	i = 0;
	while (i < curve->n_keys - 1)
	{
		if (current_time < curve->time[i + 1])
			break ;
		i++;
	}
	t_start = curve->time[i];
	t_end = curve->time[i + 1];
	if (t_end - t_start == 0)
		return (curve->value[i]);
	t = (current_time - t_start) / (t_end - t_start);
	return (curve->value[i] * (1.0 - t) + curve->value[i + 1] * t);
}

static void	ft_get_anim_at_time(t_properties *transformed,
	t_anim_node *current, t_timer timer)
{
	if (current->x)
		transformed->x = ft_get_value_at_time(current->x, timer.weighted_value);
	if (current->y)
		transformed->y = ft_get_value_at_time(current->y, timer.weighted_value);
	if (current->z)
		transformed->z = ft_get_value_at_time(current->z, timer.weighted_value);
}

static void	ft_animate_nodes(t_anim_layer *layer, t_fdf *data)
{
	t_anim_node		*node;
	t_list			*nodes;
	t_model			*model_target;

	nodes = layer->nodes;
	while (nodes)
	{
		node = (t_anim_node *)nodes->content;
		model_target = node->target;
		if (model_target)
		{
			if (node->type == 'S')
				ft_get_anim_at_time(&model_target->scale, node, data->timer);
			else if (node->type == 'R')
				ft_get_anim_at_time(&model_target->rot, node, data->timer);
			else if (node->type == 'T')
				ft_get_anim_at_time(&model_target->pos, node, data->timer);
		}
		nodes = nodes->next;
	}
}

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

/**
 * logique transition entre 2 frames :
 * 1 - trouver/stocker la clé précédente
 * 2 - trouver la clé suivante
 * 3 - calculer le % d'avancement entre les deux
 * 4 - calculer la valeur pondéré <-- (A * (1 - t) + (B * t))
 * t = 0 <-- 100% A + 0% B
 * t = 0.5 <-- 50% A + 50% B
 * t = 0.9 <-- 10% A + 90% B
 */
void	ft_animate(t_fdf *data)
{
	t_anim_layer	*layer;
	t_list			*layers;

	layers = data->fbx->current_anim->layers;
	while (layers)
	{
		layer = (t_anim_layer *)layers->content;
		ft_animate_nodes(layer, data);
		layers = layers->next;
	}
}

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
