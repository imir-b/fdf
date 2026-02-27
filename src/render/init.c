/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 04:36:29 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:53:57 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_loop_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	ft_render_image(data);
	return (SUCCESS);
}

int	ft_process_fdf(t_object *obj, t_fbx *fbx)
{
	t_fdf		*data;
	t_camera	*cam;

	cam = ft_init_camera(obj, fbx);
	if (!cam)
		return (ERROR);
	data = ft_init_data(obj, cam, fbx);
	if (!data)
		return (ERROR);
	ft_render_image(data);
	ft_events(data);
	mlx_loop_hook(data->mlx_ptr, (int (*)())(void *)ft_loop_hook, data);
	mlx_loop(data->mlx_ptr);
	ft_free_data(data);
	return (SUCCESS);
}
