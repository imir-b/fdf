/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:49:52 by vbleskin          #+#    #+#             */
/*   Updated: 2026/03/07 10:50:59 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

double	ft_get_lerp_ratio(t_anim_curve *c, double time, int *idx)
{
	int		i;
	double	t_start;
	double	t_end;

	i = 0;
	while (i < c->n_keys - 1 && time >= c->time[i + 1])
		i++;
	*idx = i;
	t_start = c->time[i];
	t_end = c->time[i + 1];
	if (t_end - t_start == 0)
		return (0.0);
	return ((time - t_start) / (t_end - t_start));
}

double	ft_get_value_at_time(t_anim_curve *curve, double current_time)
{
	int		i;
	double	ratio;

	if (!curve || curve->n_keys == 0 || !curve->time || !curve->value)
		return (0.0);
	if (curve->n_keys == 1)
		return (curve->value[0]);
	if (current_time >= curve->time[curve->n_keys - 1])
		return (curve->value[curve->n_keys - 1]);
	ratio = ft_get_lerp_ratio(curve, current_time, &i);
	return (curve->value[i] * (1.0 - ratio) + curve->value[i + 1] * ratio);
}

void	ft_apply_slerp(t_properties *rot, t_anim_node *node, double time)
{
	int		i;
	double	ratio;
	t_quat	q1;
	t_quat	q2;

	ratio = ft_get_lerp_ratio(node->x, time, &i);
	q1 = ft_euler_to_quat(node->x->value[i], node->y->value[i],
			node->z->value[i]);
	q2 = ft_euler_to_quat(node->x->value[i + 1], node->y->value[i + 1],
			node->z->value[i + 1]);
	*rot = ft_quat_to_euler(ft_slerp(q1, q2, ratio));
}
