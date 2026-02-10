/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:30:16 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 22:30:44 by vlad             ###   ########.fr       */
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
