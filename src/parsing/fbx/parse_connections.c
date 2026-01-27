/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:14:16 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/27 21:34:38 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_anim_curve	*ft_find_anim_curve_by_id(int id, t_anim_curve *node)
{
	while (node)
	{
		if (node->id == id)
			return (node);
		node = node->next;
	}
	return (NULL);
}

t_anim_node	*ft_find_anim_node_by_id(int id, t_anim_node *node)
{
	while (node)
	{
		if (node->id == id)
			return (node);
		node = node->next;
	}
	return (NULL);
}

t_geometry	*ft_find_geo_by_id(int id, t_geometry *node)
{
	while (node)
	{
		if (node->id == id)
			return (node);
		node = node->next;
	}
	return (NULL);
}

t_model	*ft_find_model_by_id(int id, t_model *node)
{
	while (node)
	{
		if (node->id == id)
			return (node);
		node = node->next;
	}
	return (NULL);
}

int	*ft_read_ids(char *line)
{
	int	ids[2];

	line = ft_strchr(line, ',');
	if (*line)
		line++;
	ids[0] = ft_atoi(line);
	line = ft_strchr(line, ',');
	if (*line)
		line++;
	ids[1] = ft_atoi(line);
	return (ids);
}

// pseudo code :
static void	ft_extract_connection(t_fbx *fbx_data, char *line)
{
	int	ids[2];

	line = ft_strchr(line, '\"') + 1;
	ids = ft_read_ids(line);
	if (IS_TAG(line, "OO"))
	{
		fbx_data->model; //get by id 1
		fbx_data->geo; //get by id 2
		//connecter model->model ou geo->model
	}
	else if (IS_TAG(line, "OP"))
	{
		//avancer line jusqu au 4e arg
		if (IS_TAG(line, "Lcl"))
		{
			fbx_data->anim_node; //get by id 1
			fbx_data->model; //get by id 2
			//connecter anim_node->model
		}
		else if (IS_TAG(line, "d|"))
		{
			fbx_data->anim_curve; //get by id 1
			fbx_data->anim_node; //get by id 2
			//connecter anim_curve->anim_node (x, y ou z)
		}
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
	return (SUCCESS);cursor + 9
}
