/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:17:50 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:23:08 by vlad             ###   ########.fr       */
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
void	ft_transform_threads(t_fdf *data)
{
	pthread_t	*threads;
	t_thread	*args;
	int			i;
	int			chunk;

	threads = malloc(sizeof(pthread_t) * data->threads_nb);
	args = malloc(sizeof(t_thread) * data->threads_nb);
	if (!threads || !args)
		return (free(threads), free(args));
	chunk = data->object->nb_vertices / data->threads_nb;
	i = -1;
	while (++i < data->threads_nb)
	{
		args[i].data = data;
		args[i].id = i;
		args[i].start = i * chunk;
		args[i].end = (i + 1) * chunk;
		if (i == data->threads_nb - 1)
			args[i].end = data->object->nb_vertices;
		pthread_create(&threads[i], NULL, ft_calc_transform_thread, &args[i]);
	}
	while (--i >= 0)
		pthread_join(threads[i], NULL);
	free(threads);
	free(args);
}

/**
 * Fonction qui dessine la map avant de l'afficher sur la fenetre.
 * On parcourt toute la map, on trace un trait entre le point et son voisin
 * en appelant la fonction ft_draw_line. Si on est au dernier point de la ligne
 * ou de la colone, on ne dessine pas car plus de voisin, la droite est tracee.
 */
void	*ft_draw_faces_thread(void *arg)
{
	t_thread	*thread;
	t_fdf		*data;
	int			i;

	thread = (t_thread *)arg;
	data = thread->data;
	i = thread->start;
	while (i < thread->end)
	{
		ft_process_face(data, &data->object->faces[i]);
		i++;
	}
	return (NULL);
}

void	ft_draw_threads(t_fdf *data)
{
	pthread_t	*threads;
	t_thread	*args;
	int			i;
	int			chunk;

	threads = malloc(sizeof(pthread_t) * data->threads_nb);
	args = malloc(sizeof(t_thread) * data->threads_nb);
	if (!threads || !args)
		return (free(threads), free(args));
	chunk = data->object->nb_faces / data->threads_nb;
	i = -1;
	while (++i < data->threads_nb)
	{
		args[i].data = data;
		args[i].start = i * chunk;
		args[i].end = (i + 1) * chunk;
		if (i == data->threads_nb - 1)
			args[i].end = data->object->nb_faces;
		pthread_create(&threads[i], NULL, ft_draw_faces_thread, &args[i]);
	}
	i = -1;
	while (++i < data->threads_nb)
		pthread_join(threads[i], NULL);
	free(threads);
	free(args);
}
