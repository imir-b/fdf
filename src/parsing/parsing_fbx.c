/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/02 02:24:44 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_object *ft_convert_fbx_to_object(t_fbx *fbx)
{
	t_object    *big_obj;
    t_geometry  *geo;
    t_list      *curr;
    int         total_v;
    int         total_f;

    total_v = 0;
    total_f = 0;
    curr = fbx->geo;
    while (curr)
    {
        geo = (t_geometry *)curr->content;
        if (geo && geo->obj)
        {
            total_v += geo->obj->nb_vertices;
            total_f += geo->obj->nb_faces;
        }
        curr = curr->next;
    }
    big_obj = ft_calloc(1, sizeof(t_object));
    if (!big_obj)
		return (NULL);
    big_obj->nb_vertices = total_v;
    big_obj->nb_faces = total_f;
    big_obj->vertices = malloc(sizeof(t_vec3) * total_v);
    big_obj->faces = malloc(sizeof(t_face) * total_f);
    if (!big_obj->vertices || !big_obj->faces)
        return (ft_free_object(big_obj), NULL);
    int v_offset = 0;
    int f_offset = 0;
    curr = fbx->geo;
    while (curr)
    {
        geo = (t_geometry *)curr->content;
        if (geo && geo->obj)
        {
            int i = 0;
            while (i < geo->obj->nb_vertices)
            {
                big_obj->vertices[v_offset + i] = geo->obj->vertices[i];
                i++;
            }
            int j = 0;
            while (j < geo->obj->nb_faces)
            {
                big_obj->faces[f_offset + j].count = geo->obj->faces[j].count;
                big_obj->faces[f_offset + j].indices = malloc(sizeof(int) * geo->obj->faces[j].count);
                int k = 0;
                while (k < geo->obj->faces[j].count)
                {
                    big_obj->faces[f_offset + j].indices[k] = \
                        geo->obj->faces[j].indices[k] + v_offset;
                    k++;
                }
                j++;
            }
            v_offset += geo->obj->nb_vertices;
            f_offset += geo->obj->nb_faces;
        }
        curr = curr->next;
    }
    return (big_obj);
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
