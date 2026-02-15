/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/13 15:26:07 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

static void	ft_copy_faces(t_object *dst, t_object *src, int f_off, int v_off)
{
	int	j;
	int	k;

	j = 0;
	while (j < src->nb_faces)
	{
		dst->faces[f_off + j].count = src->faces[j].count;
		dst->faces[f_off + j].indices = malloc(sizeof(int) * src->faces[j].count);
		k = 0;
		while (k < src->faces[j].count)
		{
			dst->faces[f_off + j].indices[k] = src->faces[j].indices[k] + v_off;
			k++;
		}
		j++;
	}
}

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

/**
 * Lit le fichier 'filename'.fbx pour remplir la structure 'fbx_data'.
 * On separe la lecture en deux parties :
 * 
 * 	Objects:	{
 * 		...
 * 	}
 * 
 * et :
 * 
 * 	Connections:  {
 * 		...
 * 	}
 * 
 */
t_fbx	*ft_parse_fbx(const char *filename)
{
	int		fd;
	char	*line;
	t_fbx	*fbx_data;

	fbx_data = ft_calloc(1, sizeof(t_fbx));
	if (!fbx_data)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (free(fbx_data), NULL);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		else if (IS_TAG(line, "Objects"))
			ft_parse_objects(fbx_data, fd);
		else if (IS_TAG(line, "Connections"))
			ft_parse_connections(fbx_data, fd);
		free(line);
	}
	close(fd);
	return (fbx_data);
}
