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
 * Cherche un modèle animé avec de vraies keyframes (>1 keys)
 * pour propager l'animation racine aux maillages squelettiques.
 */
static t_model	*ft_find_animated_bone(t_list *models)
{
	t_model	*mdl;
	t_model	*best;
	int		max_keys;
	int		keys;

	best = NULL;
	max_keys = 1;
	while (models)
	{
		mdl = (t_model *)models->content;
		keys = 0;
		if (mdl->anim_pos && mdl->anim_pos->x)
			keys = mdl->anim_pos->x->n_keys;
		if (mdl->anim_rot && mdl->anim_rot->x
			&& mdl->anim_rot->x->n_keys > keys)
			keys = mdl->anim_rot->x->n_keys;
		if (keys > max_keys)
		{
			max_keys = keys;
			best = mdl;
		}
		models = models->next;
	}
	return (best);
}

/**
 * Vérifie si un modèle est un maillage squelettique :
 * il a des liens d'animation (anim_pos/rot/scale non NULL)
 * mais avec seulement 1 keyframe (= pose de repos, pas d'animation réelle).
 * Les modèles sans AUCUN lien d'animation (ex: escaliers) ne sont PAS
 * des maillages squelettiques et ne doivent PAS recevoir d'animation d'os.
 */
static int	ft_model_is_skeletal_mesh(t_model *mdl)
{
	int	has_anim_link;
	int	has_real_keys;

	has_anim_link = (mdl->anim_pos != NULL || mdl->anim_rot != NULL
			|| mdl->anim_scale != NULL);
	if (!has_anim_link)
		return (0);
	has_real_keys = 0;
	if (mdl->anim_pos && mdl->anim_pos->x
		&& mdl->anim_pos->x->n_keys > 1)
		has_real_keys = 1;
	if (mdl->anim_rot && mdl->anim_rot->x
		&& mdl->anim_rot->x->n_keys > 1)
		has_real_keys = 1;
	if (mdl->anim_scale && mdl->anim_scale->x
		&& mdl->anim_scale->x->n_keys > 1)
		has_real_keys = 1;
	return (!has_real_keys);
}

/**
 * Met à jour les vertices du maillage à partir des animations.
 * Pour les modèles squelettiques (parrot, etc.), applique aussi
 * l'animation de l'os racine pour donner du mouvement global.
 */
void    ft_update_mesh_from_animation(t_fdf *data)
{
	t_list			*curr_geo;
	t_geometry		*geo;
	t_model			*mdl;
	t_model			*bone;
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
		bone = NULL;
		if (mdl && ft_model_is_skeletal_mesh(mdl))
			bone = ft_find_animated_bone(data->fbx->model);
		if (geo->obj)
		{
			i = 0;
			while (i < geo->obj->nb_vertices)
			{
				if (mdl)
					new_pos = ft_get_world_transform(geo->obj->vertices[i], mdl);
				else
					new_pos = apply_transform(geo->obj->vertices[i], def_pos, def_rot, def_scale);
				if (bone)
					new_pos = ft_get_world_transform(new_pos, bone);
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
