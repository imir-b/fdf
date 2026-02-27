/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_fps.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 03:40:06 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 03:42:31 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_display_fps(t_fdf *data)
{
	static long		last_check = 0;
	static int		frames = 0;
	static char		*str = NULL;
	char			*fps_str;
	long			current_time;

	current_time = ft_get_time_ms();
	frames++;
	if (current_time - last_check >= 1000)
	{
		if (str)
			free(str);
		fps_str = ft_itoa(frames);
		if (!fps_str)
			return (ERROR);
		str = ft_strjoin("FPS : ", fps_str);
		free(fps_str);
		if (!str)
			return (ERROR);
		frames = 0;
		last_check = current_time;
	}
	if (str)
		mlx_string_put(data->mlx_ptr, data->win_ptr, 50, 20, 0xFFFFFF, str);
	return (SUCCESS);
}
