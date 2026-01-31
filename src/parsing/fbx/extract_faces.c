/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_faces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 07:12:12 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/30 22:05:59 by vlad             ###   ########.fr       */
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

int	ft_parse_face(t_object *obj, char *cursor, int fd)
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
	printf("DEBUG: nb_indices lu = %d\n", nb_indices); //debug
	raw_indices = malloc(sizeof(int) * nb_indices);
	if (!raw_indices)
		return (ERROR);
	cursor = ft_strchr(cursor, '{');
	index = 0;
	line = NULL;
	while (index < nb_indices)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, &line, fd))
			break ;
		ft_skip_to_content(&cursor);
		while (*cursor && !ft_isdigit(*cursor) && *cursor != '-')
            cursor++;
		if (!*cursor)
			continue ;
		raw_indices[index] = ft_atoi(cursor);
		index++;
		cursor = ft_strchr(cursor, ',');
		ft_move_cursor(&cursor);
	}
	obj->faces = ft_extract_faces(raw_indices, nb_indices, obj);
	ft_skip_closing_brace(&cursor, &line, fd);
	if (!obj->faces)
		return (free(raw_indices), ERROR);
	return (free(raw_indices), SUCCESS);
}
