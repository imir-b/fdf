/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 07:12:12 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 15:01:53 by vbleskin         ###   ########.fr       */
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

/*
** Initialise le parsing des faces en trouvant le nombre d'indices
** et en allouant le tableau correspondant. Met à jour le curseur.
*/
static int	*ft_init_face_parse(char **cursor, int *nb)
{
	int	*raw;

	*cursor = ft_strchr(*cursor, '*');
	if (!*cursor)
		return (NULL);
	*nb = ft_atoi(++(*cursor));
	raw = malloc(sizeof(int) * (*nb));
	if (!raw)
		return (NULL);
	*cursor = ft_strchr(*cursor, '{');
	if (*cursor)
		(*cursor)++;
	return (raw);
}

int	ft_parse_face(t_object *obj, char **line, char *cursor, int fd)
{
	int	*raw;
	int	nb;
	int	i;

	raw = ft_init_face_parse(&cursor, &nb);
	if (!raw)
		return (ERROR);
	i = -1;
	while (++i < nb)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		raw[i] = ft_atoi(cursor);
		if (*cursor == '-')
			cursor++;
		while (*cursor && ft_isdigit(*cursor) && *cursor != '-')
			cursor++;
	}
	obj->faces = ft_extract_faces(raw, nb, obj);
	ft_skip_closing_brace(&cursor, line, fd);
	if (!obj->faces)
		return (free(raw), ERROR);
	return (free(raw), SUCCESS);
}
