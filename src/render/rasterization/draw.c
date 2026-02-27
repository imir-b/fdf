/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 04:36:22 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:26:36 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	fast_pixel_put(t_fdf *data, int x, int y, int color)
{
	char	*dst;

	dst = data->img.addr + (y * data->img.line_length + x
			* (data->img.bits_per_pixel >> 3));
	*(unsigned int *)dst = color;
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
	ft_draw_line(data, (t_point){o.sx, o.sy, 0},
		(t_point){x.sx, x.sy, 0}, 0xFF0000);
	ft_draw_line(data, (t_point){o.sx, o.sy, 0},
		(t_point){y.sx, y.sy, 0}, 0x00FF00);
	ft_draw_line(data, (t_point){o.sx, o.sy, 0},
		(t_point){z.sx, z.sy, 0}, 0x0000FF);
}

void	ft_process_face(t_fdf *data, t_face *face)
{
	int		j;
	t_vec3	v1;
	t_vec3	v2;

	j = 0;
	while (j < face->count)
	{
		v1 = data->object->vertices[face->indices[j]];
		v2 = data->object->vertices[face->indices[(j + 1) % face->count]];
		ft_draw_line(data, (t_point){v1.sx, v1.sy, 0},
			(t_point){v2.sx, v2.sy, 0}, v1.color);
		j++;
	}
}

/**
 * Fonction qui utilise l'algorithme de Bresenham pour dessiner un segment de
 * droite entre 2 points 'p1' et 'p2'. On stock les donnes necessaires a
 * l'algo dans la structure t_bresenham. On dessine le pixel au coordonnes x y
 * de p1 et on avance de step_x et/ou step_y pour dessiner le prochain.
 * Quand p1 est egal a p2 on a dessine toute la ligne, on sort de la boucle.
 */
void	ft_draw_line(t_fdf *data, t_point p1, t_point p2, int color)
{
	t_bresenham	bressenham;
	int			err2;

	bressenham = ft_init_graphics(p1, p2);
	while (TRUE)
	{
		if (p1.x >= 0 && p1.x < WIN_WIDTH && p1.y >= 0 && p1.y < WIN_HEIGHT)
			fast_pixel_put(data, p1.x, p1.y, color);
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
