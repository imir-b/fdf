/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/02/28 22:35:24 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_get_value_at_time(t_anim_curve *curve, double current_time)
{
	int			i;
	double		ratio;
	double		t_start;
	double		t_end;

	if (!curve || curve->n_keys == 0 || !curve->time || !curve->value)
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
	ratio = (current_time - t_start) / (t_end - t_start);
	return (curve->value[i] * (1.0 - ratio) + curve->value[i + 1] * ratio);
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

static void	ft_get_rot_at_time(t_properties *rot, t_anim_node *node, double time)
{
	t_anim_curve	*cx;
	t_anim_curve	*cy;
	t_anim_curve	*cz;
	int				i;
	double			ratio;
	t_quat			q1;
	t_quat			q2;

	cx = node->x;
	cy = node->y;
	cz = node->z;
	if (!cx || !cy || !cz || cx->n_keys < 2 || cy->n_keys < 2 || cz->n_keys < 2)
	{
		if (cx) rot->x = ft_get_value_at_time(cx, time);
		if (cy) rot->y = ft_get_value_at_time(cy, time);
		if (cz) rot->z = ft_get_value_at_time(cz, time);
		return ;
	}
	if (time >= cx->time[cx->n_keys - 1])
	{
		rot->x = cx->value[cx->n_keys - 1];
		rot->y = cy->value[cy->n_keys - 1];
		rot->z = cz->value[cz->n_keys - 1];
		return ;
	}
	i = 0;
	while (i < cx->n_keys - 1 && time >= cx->time[i + 1])
		i++;
	if (cx->time[i + 1] - cx->time[i] == 0)
		ratio = 0;
	else
		ratio = (time - cx->time[i]) / (cx->time[i + 1] - cx->time[i]);
	q1 = ft_euler_to_quat(cx->value[i], cy->value[i], cz->value[i]);
	q2 = ft_euler_to_quat(cx->value[i + 1], cy->value[i + 1], cz->value[i + 1]);
	*rot = ft_quat_to_euler(ft_slerp(q1, q2, ratio));
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
				ft_get_rot_at_time(&model_target->rot, node, data->timer.weighted_value);
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
