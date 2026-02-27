/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 07:12:12 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:40:04 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

static t_face	*ft_extract_faces(int *raw, int total,
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

int	ft_parse_face(t_object *obj, char **line, char *cursor, int fd)
{
	int		index;
	int		nb_indices;
	int		*raw_indices;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (ERROR);
	cursor++;
	nb_indices = ft_atoi(cursor);
	raw_indices = malloc(sizeof(int) * nb_indices);
	if (!raw_indices)
		return (ERROR);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	index = 0;
	while (index < nb_indices)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		raw_indices[index] = ft_atoi(cursor);
		if (*cursor == '-')
			cursor++;
		while (*cursor && ft_isdigit(*cursor) && *cursor != '-')
			cursor++;
		index++;
	}
	obj->faces = ft_extract_faces(raw_indices, nb_indices, obj);
	ft_skip_closing_brace(&cursor, line, fd);
	free(raw_indices);
	if (!obj->faces)
		return (ERROR);
	return (SUCCESS);
}
