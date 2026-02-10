/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:14:21 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/09 05:39:43 by vbleskin         ###   ########.fr       */
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
static double to_rad(double degree)
{
    return (degree * M_PI / 180.0);
}

/**
 * fonction test pour animation
 */
static t_vec3 apply_transform(t_vec3 point, t_properties p, t_properties r, t_properties s)
{
	double	tx, ty, tz;
	double	rad_x = to_rad(r.x);
	double	rad_y = to_rad(r.y);
	double	rad_z = to_rad(r.z);

    // 1. SCALE
	point.x *= s.x;
	point.y *= s.y;
	point.z *= s.z;
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


static t_vec3	ft_get_world_transform(t_vec3 point, t_model *model)
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
static t_model	*find_model_for_geo(t_list *models, t_geometry *target_geo)
{
	t_model	*mdl;

	while (models)
	{
		mdl = (t_model *)models->content;
        // On suppose que ton parser a lié mdl->geo au bon pointeur t_geometry
		if (mdl->geo == target_geo)
			return (mdl);
		models = models->next;
	}
	return (NULL);
}

/**
 * fonction test pour animation
 */
void    ft_update_mesh_from_animation(t_fdf *data)
{
	t_list		*curr_geo;
	t_geometry	*geo;
	t_model		*mdl;
	int			global_index; // Index dans le Big Object
	int			i;
	t_vec3		new_pos;
    // Valeurs par défaut (Identité) si pas d'anim trouvée
	t_properties	def_pos = {0, 0, 0, 0};
	t_properties	def_rot = {0, 0, 0, 0};
	t_properties	def_scale = {0, 1, 1, 1};

	if (!data->fbx || !data->object)
		return ;
	global_index = 0;
	curr_geo = data->fbx->geo;
    // On parcourt les géométries originales stockées dans le FBX
	while (curr_geo)
	{
		geo = (t_geometry *)curr_geo->content;
        // On cherche quel "Model" anime cette géométrie
		mdl = find_model_for_geo(data->fbx->model, geo);
		if (mdl) // debug
        {
            // On a trouvé un modèle, l'animation devrait marcher
            printf("GEO LINKED! GeoID: %ld -> ModelID: %ld | Pos: %f %f %f\n", 
                   geo->id, mdl->id, mdl->pos.x, mdl->pos.y, mdl->pos.z);
        }
        else // debug
        {
            // Pas de modèle trouvé = Pas d'animation appliquée
            printf("GEO ORPHAN! GeoID: %ld has no Model attached.\n", geo->id);
        }
		if (geo->obj)
		{
			i = 0;
			while (i < geo->obj->nb_vertices)
			{
                // CRUCIAL : On prend les coordonnées d'origine (geo->obj)
                // On applique les transfs du modèle (mdl->pos/rot/scale) mises à jour par ft_animate
				if (mdl)
					new_pos = ft_get_world_transform(geo->obj->vertices[i], mdl);
				else
					new_pos = apply_transform(geo->obj->vertices[i], def_pos, def_rot, def_scale);
				// On préserve la couleur d'origine
				new_pos.color = geo->obj->vertices[i].color;
				// On écrit le résultat dans le "Big Object" qui sert à l'affichage
				data->object->vertices[global_index + i] = new_pos;
				i++;
			}
			// On avance l'index global pour la prochaine géométrie
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
	ft_update_time(&data->timer);
	if (data->fbx && data->fbx->current_anim)
	{
		double	anim_duration = 0.0;
		if (data->fbx->current_anim->layers)
			anim_duration = 4.2;
		if (anim_duration > 0.001)
			data->timer.weighted_value = fmod(data->timer.weighted_value, anim_duration);
		ft_animate(data);
		ft_update_mesh_from_animation(data);
	}
	ft_bzero(data->img.addr, WIN_WIDTH * WIN_HEIGHT * (data->img.bits_per_pixel / 8));
	ft_transform_threads(data);
	ft_draw_threads(data);
	ft_draw_axes(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.ptr, 0, 0);
	ft_display_fps(data);
	if (data->fbx->anim_stack)
		ft_display_anim_menu(data);
}
