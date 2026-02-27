/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_obj.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:13:46 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/29 17:21:10 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Premiere lecture pour recuperer le nombre de sommets et de faces.
 */
static int	ft_get_count_elems(t_object *obj, const char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ERROR);
	obj->nb_vertices = 0;
	obj->nb_faces = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == 'v' && line[1] == ' ')
			obj->nb_vertices++;
		else if (line[0] == 'f' && line[1] == ' ')
			obj->nb_faces++;
		free(line);
	}
	close(fd);
	return (SUCCESS);
}

static void	ft_get_vertex(t_object *obj, char *line, int index)
{
	char	**split;

	split = ft_split(line, ' ');
	if (split)
	{
		obj->vertices[index].x = ft_atof(split[1]);
		obj->vertices[index].y = ft_atof(split[2]);
		obj->vertices[index].z = ft_atof(split[3]);
		obj->vertices[index].color = WHITE;
		obj->vertices[index].sx = 0;
		obj->vertices[index].sy = 0;
		ft_free_tab(split);
	}
}

static int	ft_get_face(t_object *obj, char *line, int index)
{
	char	**split;
	int		count;
	int		i;

	split = ft_split(line, ' ');
	if (!split)
		return (ERROR);
	count = 0;
	while (split[count])
		count++;
	count--;
	obj->faces[index].count = count;
	if (count < 3)
		return (ERROR);
	obj->faces[index].indices = malloc(sizeof(int) * count);
	if (!obj->faces[index].indices)
		return (ft_free_tab(split), ERROR);
	i = 0;
	while (i < count)
	{
		obj->faces[index].indices[i] = ft_atoi(split[i + 1]) - 1;
		i++;
	}
	ft_free_tab(split);
	return (SUCCESS);
}

static int	ft_fill_obj_data(t_object *obj, const char *filename)
{
	int		fd;
	char	*line;
	int		v_idx;
	int		f_idx;

	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ERROR);
	v_idx = 0;
	f_idx = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == 'v' && line[1] == ' ')
			ft_get_vertex(obj, line, v_idx++);
		else if (line[0] == 'f' && line[1] == ' ')
		{
			if (ft_get_face(obj, line, f_idx++))
				return (free(line), close(fd), ERROR);
		}
		free(line);
	}
	return (close(fd), SUCCESS);
}

t_object	*ft_parse_obj(const char *filename, t_object *obj)
{
	if (ft_get_count_elems(obj, filename))
		return (ft_free_object(obj), NULL);
	obj->vertices = malloc(sizeof(t_vec3) * obj->nb_vertices);
	obj->faces = malloc(sizeof(t_face) * obj->nb_faces);
	if (!obj->vertices || !obj->faces)
		return (ft_free_object(obj), NULL);
	if (ft_fill_obj_data(obj, filename))
		return (ft_free_object(obj), NULL);
	obj->height = 0;
	obj->width = 0;
	return (obj);
}
