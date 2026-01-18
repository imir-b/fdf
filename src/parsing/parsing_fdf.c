/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fdf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 20:37:36 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/18 01:03:02 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Premiere lecture pour recuperer la hauteur et la largeur de la grille.
 */
int	ft_get_dimensions(t_object *obj, int fd)
{
	char	*line;
	int		count;

	obj->height = 0;
	obj->width = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		obj->height++;
		count = ft_count_words_sep(line, ' ');
		if (count == FAIL)
			return (free(line), ERROR);
		if (!obj->width)
			obj->width = count;
		if (obj->width != count)
			return (free(line), ERROR);
		free(line);
	}
	return (SUCCESS);
}

int	ft_extract_color(char *s)
{
	char	*comma;

	comma = ft_strchr(s, ',');
	if (comma)
		return (ft_atoi_hexa(comma + 1));
	return (WHITE);
}

int	ft_get_face(t_face *face, int i, int width)
{
	face->count = 4;
	face->indices = malloc(sizeof(int) * 4);
	if (!face->indices)
		return (ERROR);
	face->indices[0] = i;
	face->indices[1] = i + 1;
	face->indices[2] = i + width + 1;
	face->indices[3] = i + width;
	return (SUCCESS);
}

t_vec3	ft_get_verticies(int x, int y, char *split)
{
	t_vec3	verticies;

	verticies.x = (double)x;
	verticies.y = (double)y;
	verticies.z = (double)ft_atoi(split);
	verticies.color = ft_extract_color(split);
	return (verticies);
}

/**
 * Deuxieme lecture pour remplir les tableaux de donnees.
 */
int	ft_fill_fdf_data(t_object *obj, int fd)
{
	int		index;
	int		f_idx;
	int		y;
	int		x;
	char	*line;
	char	**split;

	y = 0;
	while (y < obj->height)
	{
		line = get_next_line(fd);
		if (!line)
			return (ERROR);
		split = ft_split(line, ' ');
		free(line);
		if (!split)
			return (ERROR);
		x = 0;
		while (x < obj->width)
		{
			index = (y * obj->width) + x;
			obj->vertices[index] = ft_get_verticies(x, y, split[x]);
			if (x < obj->width - 1 && y < obj->height - 1)
			{				
				f_idx = (y * (obj->width - 1)) + x;
				if (ft_get_face(&obj->faces[f_idx], index, obj->width))
					return (ft_free_tab(split), ERROR);
			}
			x++;
		}
		ft_free_tab(split);
		y++;
	}
	return (SUCCESS);
}

t_object	*ft_parse_fdf(const char *filename)
{
	int			fd;
	t_object	*object;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_object(object));
	if (ft_get_dimensions(object, fd))
		return (close(fd), ft_free_object(object));
	close(fd);
	object->nb_vertices = object->width * object->height;
	object->nb_faces = (object->width - 1) * (object->height - 1);
	object->vertices = malloc(sizeof(t_vec3) * object->nb_vertices);
	object->faces = malloc(sizeof(t_face) * object->nb_faces);
	if (!object->vertices || !object->faces)
		return (ft_free_object(object));
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_object(object));
	if (ft_fill_fdf_data(object, fd))
		return (close (fd), ft_free_object(object));
	close(fd);
	return (object);
}
