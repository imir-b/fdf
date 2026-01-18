/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:07:20 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/18 00:04:55 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_iso_project(double *x, double *y, double *z)
{
	int	x_copy;
	int	y_copy;

	x_copy = *x;
	y_copy = *y;
	*x = (x_copy - y_copy) * cos(RADIAN_30);
	*y = (x_copy + y_copy) * sin(RADIAN_30) - *z;
}

void	ft_parallel_project(t_fdf *data, float angle_x, float angle_y)
{
	data->camera->shift_x = WIN_WIDTH / 2;
	data->camera->shift_y = WIN_HEIGHT / 2;
	data->camera->projection = PARALLEL;
	data->camera->angle_x = angle_x;
	data->camera->angle_y = angle_y;
}
