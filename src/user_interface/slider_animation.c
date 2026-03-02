/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider_animation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:51:49 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:52:05 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_pause(t_fdf *data)
{
	if (!data || !data->fbx)
		return (ERROR);
	data->timer.is_paused = !data->timer.is_paused;
	if (!data->timer.is_paused)
		data->timer.last_frame = ft_get_time_ms();
	return (SUCCESS);
}

/**
 * Affiche les temps de l'animation au format 00:00 / 01:25
 */
void	ft_print_times(t_fdf *data)
{
	data->timer->weighted_value;
	data->timer->duration;
	mlx_string_put(data->mlx_ptr, data->win_ptr, 80, 200, 0xFFFFFF,
			"PAUSED");
}
