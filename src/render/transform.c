/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 01:40:06 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 01:56:10 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_zoom(t_point *p, int zoom)
{
	p->x *= zoom;
	p->y *= zoom;
	p->z *= zoom;
}

static void	ft_rotate_x(t_point *p, double angle)
{
	int	y_copy;

	y_copy = p->y;
	p->y = y_copy * cos(angle) + p->z * sin(angle);
	p->z = -y_copy * sin(angle) + p->z * cos(angle);
}

static void	ft_rotate_y(t_point *p, double angle)
{
	int	x_copy;

	x_copy = p->x;
	p->x = x_copy * cos(angle) + p->z * sin(angle);
	p->z = -x_copy * sin(angle) + p->z * cos(angle);
}

static void	ft_iso_project(t_point *p)
{
	int	x_copy;
	int	y_copy;

	x_copy = p->x;
	y_copy = p->y;
	p->x = (x_copy - y_copy) * cos(RADIAN_30);
	p->y = (x_copy + y_copy) * sin(RADIAN_30) - p->z;
}

void	ft_transform(t_point *p, t_fdf *data)
{
	ft_zoom(p, data->camera->zoom);
	ft_rotate_x(p, data->camera->angle_x);
	ft_rotate_y(p, data->camera->angle_y);
	ft_iso_project(p);
	p->x += data->camera->shift_x;
	p->y += data->camera->shift_y;
}
