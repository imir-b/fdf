/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/25 17:14:14 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_object	*ft_parse_fbx(const char *filename, t_object *obj)
{
	int		fd;
	char	*line;
	t_fbx	*fbx_data;

	fbx_data = malloc(sizeof(t_fbx));
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_free_object(obj));
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		else if (IS_TAG(line, "Objects:"))
			ft_parse_objects(fbx_data, obj, fd);
		else if (IS_TAG(line, "Connections:"))
			ft_parse_connections(fbx_data, fd);
		free(line);
	}
	return (obj);
}
