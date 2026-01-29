/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_geo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:41 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/29 07:31:34 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static double	ft_extract_val(char **cursor)
{
	double	ret;

	if (!*cursor)
		return (0);
	*cursor = ft_strchr(*cursor, ',');
	if (!*cursor)
		return (0);
	(*cursor)++;
	ret = ft_atof(*cursor);
	return (ret);
}

void	ft_skip_closing_brace(char **cursor, char **line, int fd)
{
	while (TRUE)
	{
		if (*cursor && ft_strchr(*cursor, '}'))
			break ;
		if (*line)
			free(*line);
		*line = get_next_line(fd);
		if (!*line)
			break ;
		*cursor = *line;
	}
}

static int	ft_parse_vertex(t_object *obj, char *cursor, int fd)
{
	int		index;
	char	*line;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (ERROR);
	cursor++;
	obj->nb_vertices = ft_atoi(cursor);
	obj->vertices = malloc(sizeof(t_vec3) * obj->nb_vertices);
	if (!obj->vertices)
		return (ERROR);
	cursor = ft_strchr(cursor, '{') + 1;
	index = 0;
	line = NULL;
	while (index < obj->nb_vertices)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, &line, fd))
			break ;
		ft_skip_to_content(&cursor);
		if (*cursor == ',')
			cursor++;
		obj->vertices[index].x = ft_atof(cursor);
		obj->vertices[index].y = ft_extract_val(&cursor);
		obj->vertices[index].z = ft_extract_val(&cursor);
		obj->vertices[index].color = WHITE;
		if (cursor)
			cursor = ft_strchr(cursor, ',');
		index++;
	}
	ft_skip_closing_brace(&cursor, &line, fd);
	if (line)
		free(line);
	return (SUCCESS);
}

t_object	*ft_get_obj(t_object *obj, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (*cursor == '}')
		{
			free(line);
			break ;
		}
		if (IS_TAG(cursor, "Vertices"))
			ft_parse_vertex(obj, cursor, fd);
		else if (ft_strnstr(cursor, "PolygonVertexIndex", 20))
			ft_parse_face(obj, cursor, fd);
		free(line);
	}
	obj->height = 0;
	obj->width = 0;
	return (obj);
}

/**
 *	Geometry: 100, "Geometry::MonCube", "Mesh" {
 *		Vertices: ... { ... }
 *		PolygonVertexIndex: ... { ... }
 *	}
 */
t_geometry	*ft_get_geometry(char *cursor, int fd)
{
	t_geometry	*geo;
	t_object	*obj;

	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
		return (NULL);
	geo = malloc(sizeof(t_geometry));
	if (!geo)
		return (ft_free_object(obj));
	geo->id = ft_atoi(cursor);
	obj = ft_get_obj(obj, fd);
	if (!obj)
		return (ft_free_geo(geo));
	geo->obj = obj;
	return (geo);
}
