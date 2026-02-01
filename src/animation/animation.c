/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:49 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 19:40:45 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

long ft_get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_init_timer(t_timer *t, double duration_sec)
{
	t->start_time = ft_get_time_ms();
	t->last_frame = t->start_time;
	t->delta_time = 0;
	t->weighted_value = 0;
	t->duration = duration_sec;
}

void	ft_update_time(t_timer *t)
{
	long	current;
	
	current = ft_get_time_ms();
	t->delta_time = (double)(current - t->last_frame) / 1000.0;
	t->last_frame = current;
	t->weighted_value += t->delta_time;
	if (t->weighted_value >= t->duration)
		t->weighted_value = fmod(t->weighted_value, t->duration);
}

// User Interface :
// - FPS
// - menu de selection d'animations / poses (= parser et stocker les noms)

// Appliquer dans cet ordre : 
// 1 - scale
// 2 - rotate
// 3 - translate

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