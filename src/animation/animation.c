/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/02/03 02:52:47 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_get_value_at_time(t_anim_curve *curve, double current_time_sec)
{
	int         i;
    double      t;
	double		t_start;
	double		t_end;
    
	if (!curve || curve->n_keys == 0 || !curve->time || !curve->value)
		return (0.0);
	if (curve->n_keys == 1)
		return (curve->value[0]);
	if (current_time_sec >= curve->time[curve->n_keys - 1])
		return (curve->value[curve->n_keys - 1]);
	i = 0;
	while (i < curve->n_keys - 1)
	{
		if (current_time_sec < curve->time[i + 1])
			break ;
		i++;
	}
	t_start = curve->time[i];
	t_end = curve->time[i + 1];
	if (t_end - t_start == 0) 
		return (curve->value[i]);
	t = (current_time_sec - t_start) / (t_end - t_start);
	return (curve->value[i] * (1.0 - t) + curve->value[i + 1] * t);
}

/**
 * Renvoie l'état d'une propriete pendant l'animation 'anim' au moment du timer
 */
static t_properties	ft_transform_frame(t_anim_node *anim, t_fdf *data)
{
	t_properties	prop = (t_properties){0};

	if (anim->x)
		prop.x = ft_get_value_at_time(anim->x, data->timer.weighted_value);
	if (anim->y)
		prop.y = ft_get_value_at_time(anim->y, data->timer.weighted_value);
	if (anim->z)
		prop.z = ft_get_value_at_time(anim->z, data->timer.weighted_value);
	return (prop);
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
		model_target = ft_get_by_id(data->fbx->model, node->id);
		if (model_target)
		{
			if (node->type == 'S')
				model_target->scale = ft_transform_frame(node, data);
			else if (node->type == 'R')
				model_target->rot = ft_transform_frame(node, data);
			else if (node->type == 'T')
				model_target->pos = ft_transform_frame(node, data);
		}
		nodes = nodes->next;
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
	t_anim_stack	*anim;
	t_anim_layer	*layer;
	t_list			*layers;
	
	if (!data->fbx || !data->fbx->current_anim)
		return ;
	anim = data->fbx->current_anim;
	layers = anim->layers;
	while (layers)
	{
		layer = (t_anim_layer *)layers->content;
			ft_animate_nodes(layer, data);
		layers = layers->next;
	}
}