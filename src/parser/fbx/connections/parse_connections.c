/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:14:16 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/15 16:39:15 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_calculate_anim_duration(t_anim_stack *anim)
{
	t_list			*l_layer;
	t_list			*l_node;
	t_anim_layer	*layer;
	t_anim_node		*node;
	double			max_time; // Changed from long long to double
	long long		current_time_placeholder; // unused

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
		anim->duration = max_time; // Already in seconds
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

void	ft_connect_obj_to_obj(t_fbx *data, long *ids)
{
	void	*src;
	void	*dst;

	src = ft_get_by_id(data->geo, ids[0]);
	dst = ft_get_by_id(data->model, ids[1]);
	if (src && dst)
		((t_model *)dst)->geo = (t_geometry *)src;
	else if ((src = ft_get_by_id(data->model, ids[0])) && dst)
		((t_model *)src)->parent = (t_model *)dst;
	else if ((src = ft_get_by_id(data->anim_layer, ids[0])))
	{
		dst = ft_get_by_id(data->anim_stack, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_stack *)dst)->layers, ft_lstnew(src));
	}
	else if ((src = ft_get_by_id(data->anim_node, ids[0])))
	{
		dst = ft_get_by_id(data->anim_layer, ids[1]);
		if (dst)
			ft_lstadd_front(&((t_anim_layer *)dst)->nodes, ft_lstnew(src));
	}
}

void	ft_connect_anim_to_model(t_fbx *data, char *line, long *ids)
{
	t_anim_node	*anim;
	t_model		*model;

	anim = (t_anim_node *)ft_get_by_id(data->anim_node, ids[0]);
	if (anim)
	{
		model = (t_model *)ft_get_by_id(data->model, ids[1]);
		if (model)
		{
			anim->target = model;
			line += 4;
			if (IS_TAG(line, "Translation"))
				model->anim_pos = anim;
			else if (IS_TAG(line, "Rotation"))
				model->anim_rot = anim;
			else if (IS_TAG(line, "Scaling"))
				model->anim_scale = anim;
		}
	}
}

void	ft_connect_anim_to_anim(t_fbx *data, char *line, long *ids)
{
	t_anim_curve	*anim_curve;
	t_anim_node		*anim_node;

	anim_curve = (t_anim_curve *)ft_get_by_id(data->anim_curve, ids[0]);
	if (anim_curve)
	{
		anim_node = (t_anim_node *)ft_get_by_id(data->anim_node, ids[1]);
		if (anim_node)
		{
			line += 2;
			if (IS_TAG(line, "X"))
				anim_node->x = anim_curve;
			else if (IS_TAG(line, "Y"))
				anim_node->y = anim_curve;
			else if (IS_TAG(line, "Z"))
				anim_node->z = anim_curve;
		}
	}
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
