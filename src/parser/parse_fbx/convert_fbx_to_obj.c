/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_fbx_to_obj.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:15:37 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:17:06 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** Compte le nombre total de vertices et de faces présents dans
** toutes les géométries de la scène FBX.
*/
static void	ft_count_fbx_elements(t_fbx *fbx, int *v, int *f)
{
	t_list		*curr;
	t_geometry	*geo;

	curr = fbx->geo;
	*v = 0;
	*f = 0;
	while (curr)
	{
		geo = (t_geometry *)curr->content;
		if (geo && geo->obj)
		{
			*v += geo->obj->nb_vertices;
			*f += geo->obj->nb_faces;
		}
		curr = curr->next;
	}
}

/*
** Copie les indices de faces d'un objet source vers un objet de destination
** en appliquant un décalage (offset) sur les index de vertices.
*/
static void	ft_copy_faces(t_object *dst, t_object *src, int f_off, int v_off)
{
	int	j;
	int	k;

	j = 0;
	while (j < src->nb_faces)
	{
		dst->faces[f_off + j].count = src->faces[j].count;
		dst->faces[f_off + j].indices = malloc(sizeof(int)
				* src->faces[j].count);
		k = 0;
		while (k < src->faces[j].count)
		{
			dst->faces[f_off + j].indices[k] = src->faces[j].indices[k] + v_off;
			k++;
		}
		j++;
	}
}

/*
** Remplit le grand objet final avec les vertices et les faces
** de toutes les géométries contenues dans la structure FBX.
*/
static void	ft_fill_obj(t_fbx *fbx, t_object *big_obj)
{
	t_list		*curr;
	t_geometry	*geo;
	int			v_off;
	int			f_off;
	int			i;

	curr = fbx->geo;
	v_off = 0;
	f_off = 0;
	while (curr)
	{
		geo = (t_geometry *)curr->content;
		if (geo && geo->obj)
		{
			i = -1;
			while (++i < geo->obj->nb_vertices)
				big_obj->vertices[v_off + i] = geo->obj->vertices[i];
			ft_copy_faces(big_obj, geo->obj, f_off, v_off);
			v_off += geo->obj->nb_vertices;
			f_off += geo->obj->nb_faces;
		}
		curr = curr->next;
	}
}

/*
** Convertit l'ensemble des géométries FBX en un seul objet unifié,
** en allouant la mémoire nécessaire pour les vertices et les faces.
*/
t_object	*ft_convert_fbx_to_object(t_fbx *fbx)
{
	t_object	*big;
	int			total_v;
	int			total_f;

	ft_count_fbx_elements(fbx, &total_v, &total_f);
	big = ft_calloc(1, sizeof(t_object));
	if (!big)
		return (NULL);
	big->nb_vertices = total_v;
	big->nb_faces = total_f;
	big->vertices = malloc(sizeof(t_vec3) * total_v);
	big->faces = malloc(sizeof(t_face) * total_f);
	if (!big->vertices || !big->faces)
		return (ft_free_object(big), NULL);
	ft_fill_obj(fbx, big);
	return (big);
}
