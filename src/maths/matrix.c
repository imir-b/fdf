/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 10:48:29 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 12:17:08 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_mat4_identity(t_mat4 *out)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			out->m[i][j] = (i == j) ? 1.0 : 0.0;
	}
}

/**
 * Multiplie deux matrices 4x4 (row-major): result = A × B
 */
static void	ft_mat4_multiply(t_mat4 *a, t_mat4 *b, t_mat4 *out)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			out->m[i][j] = 0;
			k = -1;
			while (++k < 4)
				out->m[i][j] += a->m[i][k] * b->m[k][j];
		}
	}
}

/**
 * Multiplie une matrice 4x4 (row-major FBX) par un vec3 (w=1).
 */
t_vec3	ft_apply_mat4(t_mat4 *mat, t_vec3 v)
{
	t_vec3	r;

	if (!mat)
		return (v);
	r.x = v.x * mat->m[0][0] + v.y * mat->m[1][0] + v.z
		* mat->m[2][0] + mat->m[3][0];
	r.y = v.x * mat->m[0][1] + v.y * mat->m[1][1] + v.z
		* mat->m[2][1] + mat->m[3][1];
	r.z = v.x * mat->m[0][2] + v.y * mat->m[1][2] + v.z
		* mat->m[2][2] + mat->m[3][2];
	r.color = v.color;
	return (r);
}
