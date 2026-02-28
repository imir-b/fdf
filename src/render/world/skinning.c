/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skinning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:02:12 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:44:53 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_compute_skin_mat(t_deformer *def, t_mat4 *final)
{
	t_mat4	bone_world;
	t_mat4	trans_geo;
	t_mat4	inv_bind;
	t_mat4	m_step1;

	ft_get_bone_world_matrix(def->bone, &bone_world, 0);
	if (def->transform)
		ft_memcpy(&trans_geo, def->transform, sizeof(t_mat4));
	else
		ft_mat4_identity(&trans_geo);
	if (def->t_link)
		ft_mat4_inverse(def->t_link, &inv_bind);
	else
		ft_mat4_identity(&inv_bind);
	ft_mat4_multiply(&trans_geo, &inv_bind, &m_step1);
	ft_mat4_multiply(&m_step1, &bone_world, final);
}

static void	ft_apply_weight(t_deformer *def, int j, t_vec3 v, t_vec3 *res)
{
	t_mat4	final_mat;
	t_vec3	skinned;

	ft_compute_skin_mat(def, &final_mat);
	skinned = ft_apply_mat4(&final_mat, v);
	res->x += def->weights[j] * skinned.x;
	res->y += def->weights[j] * skinned.y;
	res->z += def->weights[j] * skinned.z;
}

/**
 * Applique le skinning sur un vertex donné en utilisant tous les deformers.
 * Formule : final = Transform * Inverse(TransformLink) * BoneWorld
 */
static t_vec3	ft_skin_vertex(t_vec3 vertex, int vtx_idx, t_list *deformers)
{
	t_vec3		res;
	t_deformer	*def;
	double		total_w;
	int			j;

	res = (t_vec3){0, 0, 0, 0, 0, vertex.color};
	total_w = 0;
	while (deformers)
	{
		def = (t_deformer *)deformers->content;
		j = -1;
		while (def && ++j < def->n_vertices)
		{
			if (def->verticies[j] == vtx_idx)
			{
				ft_apply_weight(def, j, vertex, &res);
				total_w += def->weights[j];
				break ;
			}
		}
		deformers = deformers->next;
	}
	if (total_w <= 0.001)
		return (vertex);
	return (res.x /= total_w, res.y /= total_w, res.z /= total_w, res);
}

t_vec3	ft_get_new_pos(t_geometry *geo, t_model *model, int i)
{
	t_vec3	v;
	t_vec3	res;

	v = geo->obj->vertices[i];
	if (geo->deformers)
		res = ft_skin_vertex(v, i, geo->deformers);
	else if (model)
		res = ft_get_world_transform(v, model);
	else
		res = apply_transform(v, (t_properties){0, 0, 0, 0},
				(t_properties){0, 0, 0, 0}, (t_properties){0, 1, 1, 1});
	res.color = v.color;
	return (res);
}
