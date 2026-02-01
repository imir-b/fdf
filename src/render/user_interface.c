/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:27:27 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 23:25:01 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_display_fps(t_fdf *data)
{
	static long		last_check = 0;
	static int		frames = 0;
	static char		*final_str = NULL;
	char			*fps_str;
	long			current_time;

	current_time = ft_get_time_ms();
	frames++;
	if (current_time - last_check >= 1000)
	{
		if (final_str)
			free(final_str);
		fps_str = ft_itoa(frames);
		if (!fps_str)
			return (ERROR);
		final_str = ft_strjoin("FPS : ", fps_str);
		free(fps_str);
		if (!final_str)
			return (ERROR);
		frames = 0;
		last_check = current_time;
	}
	if (final_str)
		mlx_string_put(data->mlx_ptr, data->win_ptr, 50, 20, 0xFFFFFF, final_str);
	return (SUCCESS);
}

int ft_display_anim_menu(t_fdf *data)
{
    (void)data;
    return (SUCCESS);
}
