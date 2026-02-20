/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/12 22:36:55 by vlad             ###   ########.fr       */
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
    // 1. SCALE
	point.x *= scale_x;
	point.y *= scale_y;
	point.z *= scale_z;
    // 2. ROTATION X
	ty = point.y * cos(rad_x) - point.z * sin(rad_x);
	tz = point.y * sin(rad_x) + point.z * cos(rad_x);
	point.y = ty;
	point.z = tz;
    // 3. ROTATION Y
	tx = point.x * cos(rad_y) + point.z * sin(rad_y);
	tz = -point.x * sin(rad_y) + point.z * cos(rad_y);
	point.x = tx;
	point.z = tz;
    // 4. ROTATION Z
	tx = point.x * cos(rad_z) - point.y * sin(rad_z);
	ty = point.x * sin(rad_z) + point.y * cos(rad_z);
	point.x = tx;
	point.y = ty;
    // 5. TRANSLATION
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
static t_vec3	ft_apply_mat4(double *m, t_vec3 v)
{
	t_vec3	r;

	if (!m)
		return (v);
	r.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
	r.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
	r.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
	r.color = v.color;
	return (r);
}

/**
 * Calcule l'inverse d'une matrice 4x4
 * Utilise la méthode des cofacteurs / adjugée
 */
static void	ft_mat4_inverse(double *m, double *inv)
{
	double	det;
	int		i;

	inv[0] = m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] +
			 m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
	inv[4] = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] -
			 m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
	inv[8] = m[4]  * m[9]  * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5]  * m[15] +
			 m[8]  * m[7]  * m[13] + m[12] * m[5]  * m[11] - m[12] * m[7]  * m[9];
	inv[12] = -m[4]  * m[9]  * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5]  * m[14] -
			  m[8]  * m[6]  * m[13] - m[12] * m[5]  * m[10] + m[12] * m[6]  * m[9];
	inv[1] = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2]  * m[15] -
			 m[9]  * m[3]  * m[14] - m[13] * m[2]  * m[11] + m[13] * m[3]  * m[10];
	inv[5] = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2]  * m[15] +
			 m[8]  * m[3]  * m[14] + m[12] * m[2]  * m[11] - m[12] * m[3]  * m[10];
	inv[9] = -m[0]  * m[9]  * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1]  * m[15] -
			 m[8]  * m[3]  * m[13] - m[12] * m[1]  * m[11] + m[12] * m[3]  * m[9];
	inv[13] = m[0]  * m[9]  * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1]  * m[14] +
			  m[8]  * m[2]  * m[13] + m[12] * m[1]  * m[10] - m[12] * m[2]  * m[9];
	inv[2] = m[1]  * m[6]  * m[15] - m[1]  * m[7]  * m[14] - m[5]  * m[2]  * m[15] +
			 m[5]  * m[3]  * m[14] + m[13] * m[2]  * m[7]  - m[13] * m[3]  * m[6];
	inv[6] = -m[0]  * m[6]  * m[15] + m[0]  * m[7]  * m[14] + m[4]  * m[2]  * m[15] -
			 m[4]  * m[3]  * m[14] - m[12] * m[2]  * m[7]  + m[12] * m[3]  * m[6];
	inv[10] = m[0]  * m[5]  * m[15] - m[0]  * m[7]  * m[13] - m[4]  * m[1]  * m[15] +
			  m[4]  * m[3]  * m[13] + m[12] * m[1]  * m[7]  - m[12] * m[3]  * m[5];
	inv[14] = -m[0]  * m[5]  * m[14] + m[0]  * m[6]  * m[13] + m[4]  * m[1]  * m[14] -
			  m[4]  * m[2]  * m[13] - m[12] * m[1]  * m[6]  + m[12] * m[2]  * m[5];
	inv[3] = -m[1]  * m[6]  * m[11] + m[1]  * m[7]  * m[10] + m[5]  * m[2]  * m[11] -
			 m[5]  * m[3]  * m[10] - m[9]  * m[2]  * m[7]  + m[9]  * m[3]  * m[6];
	inv[7] = m[0]  * m[6]  * m[11] - m[0]  * m[7]  * m[10] - m[4]  * m[2]  * m[11] +
			 m[4]  * m[3]  * m[10] + m[8]  * m[2]  * m[7]  - m[8]  * m[3]  * m[6];
	inv[11] = -m[0]  * m[5]  * m[11] + m[0]  * m[7]  * m[9]  + m[4]  * m[1]  * m[11] -
			  m[4]  * m[3]  * m[9]  - m[8]  * m[1]  * m[7]  + m[8]  * m[3]  * m[5];
	inv[15] = m[0]  * m[5]  * m[10] - m[0]  * m[6]  * m[9]  - m[4]  * m[1]  * m[10] +
			  m[4]  * m[2]  * m[9]  + m[8]  * m[1]  * m[6]  - m[8]  * m[2]  * m[5];
	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	if (det == 0)
		return ;
	det = 1.0 / det;
	i = -1;
	while (++i < 16)
		inv[i] = inv[i] * det;
}

/**
 * Multiplie deux matrices 4x4 (row-major): result = A × B
 */
static void	ft_mat4_multiply(double *a, double *b, double *out)
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
			out[i * 4 + j] = 0;
			k = -1;
			while (++k < 4)
				out[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
		}
	}
}

/**
 * Construit une matrice 4x4 à partir de pos/rot/scale d'un modèle.
 * Ordre : Scale → RotX → RotY → RotZ → Translation
 */
static void	ft_build_bone_matrix(t_model *mdl, double *out)
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
	out[0] = sx * (cos(ry) * cos(rz));
	out[1] = sx * (cos(ry) * sin(rz));
	out[2] = sx * (-sin(ry));
	out[3] = 0;
	out[4] = sy * (sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz));
	out[5] = sy * (sin(rx) * sin(ry) * sin(rz) + cos(rx) * cos(rz));
	out[6] = sy * (sin(rx) * cos(ry));
	out[7] = 0;
	out[8] = sz * (cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz));
	out[9] = sz * (cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz));
	out[10] = sz * (cos(rx) * cos(ry));
	out[11] = 0;
	out[12] = mdl->pos.x;
	out[13] = mdl->pos.y;
	out[14] = mdl->pos.z;
	out[15] = 1;
}

/**
 * Construit la matrice monde d'un bone en remontant la chaîne parent.
 * world = local × parent_world
 */
static void	ft_get_bone_world_matrix(t_model *bone, double *world, int depth)
{
	double	local[16];
	double	parent_world[16];
	double	tmp[16];

	if (depth > 100)
	{
		fprintf(stderr, "ERROR: Bone recursion depth exceeded for bone ID %ld\n", bone->id);
		return ; // Should probably return identity or handle error better
	}
	ft_build_bone_matrix(bone, local);
	if (!bone->parent)
	{
		ft_memcpy(world, local, sizeof(double) * 16);
		return ;
	}
	ft_get_bone_world_matrix(bone->parent, parent_world, depth + 1);
	ft_mat4_multiply(local, parent_world, tmp);
	ft_memcpy(world, tmp, sizeof(double) * 16);
}

static void	ft_mat4_identity(double *out)
{
	int	i;

	i = -1;
	while (++i < 16)
		out[i] = (i % 5 == 0) ? 1.0 : 0.0;
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
	double		bone_world[16];
	double		transform_geom[16];
	double		inv_bind[16];
	double		m_step1[16];
	double		final_mat[16];
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
				ft_get_bone_world_matrix(def->bone, bone_world, 0);

				if (def->transform)
					ft_memcpy(transform_geom, def->transform, sizeof(double) * 16);
				else
					ft_mat4_identity(transform_geom);

				if (def->t_link)
					ft_mat4_inverse(def->t_link, inv_bind);
				else
					ft_mat4_identity(inv_bind);

				ft_mat4_multiply(transform_geom, inv_bind, m_step1);
				ft_mat4_multiply(m_step1, bone_world, final_mat);

				skinned = ft_apply_mat4(final_mat, vertex);
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
