/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_geo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:41 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/29 05:36:11 by vbleskin         ###   ########.fr       */
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
	if (line)
		free(line);
	return (SUCCESS);
}

static int	ft_count_faces(int *raw, int total)
{
	int	index;
	int	ret;

	index = 0;
	ret = 0;
	while (index < total)
	{
		if (raw[index] < 0)
			ret++;
		index++;
	}
	return (ret);
}

static int	ft_extract_one_face(t_face *face, int *raw, int start, int end)
{
	int	f_id;

	face->count = (end - start) + 1;
	face->indices = malloc(sizeof(int) * face->count);
	if (!face->indices)
		return (ERROR);
	f_id = 0;
	while (start <= end)
	{
		if (raw[start] < 0)
			face->indices[f_id] = ~raw[start];
		else
			face->indices[f_id] = raw[start];
		start++;
		f_id++;
	}
	return (SUCCESS);
}

static t_face	*ft_extract_faces(int *raw, int total, \
										t_object *obj)
{
	t_face	*faces;
	int		index;
	int		start;
	int		f_id;

	obj->nb_faces = ft_count_faces(raw, total);
	faces = malloc(sizeof(t_face) * obj->nb_faces);
	if (!faces)
		return (NULL);
	index = 0;
	f_id = 0;
	start = 0;
	while (index < total)
	{
		if (raw[index] < 0)
		{
			if (ft_extract_one_face(&faces[f_id], raw, start, index))
				return (NULL);
			f_id++;
			start = index + 1;
		}
		index++;
	}
	return (faces);
}

static int	ft_parse_face(t_object *obj, char *cursor, int fd)
{
	int		index;
	int		nb_indices;
	int		*raw_indices;
	char	*line;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (ERROR);
	cursor++;
	nb_indices = ft_atoi(cursor);
	raw_indices = malloc(sizeof(int) * nb_indices);
	if (!raw_indices)
		return (ERROR);
	index = 0;
	line = NULL;
	while (index < nb_indices)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, &line, fd))
			break ;
		ft_skip_to_content(&cursor);
		if (!*cursor)
			continue ;
		raw_indices[index] = ft_atoi(cursor);
		index++;
		cursor = ft_strchr(cursor, ',');
		ft_move_cursor(&cursor);
	}
	obj->faces = ft_extract_faces(raw_indices, nb_indices, obj);
	return (free(raw_indices), SUCCESS);
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
		if (IS_TAG(cursor, "Vertices:"))
			ft_parse_vertex(obj, cursor, fd);
		else if (IS_TAG(cursor, "PolygonVertexIndex:"))
		{
			if (ft_parse_face(obj, cursor, fd))
				printf("failed parsing face\n");
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
