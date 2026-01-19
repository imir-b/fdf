/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/19 06:37:37 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction qui utilise l'algorithme de Bresenham pour dessiner un segment de
 * droite entre 2 points 'p1' et 'p2'. On stock les donnes necessaires a
 * l'algo dans la structure t_bresenham. On dessine le pixel au coordonnes x y
 * de p1 et on avance de step_x et/ou step_y pour dessiner le prochain.
 * Quand p1 est egal a p2 on a dessine toute la ligne, on sort de la boucle.
 */
static void	ft_draw_line(t_fdf *data, t_point p1, t_point p2, int color)
{
	t_bresenham	bressenham;
	int			err2;

	if ((p1.x < 0 && p2.x < 0) || (p1.x >= WIN_WIDTH && p2.x >= WIN_WIDTH) || \
		(p1.y < 0 && p2.y < 0) || (p1.y >= WIN_HEIGHT && p2.y >= WIN_HEIGHT))
		return ;
	bressenham = ft_init_graphics(p1, p2);
	while (TRUE)
	{
		if (p1.x >= 0 && p1.x < WIN_WIDTH && p1.y >= 0 && p1.y < WIN_HEIGHT)
			my_mlx_pixel_put(data, p1.x, p1.y, color);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		err2 = bressenham.err * 2;
		if (err2 > -bressenham.diff_y)
		{
			bressenham.err -= bressenham.diff_y;
			p1.x += bressenham.step_x;
		}
		if (err2 < bressenham.diff_x)
		{
			bressenham.err += bressenham.diff_x;
			p1.y += bressenham.step_y;
		}
	}
}

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
 * Fonction pour afficher les axes de rotation en bas a droite de l'ecran.
 * (Utile pour se reperer dans l'espace)
 */
void	ft_draw_axes(t_fdf *data)
{
	t_vec3	o;
	t_vec3	x;
	t_vec3	y;
	t_vec3	z;
	int		len;

	len = 100;
	o = (t_vec3){0, 0, 0, 0, 0, 0};
	x = (t_vec3){len, 0, 0, 0, 0, 0};
	y = (t_vec3){0, len, 0, 0, 0, 0};
	z = (t_vec3){0, 0, len, 0, 0, 0};
	ft_project_point(&o, data);
	ft_project_point(&x, data);
	ft_project_point(&y, data);
	ft_project_point(&z, data);
	ft_draw_line(data, \
		(t_point){o.sx, o.sy, 0}, \
		(t_point){x.sx, x.sy, 0}, 0xFF0000);
	ft_draw_line(data, \
		(t_point){o.sx, o.sy, 0}, \
		(t_point){y.sx, y.sy, 0}, 0x00FF00);
	ft_draw_line(data, \
		(t_point){o.sx, o.sy, 0}, \
		(t_point){z.sx, z.sy, 0}, 0x0000FF);
}

static void	ft_process_face(t_fdf *data, t_face *face)
{
	int		j;
	t_vec3	v1;
	t_vec3	v2;

	j = 0;
	while (j < face->count)
	{
		v1 = data->object->vertices[face->indices[j]];
		v2 = data->object->vertices[face->indices[(j + 1) % face->count]];
		ft_draw_line(data, \
			(t_point){v1.sx, v1.sy, 0}, \
			(t_point){v2.sx, v2.sy, 0}, v1.color);
		j++;
	}
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

static void	ft_draw_threads(t_fdf *data)
{
	pthread_t	threads[THREADS_NB];
	t_thread	args[THREADS_NB];
	int			i;
	int			chunk;

	chunk = data->object->nb_faces / THREADS_NB;
	i = -1;
	while (++i < THREADS_NB)
	{
		args[i].data = data;
		args[i].start = i * chunk;
		args[i].end = (i + 1) * chunk;
		if (i == THREADS_NB - 1)
			args[i].end = data->object->nb_faces;
		pthread_create(&threads[i], NULL, ft_draw_faces_thread, &args[i]);
	}
	i = -1;
	while (++i < THREADS_NB)
		pthread_join(threads[i], NULL);
}

/**
 * Fonction qui rend l'image en 5 etapes :
 * - 1 Pre-calcul des maths pour eviter de repeter les calculs lourds,
 * - 2 Projection : J'appelle ft_transform_threads pour faire les calculs des 
 * transformations,
 * - 3 Nettoyage : J'appelle bzero sur 'addr' pour effacer l'ancienne image,
 * - 4 Rasterization : j'appelle ft_draw_threads pour dessiner la nouvelle image,
 * - 5 Affichage : j'appelle mlx_put_image_to_window pour affichier l'image sur 
 * l'ecran.
 */
void	ft_render_image(t_fdf *data)
{
	data->trigo.sin_alpha = sin(data->camera->angle_x);
	data->trigo.cos_alpha = cos(data->camera->angle_x);
	data->trigo.sin_beta = sin(data->camera->angle_y);
	data->trigo.cos_beta = cos(data->camera->angle_y);
	ft_bzero(data->addr, WIN_WIDTH * WIN_HEIGHT * (data->bits_per_pixel / 8));
	ft_transform_threads(data);
	ft_draw_threads(data);
	ft_draw_axes(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
}

t_camera	*ft_init_camera(t_object *obj)
{
	t_camera	*cam;

	cam = malloc(sizeof(t_camera));
	if (!cam)
		return (NULL);
	if (obj->width > 0)
		cam->zoom = WIN_WIDTH / obj->width / 2;
	else
		cam->zoom = 20;
	if (cam->zoom < 1)
		cam->zoom = 1;
	cam->angle_x = RADIAN_30;
	cam->angle_y = RADIAN_30;
	cam->z_scale = 1.0;
	cam->shift_x = WIN_WIDTH / 2;
	cam->shift_y = WIN_HEIGHT / 2;
	cam->projection = ISOMETRIC;
	return (cam);
}

int	ft_process_fdf(t_object *obj)
{
	t_fdf		*data;
	t_camera	*cam;

	cam = ft_init_camera(obj);
	if (!cam)
		return (ERROR);
	data = ft_init_data(obj, cam);
	if (!data)
		return (ERROR);
	ft_render_image(data);
	ft_events(data);
	mlx_loop(data->mlx_ptr);
	ft_free_data(data);
	return (SUCCESS);
}
