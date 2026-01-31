/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_geo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:41 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/30 21:58:10 by vlad             ###   ########.fr       */
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
	cursor = ft_strchr(cursor, '{') + 1;
	index = 0;
	while (index < obj->nb_vertices)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, line, fd))
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
	printf("DEBUG : line before skip closing brace : %s\n", *line);
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
		{
			printf("DEBUG : line before parsing : %s\n", line);
			ft_parse_vertex(obj, &line, cursor, fd);
			printf("DEBUG : line after parse vertex : %s\n", line);
		}
		else if (IS_TAG(cursor, "PolygonVertexIndex"))
		{
			printf("DEBUG : line before parsing : %s\n", line);
			ft_parse_face(obj, cursor, fd);
			printf("DEBUG : line after parse face : %s\n", line);
		}
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
