/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:14:16 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/06 20:14:46 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
		{
			// printf("STACK CONNEXION : Layer %ld -> Stack %ld\n", ids[0], ids[1]); //debug
			ft_lstadd_front(&((t_anim_stack *)dst)->layers, ft_lstnew(src));
		}
		// else //debug
		// 	printf("FAIL STACK : Layer %ld veut se connecter à %ld (Introuvable)\n", ids[0], ids[1]);
	}
	else if ((src = ft_get_by_id(data->anim_node, ids[0])))
	{
		dst = ft_get_by_id(data->anim_layer, ids[1]);
		if (dst)
		{
			// printf("LAYER CONNEXION : Layer %ld \n", ids[1]); //debug
			ft_lstadd_front(&((t_anim_layer *)dst)->nodes, ft_lstnew(src));
		}
		// else //debug
		// {
		// 	static int printed = 0;
		// 	if (!printed)
		// 	{
		// 		printf("Taille liste AnimNodes: %d\n", ft_lstsize(data->anim_node));
		// 		printed = 1;
		// 	}
		// }
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
