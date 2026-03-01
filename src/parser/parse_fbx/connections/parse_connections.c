/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:14:16 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:19:00 by vlad             ###   ########.fr       */
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

static void	ft_extract_connection(t_fbx *fbx_data, char *line)
{
	long	ids[2];

	line = ft_strchr(line, '\"');
	if (*line)
		line++;
	ft_read_ids(line, ids);
	if (ft_is_tag(line, "OO"))
		ft_connect_obj_to_obj(fbx_data, ids);
	else if (ft_is_tag(line, "OP"))
	{
		line = ft_strrchr(line, ',');
		while (*line && (*line == ' ' || *line == '\"' || *line == ','))
			line++;
		if (ft_is_tag(line, "Lcl"))
			ft_connect_anim_to_model(fbx_data, line, ids);
		else if (ft_is_tag(line, "d|"))
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
		if (ft_is_tag(cursor, "C:"))
			ft_extract_connection(fbx_data, cursor);
		free(line);
	}
	return (SUCCESS);
}
