/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 01:40:06 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:51:44 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Applique la rotation autour de l'axe X.
 */
static void	ft_rotate_x(double *y, double *z, t_maths trigo)
{
	int	y_copy;

	y_copy = *y;
	*y = y_copy * trigo.cos_alpha - *z * trigo.sin_alpha;
	*z = y_copy * trigo.sin_alpha + *z * trigo.cos_alpha;
}

/**
 * Applique la rotation autour de l'axe Y.
 */
static void	ft_rotate_y(double *x, double *z, t_maths trigo)
{
	int	x_copy;

	x_copy = *x;
	*x = x_copy * trigo.cos_beta + *z * trigo.sin_beta;
	*z = -x_copy * trigo.sin_beta + *z * trigo.cos_beta;
}

/**
 * 
 */
void	ft_transform_axis_point(double *x, double *y, double *z, t_fdf *data)
{
	ft_rotate_y(x, z, data->trigo);
	ft_rotate_x(y, z, data->trigo);
	if (data->camera->projection == ISOMETRIC)
		ft_iso_project(x, y, z);
	*x += WIN_WIDTH - 100;
	*y += WIN_HEIGHT - 100;
}

/**
 * Fonction qui applique les transformations zoom, rotate_x, rotate_y, la
 * position sur la fenetre et la projection isometrique sur un point 'p'.
 */
void	ft_project_point(t_vec3 *v, t_fdf *data)
{
	double	x;
	double	y;
	double	z;

	x = v->x;
	y = v->y;
	z = v->z;
	v->sy = y;
	if (data->object->width > 0)
	{
		x -= data->object->width / 2;
		y -= data->object->height / 2;
	}
	x *= data->camera->zoom;
	y *= data->camera->zoom;
	z *= data->camera->zoom * data->camera->z_scale;
	ft_rotate_y(&x, &z, data->trigo);
	ft_rotate_x(&y, &z, data->trigo);
	x += data->camera->shift_x;
	y += data->camera->shift_y;
	v->sx = x;
	v->sy = y;
}
