/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:53:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction qui rend l'image en 6 etapes :
 * - 1 Pre-calcul des maths pour eviter de repeter les calculs lourds,
 * - 2 Projection : J'appelle ft_transform_threads pour faire les calculs des
 * transformations,
 * - 3 Nettoyage : J'appelle bzero sur 'addr' pour effacer l'ancienne image,
 * - 4 Rasterization : j'appelle ft_draw_threads pour dessiner la nouvelle image,
 * - 5 Affichage : j'appelle mlx_put_image_to_window pour affichier l'image sur
 * l'ecran.
 * - 6 User interface : j'appelle les fonctions qui affichent les éléments de
 * l'interface.
 */
void	ft_render_image(t_fdf *data)
{
	data->trigo.sin_alpha = sin(data->camera->angle_x);
	data->trigo.cos_alpha = cos(data->camera->angle_x);
	data->trigo.sin_beta = sin(data->camera->angle_y);
	data->trigo.cos_beta = cos(data->camera->angle_y);
	if (data->fbx && data->fbx->current_anim)
	{
		ft_update_time(&data->timer);
		if (isnan(data->timer.weighted_value)
			|| isinf(data->timer.weighted_value))
			data->timer.weighted_value = 0.0;
		if (data->fbx->current_anim->duration > 0.001)
			data->timer.weighted_value = fmod(data->timer.weighted_value,
					data->fbx->current_anim->duration);
		ft_animate(data);
		ft_update_mesh_from_animation(data);
	}
	ft_bzero(data->img.addr, WIN_WIDTH * WIN_HEIGHT
		* (data->img.bits_per_pixel / 8));
	ft_transform_threads(data);
	ft_draw_threads(data);
	ft_draw_axes(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.ptr, 0, 0);
	ft_display_fps(data);
	if (data->fbx && data->fbx->anim_stack)
		ft_display_anim_menu(data);
}
