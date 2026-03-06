/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider_animation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:51:49 by vbleskin          #+#    #+#             */
/*   Updated: 2026/03/03 14:11:42 by vbleskin         ###   ########.fr       */
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

char	*ft_get_timecode(char *timecode, double time)
{
	int	minutes;
	int	seconds;

	if (time < 0)
		time = 0;
	minutes = (int)time / 60;
	seconds = (int)time % 60;
	timecode[0] = (minutes / 10) + '0';
	timecode[1] = (minutes % 10) + '0';
	timecode[2] = ':';
	timecode[3] = (seconds / 10) + '0';
	timecode[4] = (seconds % 10) + '0';
	timecode[5] = '\0';
	return (timecode);
}

void	ft_draw_slider(t_fdf *data, int y, int x_start, int x_end)
{
	float	progress;
	int		x;

	if (data->timer.duration <= 0.001)
		progress = 0.0;
	else
		progress = data->timer.weighted_value / data->timer.duration;
	x = x_start + (int)(progress * (x_end - x_start));
	ft_draw_line(data, (t_point){x_start, y, 0},
		(t_point){x_end, y, 0}, 0x555555);
	ft_draw_line(data, (t_point){x_start, y, 0},
		(t_point){x, y, 0}, 0xFFFFFF);
	ft_draw_line(data, (t_point){x, y - 5, 0},
		(t_point){x, y + 5, 0}, 0xFF0000);
}

/**
 * Affiche les temps de l'animation au format 00:00 / 01:25
 * 
 * @param data Structure principale contenant les données temporelles.
 */
void	ft_display_slider(t_fdf *data)
{
	const int	y = 80;
	const int	x_start = 100;
	const int	x_end = 300;
	char		timecode_start[6];
	char		timecode_end[6];

	ft_get_timecode(timecode_start, data->timer.weighted_value);
	ft_get_timecode(timecode_end, data->timer.duration);
	ft_draw_slider(data, y, x_start, x_end);
	mlx_string_put(data->mlx_ptr, data->win_ptr, x_start - 45, y + 5, 0xFFFFFF,
		timecode_start);
	mlx_string_put(data->mlx_ptr, data->win_ptr, x_end + 15, y + 5, 0xFFFFFF,
		timecode_end);
}
