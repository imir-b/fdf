/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/03/07 10:53:09 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

/**
 * Assemble les Quaternions A et B à partir des frames connues et 
 * demande à ft_slerp un résultat ponctuel et interpolé, supprimant le 
 * risque de vrilles au franchissement de 180°.
 **/
static void	ft_get_rot_at_time(t_properties *rot, t_anim_node *node,
				double time)
{
	if (!node->x || !node->y || !node->z || node->x->n_keys < 2
		|| node->y->n_keys < 2 || node->z->n_keys < 2)
	{
		rot->x = ft_get_value_at_time(node->x, time);
		rot->y = ft_get_value_at_time(node->y, time);
		rot->z = ft_get_value_at_time(node->z, time);
		return ;
	}
	if (time >= node->x->time[node->x->n_keys - 1])
	{
		rot->x = node->x->value[node->x->n_keys - 1];
		rot->y = node->y->value[node->y->n_keys - 1];
		rot->z = node->z->value[node->z->n_keys - 1];
		return ;
	}
	ft_apply_slerp(rot, node, time);
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
				ft_get_rot_at_time(&model_target->rot, node,
					data->timer.weighted_value);
			else if (node->type == 'T')
				ft_get_anim_at_time(&model_target->pos, node, data->timer);
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
 * 
 * @param data Structure principale du programme.
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
