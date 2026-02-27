/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:14:16 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:58:32 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_calculate_anim_duration(t_anim_stack *anim)
{
	t_list			*l_layer;
	t_list			*l_node;
	t_anim_layer	*layer;
	t_anim_node		*node;
	double			max_time;

	max_time = 0;
	l_layer = anim->layers;
	while (l_layer)
	{
		layer = (t_anim_layer *)l_layer->content;
		l_node = layer->nodes;
		while (l_node)
		{
			node = (t_anim_node *)l_node->content;
			if (node->x && node->x->n_keys > 0)
			{
				if (node->x->time[node->x->n_keys - 1] > max_time)
					max_time = node->x->time[node->x->n_keys - 1];
			}
			if (node->y && node->y->n_keys > 0)
			{
				if (node->y->time[node->y->n_keys - 1] > max_time)
					max_time = node->y->time[node->y->n_keys - 1];
			}
			if (node->z && node->z->n_keys > 0)
			{
				if (node->z->time[node->z->n_keys - 1] > max_time)
					max_time = node->z->time[node->z->n_keys - 1];
			}
			l_node = l_node->next;
		}
		l_layer = l_layer->next;
	}
	if (max_time > 0)
		anim->duration = max_time;
	else
		anim->duration = 1.0;
}

long	*ft_read_ids(char *line, long *ids)
{
	line = ft_strchr(line, ',');
	if (*line)
		line++;
	ids[0] = ft_atol(line);
	line = ft_strchr(line, ',');
	if (line && *line)
		line++;
	ids[1] = ft_atol(line);
	return (ids);
}

static t_geometry	*ft_find_geo_with_deformer(t_fbx *data, long skin_id)
{
	t_list		*geos;
	t_geometry	*geo;
	t_list		*defs;
	t_deformer	*def;

	geos = data->geo;
	while (geos)
	{
		geo = (t_geometry *)geos->content;
		defs = geo->deformers;
		while (defs)
		{
			def = (t_deformer *)defs->content;
			if (def->id == skin_id)
				return (geo);
			defs = defs->next;
		}
		geos = geos->next;
	}
	return (NULL);
}

static void	ft_extract_connection(t_fbx *fbx_data, char *line)
{
	long	ids[2];

	line = ft_strchr(line, '\"');
	if (*line)
		line++;
	ft_read_ids(line, ids);
	if (IS_TAG(line, "OO"))
		ft_connect_obj_to_obj(fbx_data, ids);
	else if (IS_TAG(line, "OP"))
	{
		line = ft_strrchr(line, ',');
		while (*line && (*line == ' ' || *line == '\"' || *line == ','))
			line++;
		if (IS_TAG(line, "Lcl"))
			ft_connect_anim_to_model(fbx_data, line, ids);
		else if (IS_TAG(line, "d|"))
			ft_connect_anim_to_anim(fbx_data, line, ids);
	}
}

/**
 * Fonction de parsing pour recuperer les donnes dans la partie 'Connections'
 * d'un fichier .fbx
 */
int	ft_parse_connections(t_fbx *fbx_data, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line || line[0] == '}')
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "C:"))
			ft_extract_connection(fbx_data, cursor);
		free(line);
	}
	return (SUCCESS);
}
