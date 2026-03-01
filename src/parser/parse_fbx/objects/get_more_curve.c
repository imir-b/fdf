/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_curve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:26:29 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:26:39 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_distribute_group(t_anim_curve *c, int start, int i)
{
	int		size;
	double	next;
	double	step;
	int		j;

	size = i - start + 1;
	if (size <= 1)
		return ;
	if (i + 1 < c->n_keys)
		next = c->time[i + 1];
	else
		next = c->time[start] + 1.0;
	step = (next - c->time[start]) / (size + 1);
	j = -1;
	while (++j < size)
		c->time[start + j] = c->time[start] + j * step;
}

/**
 * Redistribue les clés temporelles (keyframes) pour lisser l'animation.
 * Les exports "baked" partagent souvent le même "timestamp" pour de multiples
 * frames (ex: 60 frames listées à 0 seconde). Cette fonction repère ces groupes
 * et les espace de manière égale jusqu'à la seconde suivante pour interpoler
 * chaque frame fluidement sans accoups.
 */
void	ft_redistribute_keytimes(t_anim_curve *curve)
{
	int	i;
	int	start;

	if (!curve || curve->n_keys <= 1 || !curve->time)
		return ;
	i = 0;
	while (i < curve->n_keys)
	{
		start = i;
		while (i < curve->n_keys - 1
			&& curve->time[i + 1] == curve->time[start])
			i++;
		ft_distribute_group(curve, start, i);
		i++;
	}
}
