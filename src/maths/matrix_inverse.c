/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 10:54:18 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:27:42 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_mat3_det(double m[3][3])
{
	return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
		+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

static double	ft_mat4_cofactor(t_mat4 *m, int r, int c)
{
	double	sub[3][3];
	int		i;
	int		j;
	int		sub_i;
	int		sub_j;

	sub_i = 0;
	i = -1;
	while (++i < 4)
	{
		if (i == r)
			continue ;
		sub_j = 0;
		j = -1;
		while (++j < 4)
		{
			if (j == c)
				continue ;
			sub[sub_i][sub_j++] = m->m[i][j];
		}
		sub_i++;
	}
	if ((r + c) % 2 == 0)
		return (ft_mat3_det(sub));
	return (-ft_mat3_det(sub));
}

/**
 * Calcule l'inverse d'une matrice 4x4
 * Utilise la méthode des cofacteurs / adjugée
 */
void	ft_mat4_inverse(t_mat4 *m, t_mat4 *inv)
{
	double	det;
	int		i;
	int		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			inv->m[j][i] = ft_mat4_cofactor(m, i, j);
	}
	det = m->m[0][0] * inv->m[0][0] + m->m[0][1] * inv->m[1][0] + m->m[0][2]
		* inv->m[2][0] + m->m[0][3] * inv->m[3][0];
	if (det == 0)
		return ;
	det = 1.0 / det;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			inv->m[i][j] *= det;
	}
}
