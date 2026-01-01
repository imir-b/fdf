/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:07:20 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 19:07:26 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_iso_project(t_point *p)
{
	int	x_copy;
	int	y_copy;

	x_copy = p->x;
	y_copy = p->y;
	p->x = (x_copy - y_copy) * cos(RADIAN_30);
	p->y = (x_copy + y_copy) * sin(RADIAN_30) - p->z;
}

void	ft_parallel_project(t_fdf *data, float angle_x, float angle_y)
{
	data->camera->projection = PARALLEL;
	data->camera->angle_x = angle_x;
	data->camera->angle_y = angle_y;
}
