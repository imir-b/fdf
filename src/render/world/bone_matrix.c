/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bone_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 22:44:11 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:44:57 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_set_scale_vec(t_model *model, double s[3])
{
	s[0] = model->scale.x;
	if (s[0] == 0.0)
		s[0] = 1.0;
	s[1] = model->scale.y;
	if (s[1] == 0.0)
		s[1] = 1.0;
	s[2] = model->scale.z;
	if (s[2] == 0.0)
		s[2] = 1.0;
}

static void	ft_set_bone_rotation(t_mat4 *out, double c[3], double s[3],
				double sc[3])
{
	out->m[0][0] = sc[0] * (c[1] * c[2]);
	out->m[0][1] = sc[0] * (c[1] * s[2]);
	out->m[0][2] = sc[0] * (-s[1]);
	out->m[0][3] = 0;
	out->m[1][0] = sc[1] * (s[0] * s[1] * c[2] - c[0] * s[2]);
	out->m[1][1] = sc[1] * (s[0] * s[1] * s[2] + c[0] * c[2]);
	out->m[1][2] = sc[1] * (s[0] * c[1]);
	out->m[1][3] = 0;
	out->m[2][0] = sc[2] * (c[0] * s[1] * c[2] + s[0] * s[2]);
	out->m[2][1] = sc[2] * (c[0] * s[1] * s[2] - s[0] * c[2]);
	out->m[2][2] = sc[2] * (c[0] * c[1]);
	out->m[2][3] = 0;
}

/**
 * Construit une matrice 4x4 à partir de pos/rot/scale d'un modèle.
 * Ordre : Scale → RotX → RotY → RotZ → Translation
 */
static void	ft_build_bone_matrix(t_model *model, t_mat4 *out)
{
	double	rot[3];
	double	scale[3];
	double	cos_rot[3];
	double	sin_rot[3];

	rot[0] = ft_to_rad(model->rot.x);
	rot[1] = ft_to_rad(model->rot.y);
	rot[2] = ft_to_rad(model->rot.z);
	ft_set_scale_vec(model, scale);
	cos_rot[0] = cos(rot[0]);
	cos_rot[1] = cos(rot[1]);
	cos_rot[2] = cos(rot[2]);
	sin_rot[0] = sin(rot[0]);
	sin_rot[1] = sin(rot[1]);
	sin_rot[2] = sin(rot[2]);
	ft_set_bone_rotation(out, cos_rot, sin_rot, scale);
	out->m[3][0] = model->pos.x;
	out->m[3][1] = model->pos.y;
	out->m[3][2] = model->pos.z;
	out->m[3][3] = 1;
}

/**
 * Construit la matrice monde d'un bone en remontant la chaîne parent.
 * world = local × parent_world
 */
void	ft_get_bone_world_matrix(t_model *bone, t_mat4 *world, int depth)
{
	t_mat4	local;
	t_mat4	parent_world;
	t_mat4	tmp;

	if (depth > 100)
		return ;
	ft_build_bone_matrix(bone, &local);
	if (!bone->parent)
	{
		ft_memcpy(world, &local, sizeof(t_mat4));
		return ;
	}
	ft_get_bone_world_matrix(bone->parent, &parent_world, depth + 1);
	ft_mat4_multiply(&local, &parent_world, &tmp);
	ft_memcpy(world, &tmp, sizeof(t_mat4));
}
