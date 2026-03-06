/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:10:53 by vbleskin          #+#    #+#             */
/*   Updated: 2026/03/06 18:39:44 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

double	ft_abs_double(double n)
{
	if (n < 0)
		return (-n);
	return (n);
}

double	ft_to_rad(double degree)
{
	return (degree * M_PI / 180.0);
}
