/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 10:48:29 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:30:13 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_mat4_identity(t_mat4 *out_matrix)
{
	int	row;
	int	col;

	row = -1;
	while (++row < 4)
	{
		col = -1;
		while (++col < 4)
		{
			if (row == col)
				out_matrix->m[row][col] = 1.0;
			else
				out_matrix->m[row][col] = 0.0;
		}
	}
}

/**
 * Multiplie deux matrices 4x4 (row-major): result = A × B
 */
void	ft_mat4_multiply(t_mat4 *mat_a, t_mat4 *mat_b, t_mat4 *out_matrix)
{
	int	row;
	int	col;
	int	idx;

	row = -1;
	while (++row < 4)
	{
		col = -1;
		while (++col < 4)
		{
			out_matrix->m[row][col] = 0;
			idx = -1;
			while (++idx < 4)
			{
				out_matrix->m[row][col] += mat_a->m[row][idx]
					* mat_b->m[idx][col];
			}
		}
	}
}

/**
 * Multiplie une matrice 4x4 (row-major FBX) par un vec3 (w=1).
 */
t_vec3	ft_apply_mat4(t_mat4 *matrix, t_vec3 in_vec)
{
	t_vec3	out_vec;

	if (!matrix)
		return (in_vec);
	out_vec.x = in_vec.x * matrix->m[0][0] + in_vec.y * matrix->m[1][0]
		+ in_vec.z * matrix->m[2][0] + matrix->m[3][0];
	out_vec.y = in_vec.x * matrix->m[0][1] + in_vec.y * matrix->m[1][1]
		+ in_vec.z * matrix->m[2][1] + matrix->m[3][1];
	out_vec.z = in_vec.x * matrix->m[0][2] + in_vec.y * matrix->m[1][2]
		+ in_vec.z * matrix->m[2][2] + matrix->m[3][2];
	out_vec.color = in_vec.color;
	return (out_vec);
}
