/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:07:20 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 22:42:02 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_iso_project(t_point *p, t_maths *maths)
{
	int	x_copy;
	int	y_copy;

	x_copy = p->x;
	y_copy = p->y;
	p->x = (x_copy - y_copy) * maths->cos_30;
	p->y = (x_copy + y_copy) * maths->sin_30 - p->z;
}

void	ft_parallel_project(t_fdf *data, float angle_x, float angle_y)
{
	data->camera->projection = PARALLEL;
	data->camera->angle_x = angle_x;
	data->maths->cos_x = cos(angle_x);
	data->maths->sin_x = sin(angle_x);
	data->camera->angle_y = angle_y;
	data->maths->cos_y = cos(angle_y);
	data->maths->sin_y = sin(angle_y);
}
