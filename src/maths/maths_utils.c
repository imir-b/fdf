/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:10:53 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:11:46 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

double	ft_to_rad(double degree)
{
	return (degree * M_PI / 180.0);
}
