/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_geo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:41 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/31 21:07:39 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_read_value(char **cursor)
{
	while (	**cursor && (ft_isdigit(**cursor) || **cursor == '-' || \
			**cursor == '.' || **cursor == 'e' || **cursor == 'E'))
	{
		(*cursor)++;
	}
}

static char	*ft_parse_vertex(t_object *obj, char **line, char *cursor, int fd)
{
	int		index;
	int 	nb_floats;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (NULL);
	cursor++;
	nb_floats = ft_atoi(cursor);
	obj->nb_vertices = nb_floats / 3;
	obj->vertices = malloc(sizeof(t_vec3) * obj->nb_vertices);
	if (!obj->vertices)
		return (NULL);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	index = 0;
	while (index < obj->nb_vertices)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		obj->vertices[index].x = ft_atof(cursor);
		ft_read_value(&cursor);
		ft_jump_to_next_value(&cursor, line, fd);
		obj->vertices[index].y = ft_atof(cursor);
		ft_read_value(&cursor);
		ft_jump_to_next_value(&cursor, line, fd);
		obj->vertices[index].z = ft_atof(cursor);
		ft_read_value(&cursor);
		obj->vertices[index].color = WHITE;
		index++;
	}
	ft_skip_closing_brace(&cursor, line, fd);
	return (*line);
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
			ft_parse_vertex(obj, &line, cursor, fd);
		else if (IS_TAG(cursor, "PolygonVertexIndex"))
			ft_parse_face(obj, &line, cursor, fd);
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
