/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skinning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:02:12 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:09:06 by vbleskin         ###   ########.fr       */
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

	rot[0] = to_rad(model->rot.x);
	rot[1] = to_rad(model->rot.y);
	rot[2] = to_rad(model->rot.z);
	ft_set_scale_vec(model, scale);
	cos_rot[0] = cos(rot[0]);
	cos_rot[1] = cos(rot[1]);
	cos_rot[2] = cos(rot[2]);
	sin_rot[0] = sin(rot[0]);
	sin_rot[1] = sin(rot[1]);
	sin_rot[2] = sin(rot[2]);
	out->m[0][0] = scale[0] * (cos_rot[1] * cos_rot[2]);
	out->m[0][1] = scale[0] * (cos_rot[1] * sin_rot[2]);
	out->m[0][2] = scale[0] * (-sin_rot[1]);
	out->m[0][3] = 0;
	out->m[1][0] = scale[1] * (sin_rot[0] * sin_rot[1]
			* cos_rot[2] - cos_rot[0] * sin_rot[2]);
	out->m[1][1] = scale[1] * (sin_rot[0] * sin_rot[1]
			* sin_rot[2] + cos_rot[0] * cos_rot[2]);
	out->m[1][2] = scale[1] * (sin_rot[0] * cos_rot[1]);
	out->m[1][3] = 0;
	out->m[2][0] = scale[2] * (cos_rot[0] * sin_rot[1]
			* cos_rot[2] + sin_rot[0] * sin_rot[2]);
	out->m[2][1] = scale[2] * (cos_rot[0] * sin_rot[1]
			* sin_rot[2] - sin_rot[0] * cos_rot[2]);
	out->m[2][2] = scale[2] * (cos_rot[0] * cos_rot[1]);
	out->m[2][3] = 0;
	out->m[3][0] = model->pos.x;
	out->m[3][1] = model->pos.y;
	out->m[3][2] = model->pos.z;
	out->m[3][3] = 1;
}

/**
 * Construit la matrice monde d'un bone en remontant la chaîne parent.
 * world = local × parent_world
 */
static void ft_get_bone_world_matrix(t_model *bone, t_mat4 *world, int depth)
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

static t_vec3	ft_get_new_pos(t_geometry *geo, t_model *model, int i)
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

static void	ft_process_geo_vertices(t_fdf *data, t_geometry *geo, int *g_idx)
{
	t_model	*model;
	t_vec3	new_pos;
	int		i;

	model = find_model_for_geo(data->fbx->model, geo);
	i = 0;
	while (i < geo->obj->nb_vertices)
	{
		new_pos = ft_get_new_pos(geo, model, i);
		data->object->vertices[*g_idx + i] = new_pos;
		i++;
	}
	*g_idx += geo->obj->nb_vertices;
}

/**
 * Met à jour les vertices du maillage à partir des animations.
 * Pour les modèles avec deformers (skinning squelettique),
 * applique la formule de skinning par vertex.
 * Pour les modèles sans deformers, applique le transform rigide.
 */
void	ft_update_mesh_from_animation(t_fdf *data)
{
	t_list		*curr_geo;
	t_geometry	*geo;
	int			global_index;

	if (!data->fbx || !data->object)
		return ;
	global_index = 0;
	curr_geo = data->fbx->geo;
	while (curr_geo)
	{
		geo = (t_geometry *)curr_geo->content;
		if (geo->obj)
			ft_process_geo_vertices(data, geo, &global_index);
		curr_geo = curr_geo->next;
	}
}
