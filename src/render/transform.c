/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 01:40:06 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/02 17:13:37 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_zoom(t_point *p, int zoom)
{
	p->x *= zoom;
	p->y *= zoom;
	p->z *= zoom;
}

static void	ft_rotate_x(t_point *p, t_maths *maths)
{
	int	y_copy;

	y_copy = p->y;
	p->y = y_copy * maths->cos_y + p->z * maths->sin_y;
	p->z = -y_copy * maths->sin_y + p->z * maths->cos_y;
}

static void	ft_rotate_y(t_point *p, t_maths *maths)
{
	int	x_copy;

	x_copy = p->x;
	p->x = x_copy * maths->cos_x + p->z * maths->sin_x;
	p->z = -x_copy * maths->sin_x + p->z * maths->cos_x;
}

void	ft_transform_axis_point(t_point *p, t_fdf *data)
{
	ft_rotate_x(p, data->maths);
	ft_rotate_y(p, data->maths);
	if (data->camera->projection == ISOMETRIC)
		ft_iso_project(p, data->maths);
	p->x += WIN_WIDTH - 100;
	p->y += WIN_HEIGHT - 100;
}

/**
 * Fonction qui applique les transformations zoom, rotate_x, rotate_y, la
 * position sur la fenetre et la projection isometrique sur un point 'p'.
 */
void	ft_transform(t_point *p, t_fdf *data)
{
	ft_zoom(p, data->camera->zoom);
	ft_rotate_x(p, data->maths);
	ft_rotate_y(p, data->maths);
	if (data->camera->projection == ISOMETRIC)
		ft_iso_project(p, data->maths);
	p->x += data->camera->shift_x;
	p->y += data->camera->shift_y;
}
