/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/02 15:08:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction qui utilise l'algorithme de Bresenham pour dessiner un segment de
 * droite entre 2 points 'p1' et 'p2'. On appelle d'abord la fonction 
 * ft_transform pour appliquer les transformations (zoom, rotation, projection)
 * en fonction de la position de la camera. On stock les donnes necessaires a
 * l'algo dans la structure t_bresenham. On dessine le pixel au coordonnes x y
 * de p1 et on avance de step_x et/ou step_y pour dessiner le prochain.
 * Quand p1 est egal a p2 on a dessine toute la ligne, on sort de la boucle.
 */
static void	ft_draw_line(t_fdf *data, t_point p1, t_point p2, int color)
{
	t_pixel		pixel;
	t_bresenham	graphics;
	int			err2;

	ft_transform(&p1, data);
	ft_transform(&p2, data);
	if ((p1.x < 0 && p2.x < 0) || (p1.x >= WIN_WIDTH && p2.x >= WIN_WIDTH) || \
		(p1.y < 0 && p2.y < 0) || (p1.y >= WIN_HEIGHT && p2.y >= WIN_HEIGHT))
		return ;
	graphics = ft_init_graphics(p1, p2);
	while (TRUE)
	{
		if (p1.x >= 0 && p1.x < WIN_WIDTH && p1.y >= 0 && p1.y < WIN_HEIGHT)
		{
			pixel = (t_pixel){p1.x, p1.y};
			my_mlx_pixel_put(data, pixel, color);
		}
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		err2 = graphics.err * 2;
		if (err2 > -graphics.diff_y)
		{
			graphics.err -= graphics.diff_y;
			p1.x += graphics.step_x;
		}
		if (err2 < graphics.diff_x)
		{
			graphics.err += graphics.diff_x;
			p1.y += graphics.step_y;
		}
	}
}

/**
 * Fonction qui dessine la map avant de l'afficher sur la fenetre.
 * On parcourt toute la map, on trace un trait entre le point et son voisin
 * en appelant la fonction ft_draw_line. Si on est au dernier point de la ligne
 * ou de la colone, on ne dessine pas car plus de voisin, la droite est tracee.
 */
void	*ft_draw_thread(void *arg)
{
	t_thread	*thread;
	t_fdf		*data;
	t_point		p1;
	t_point		p2;

	thread = (t_thread *)(arg);
	data = thread->data;
	p1.y = thread->start;
	while (p1.y < thread->end)
	{
		p1.x = 0;
		while (p1.x < data->map->width)
		{
			p1.z = data->map->grid[p1.y][p1.x];
			if (p1.x < data->map->width - 1)
			{
				p2 = (t_point){p1.x + 1, p1.y, data->map->grid[p1.y][p1.x + 1]};
				ft_draw_line(data, p1, p2, data->map->colors[p1.y][p1.x]);
			}
			if (p1.y < data->map->height - 1)
			{
				p2 = (t_point){p1.x, p1.y + 1, data->map->grid[p1.y + 1][p1.x]};
				ft_draw_line(data, p1, p2, data->map->colors[p1.y][p1.x]);
			}
			p1.x++;
		}
		p1.y++;
	}
	return (free(thread), NULL);
}

int	ft_render(t_fdf *data)
{
	pthread_t	threads[THREADS_NB];
	t_thread	*args;
	int			index;

	bzero(data->addr, WIN_WIDTH * WIN_HEIGHT * (data->bits_per_pixel / 8));
	index = -1;
	while (++index < THREADS_NB)
	{
		args = malloc(sizeof(t_thread));
		if (!args)
			return (ERROR);
		args->data = data;
		args->id = index;
		args->start = index * (data->map->height / THREADS_NB);
		args->end = (index + 1) * (data->map->height / THREADS_NB);
		if (index == THREADS_NB - 1)
			args->end = data->map->height;
		if (pthread_create(&threads[index], NULL, ft_draw_thread, args))
			return (free(args), ERROR);
	}
	index = 0;
	while (index < THREADS_NB)
		pthread_join(threads[index++], NULL);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
	return (SUCCESS);
}

int	ft_process_fdf(t_map *map)
{
	t_fdf		*data;
	t_camera	camera;
	t_maths		maths;

	camera = (t_camera){WIN_WIDTH / 2, WIN_HEIGHT / 2, 10, 1.0, \
		ISOMETRIC, RADIAN_30, RADIAN_30};
	maths = (t_maths){0.0, 0.0, 0.0, 0.0, cos(RADIAN_30), sin(RADIAN_30), \
		map->width / THREADS_NB, map->height / THREADS_NB};
	data = ft_init_data(map, &camera, &maths);
	if (!data)
		return (ERROR);
	if (ft_render(data))
		return (ERROR);
	ft_events(data);
	mlx_loop(data->mlx_ptr);
	ft_free_data(data);
	return (SUCCESS);
}
