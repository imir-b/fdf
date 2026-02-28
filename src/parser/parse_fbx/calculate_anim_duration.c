/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_anim_duration.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:20:02 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:20:06 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** Met à jour le temps maximum d'animation en comparant
** les courbes temporelles x, y et z d'un nœud donné.
*/
static void	ft_update_max_time(t_anim_node *node, double *max_time)
{
	if (node->x && node->x->n_keys > 0)
	{
		if (node->x->time[node->x->n_keys - 1] > *max_time)
			*max_time = node->x->time[node->x->n_keys - 1];
	}
	if (node->y && node->y->n_keys > 0)
	{
		if (node->y->time[node->y->n_keys - 1] > *max_time)
			*max_time = node->y->time[node->y->n_keys - 1];
	}
	if (node->z && node->z->n_keys > 0)
	{
		if (node->z->time[node->z->n_keys - 1] > *max_time)
			*max_time = node->z->time[node->z->n_keys - 1];
	}
}

/*
** Calcule la durée totale d'une animation en trouvant
** la dernière clé temporelle (keyframe) parmi toutes les couches.
*/
void	ft_calculate_anim_duration(t_anim_stack *anim)
{
	t_list			*l_layer;
	t_list			*l_node;
	t_anim_layer	*layer;
	double			max_time;

	max_time = 0;
	l_layer = anim->layers;
	while (l_layer)
	{
		layer = (t_anim_layer *)l_layer->content;
		l_node = layer->nodes;
		while (l_node)
		{
			ft_update_max_time((t_anim_node *)l_node->content, &max_time);
			l_node = l_node->next;
		}
		l_layer = l_layer->next;
	}
	if (max_time > 0)
		anim->duration = max_time;
	else
		anim->duration = 1.0;
}
