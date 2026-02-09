/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/09 04:55:17 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction du thread, je lui passe une structure 't_thread' en argument car
 * elle ne peut prendre qu'un argument void *.
 * J'itere 'start' jusqu'a 'end' pour trouver tous les points 'p' et j
 */
void	*ft_calc_transform_thread(void *arg)
{
	t_thread	*thread;
	t_fdf		*data;
	int			start;

	thread = (t_thread *)arg;
	data = thread->data;
	start = thread->start;
	while (start < thread->end)
	{
		ft_project_point(&data->object->vertices[start], data);
		start++;
	}
	return (NULL);
}

/**
 * Utilise le multi-threading pour faire les calculs de projection en fonction
 * des transformations.
 */
static void	ft_transform_threads(t_fdf *data)
{
	pthread_t	threads[THREADS_NB];
	t_thread	args[THREADS_NB];
	int			i;
	int			chunk;

	chunk = data->object->nb_vertices / THREADS_NB;
	i = -1;
	while (++i < THREADS_NB)
	{
		args[i].data = data;
		args[i].id = i;
		args[i].start = i * chunk;
		args[i].end = (i + 1) * chunk;
		if (i == THREADS_NB - 1)
			args[i].end = data->object->nb_vertices;
		pthread_create(&threads[i], NULL, ft_calc_transform_thread, &args[i]);
	}
	i = -1;
	while (++i < THREADS_NB)
		pthread_join(threads[i], NULL);
}

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
	ft_update_time(&data->timer);
	if (data->fbx)
	{
		if (!data->fbx->current_anim)
			data->fbx->current_anim = data->fbx->anim_stack->content;
		printf("animate %s\n", data->fbx->current_anim->name);
		ft_animate(data);
	}
	ft_bzero(data->img.addr, WIN_WIDTH * WIN_HEIGHT * (data->img.bits_per_pixel / 8));
	ft_transform_threads(data);
	ft_draw_threads(data);
	ft_draw_axes(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.ptr, 0, 0);
	ft_display_fps(data);
	if (data->fbx)
		ft_display_anim_menu(data);
}
