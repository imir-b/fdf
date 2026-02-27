/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 00:36:47 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction du thread, je lui passe une structure 't_thread' en argument car
 * elle ne peut prendre qu'un argument void *.
 * J'itere 'start' jusqu'a 'end' pour trouver tous les points 'p' et j
 */
void	*ft_calc_transform_thread(void *arg)
{
	t_thread	*thread;
	t_fdf		*data;
	int			start;

	thread = (t_thread *)arg;
	data = thread->data;
	start = thread->start;
	while (start < thread->end)
	{
		ft_project_point(&data->object->vertices[start], data);
		start++;
	}
	return (NULL);
}

/**
 * Utilise le multi-threading pour faire les calculs de projection en fonction
 * des transformations.
 */
static void	ft_transform_threads(t_fdf *data)
{
	pthread_t	threads[THREADS_NB];
	t_thread	args[THREADS_NB];
	int			i;
	int			chunk;

	chunk = data->object->nb_vertices / THREADS_NB;
	i = -1;
	while (++i < THREADS_NB)
	{
		args[i].data = data;
		args[i].id = i;
		args[i].start = i * chunk;
		args[i].end = (i + 1) * chunk;
		if (i == THREADS_NB - 1)
			args[i].end = data->object->nb_vertices;
		pthread_create(&threads[i], NULL, ft_calc_transform_thread, &args[i]);
	}
	i = -1;
	while (++i < THREADS_NB)
		pthread_join(threads[i], NULL);
}

/**
 * fonction test pour animation
 */
double to_rad(double degree)
{
    return (degree * M_PI / 180.0);
}

/**
 * fonction test pour animation
 */
t_vec3 apply_transform(t_vec3 point, t_properties p, t_properties r, t_properties s)
{
	double	tx, ty, tz;
	double	rad_x;
	double	rad_y;
	double	rad_z;
	double	scale_x;
	double	scale_y;
	double	scale_z;

	rad_x = to_rad(r.x);
	rad_y = to_rad(r.y);
	rad_z = to_rad(r.z);
	if (s.x == 0.0)
		scale_x = 1.0;
	else
		scale_x = s.x;
	if (s.y == 0.0)
		scale_y = 1.0;
	else
		scale_y = s.y;
	if (s.z == 0.0)
		scale_z = 1.0;
	else
		scale_z = s.z;
	point.x *= scale_x;
	point.y *= scale_y;
	point.z *= scale_z;
	ty = point.y * cos(rad_x) - point.z * sin(rad_x);
	tz = point.y * sin(rad_x) + point.z * cos(rad_x);
	point.y = ty;
	point.z = tz;
	tx = point.x * cos(rad_y) + point.z * sin(rad_y);
	tz = -point.x * sin(rad_y) + point.z * cos(rad_y);
	point.x = tx;
	point.z = tz;
	tx = point.x * cos(rad_z) - point.y * sin(rad_z);
	ty = point.x * sin(rad_z) + point.y * cos(rad_z);
	point.x = tx;
	point.y = ty;
	point.x += p.x;
	point.y += p.y;
	point.z += p.z;
	return (point);
}

t_vec3	ft_get_world_transform(t_vec3 point, t_model *model)
{
	t_vec3	curr_point;

	curr_point = point;
	while (model)
	{
		curr_point = apply_transform(curr_point, model->pos, model->rot, model->scale);
		model = model->parent;
	}
	return (curr_point);
}

/**
 * fonction test pour animation
 */
t_model	*find_model_for_geo(t_list *models, t_geometry *target_geo)
{
	t_model	*mdl;

	while (models)
	{
		mdl = (t_model *)models->content;
		if (mdl->geo == target_geo)
			return (mdl);
		models = models->next;
	}
	return (NULL);
}
/**
 * Multiplie une matrice 4x4 (row-major FBX) par un vec3 (w=1).
 */
static t_vec3	ft_apply_mat4(t_mat4 *mat, t_vec3 v)
{
	t_vec3	r;

	if (!mat)
		return (v);
	r.x = v.x * mat->m[0][0] + v.y * mat->m[1][0] + v.z * mat->m[2][0] + mat->m[3][0];
	r.y = v.x * mat->m[0][1] + v.y * mat->m[1][1] + v.z * mat->m[2][1] + mat->m[3][1];
	r.z = v.x * mat->m[0][2] + v.y * mat->m[1][2] + v.z * mat->m[2][2] + mat->m[3][2];
	r.color = v.color;
	return (r);
}

/**
 * Calcule l'inverse d'une matrice 4x4
 * Utilise la méthode des cofacteurs / adjugée
 */
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
			sub[sub_i][sub_j] = m->m[i][j];
			sub_j++;
		}
		sub_i++;
	}
	if ((r + c) % 2 == 0)
		return (ft_mat3_det(sub));
	return (-ft_mat3_det(sub));
}

static void	ft_mat4_inverse(t_mat4 *m, t_mat4 *inv)
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
	det = m->m[0][0] * inv->m[0][0] + m->m[0][1] * inv->m[1][0]
		+ m->m[0][2] * inv->m[2][0] + m->m[0][3] * inv->m[3][0];
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
 * Construit une matrice 4x4 à partir de pos/rot/scale d'un modèle.
 * Ordre : Scale → RotX → RotY → RotZ → Translation
 */
static void	ft_build_bone_matrix(t_model *mdl, t_mat4 *out)
{
	double	rx;
	double	ry;
	double	rz;
	double	sx;
	double	sy;
	double	sz;

	rx = to_rad(mdl->rot.x);
	ry = to_rad(mdl->rot.y);
	rz = to_rad(mdl->rot.z);
	sx = (mdl->scale.x == 0.0) ? 1.0 : mdl->scale.x;
	sy = (mdl->scale.y == 0.0) ? 1.0 : mdl->scale.y;
	sz = (mdl->scale.z == 0.0) ? 1.0 : mdl->scale.z;
	out->m[0][0] = sx * (cos(ry) * cos(rz));
	out->m[0][1] = sx * (cos(ry) * sin(rz));
	out->m[0][2] = sx * (-sin(ry));
	out->m[0][3] = 0;
	out->m[1][0] = sy * (sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz));
	out->m[1][1] = sy * (sin(rx) * sin(ry) * sin(rz) + cos(rx) * cos(rz));
	out->m[1][2] = sy * (sin(rx) * cos(ry));
	out->m[1][3] = 0;
	out->m[2][0] = sz * (cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz));
	out->m[2][1] = sz * (cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz));
	out->m[2][2] = sz * (cos(rx) * cos(ry));
	out->m[2][3] = 0;
	out->m[3][0] = mdl->pos.x;
	out->m[3][1] = mdl->pos.y;
	out->m[3][2] = mdl->pos.z;
	out->m[3][3] = 1;
}

/**
 * Construit la matrice monde d'un bone en remontant la chaîne parent.
 * world = local × parent_world
 */
static void	ft_get_bone_world_matrix(t_model *bone, t_mat4 *world, int depth)
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
 * Applique le skinning sur un vertex donné en utilisant tous les deformers.
 * Formule : final = Transform * Inverse(TransformLink) * BoneWorld
 */
static t_vec3	ft_skin_vertex(t_vec3 vertex, int vtx_idx, t_list *deformers)
{
	t_vec3		result;
	t_vec3		skinned;
	t_deformer	*def;
	double		total_weight;
	t_mat4		bone_world;
	t_mat4		transform_geo;
	t_mat4		inv_bind;
	t_mat4		m_step1;
	t_mat4		final_mat;
	int			j;

	result.x = 0;
	result.y = 0;
	result.z = 0;
	total_weight = 0;
	while (deformers)
	{
		def = (t_deformer *)deformers->content;
		if (!def || !def->bone || !def->verticies || !def->weights)
			break ;
		j = -1;
		while (++j < def->n_vertices)
		{
			if (def->verticies[j] == vtx_idx)
			{
				ft_get_bone_world_matrix(def->bone, &bone_world, 0);

				if (def->transform)
					ft_memcpy(&transform_geo, def->transform, sizeof(t_mat4));
				else
					ft_mat4_identity(&transform_geo);

				if (def->t_link)
					ft_mat4_inverse(def->t_link, &inv_bind);
				else
					ft_mat4_identity(&inv_bind);

				ft_mat4_multiply(&transform_geo, &inv_bind, &m_step1);
				ft_mat4_multiply(&m_step1, &bone_world, &final_mat);

				skinned = ft_apply_mat4(&final_mat, vertex);
				result.x += def->weights[j] * skinned.x;
				result.y += def->weights[j] * skinned.y;
				result.z += def->weights[j] * skinned.z;
				total_weight += def->weights[j];
				break ;
			}
		}
		deformers = deformers->next;
	}
	if (total_weight > 0.001)
	{
		result.x /= total_weight;
		result.y /= total_weight;
		result.z /= total_weight;
	}
	else
		return (vertex);
	result.color = vertex.color;
	return (result);
}


/**
 * Met à jour les vertices du maillage à partir des animations.
 * Pour les modèles avec deformers (skinning squelettique),
 * applique la formule de skinning par vertex.
 * Pour les modèles sans deformers, applique le transform rigide.
 */
void    ft_update_mesh_from_animation(t_fdf *data)
{
	t_list			*curr_geo;
	t_geometry		*geo;
	t_model			*mdl;
	int				global_index;
	int				i;
	t_vec3			new_pos;
	t_properties	def_pos = {0, 0, 0, 0};
	t_properties	def_rot = {0, 0, 0, 0};
	t_properties	def_scale = {0, 1, 1, 1};

	if (!data->fbx || !data->object)
		return ;
	global_index = 0;
	curr_geo = data->fbx->geo;
	while (curr_geo)
	{
		geo = (t_geometry *)curr_geo->content;
		mdl = find_model_for_geo(data->fbx->model, geo);
		if (geo->obj)
		{
			i = 0;
			while (i < geo->obj->nb_vertices)
			{
				if (geo->deformers)
					new_pos = ft_skin_vertex(geo->obj->vertices[i],
							i, geo->deformers);
				else if (mdl)
					new_pos = ft_get_world_transform(
							geo->obj->vertices[i], mdl);
				else
					new_pos = apply_transform(geo->obj->vertices[i],
							def_pos, def_rot, def_scale);
				new_pos.color = geo->obj->vertices[i].color;
				data->object->vertices[global_index + i] = new_pos;
				i++;
			}
			global_index += geo->obj->nb_vertices;
		}
		curr_geo = curr_geo->next;
	}
}

/**
 * Fonction qui rend l'image en 6 etapes :
 * - 1 Pre-calcul des maths pour eviter de repeter les calculs lourds,
 * - 2 Projection : J'appelle ft_transform_threads pour faire les calculs des 
 * transformations,
 * - 3 Nettoyage : J'appelle bzero sur 'addr' pour effacer l'ancienne image,
 * - 4 Rasterization : j'appelle ft_draw_threads pour dessiner la nouvelle image,
 * - 5 Affichage : j'appelle mlx_put_image_to_window pour affichier l'image sur 
 * l'ecran.
 * - 6 User interface : j'appelle les fonctions qui affichent les éléments de
 * l'interface.
 */
void	ft_render_image(t_fdf *data)
{
	data->trigo.sin_alpha = sin(data->camera->angle_x);
	data->trigo.cos_alpha = cos(data->camera->angle_x);
	data->trigo.sin_beta = sin(data->camera->angle_y);
	data->trigo.cos_beta = cos(data->camera->angle_y);
	if (data->fbx && data->fbx->current_anim)
	{
		ft_update_time(&data->timer);
		if (isnan(data->timer.weighted_value) || isinf(data->timer.weighted_value))
			data->timer.weighted_value = 0.0;
		if (data->fbx->current_anim->duration > 0.001)
			data->timer.weighted_value = fmod(data->timer.weighted_value, \
			data->fbx->current_anim->duration);
		ft_animate(data);
		ft_update_mesh_from_animation(data);
	}
	ft_bzero(data->img.addr, WIN_WIDTH * WIN_HEIGHT * (data->img.bits_per_pixel / 8));
	ft_transform_threads(data);
	ft_draw_threads(data);
	ft_draw_axes(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.ptr, 0, 0);
	ft_display_fps(data);
	if (data->fbx && data->fbx->anim_stack)
		ft_display_anim_menu(data);
}
