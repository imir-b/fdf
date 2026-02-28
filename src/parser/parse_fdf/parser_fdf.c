/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fdf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 20:37:36 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:51:26 by vbleskin         ###   ########.fr       */
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

t_object	*ft_parse_fdf(const char *filename, t_object *obj)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_object(obj));
	if (ft_get_dimensions(obj, fd))
		return (close(fd), ft_free_object(obj));
	close(fd);
	obj->nb_vertices = obj->width * obj->height;
	obj->nb_faces = (obj->width - 1) * (obj->height - 1);
	obj->vertices = malloc(sizeof(t_vec3) * obj->nb_vertices);
	obj->faces = malloc(sizeof(t_face) * obj->nb_faces);
	if (!obj->vertices || !obj->faces)
		return (ft_free_object(obj));
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_object(obj));
	if (ft_fill_fdf_data(obj, fd))
		return (close (fd), ft_free_object(obj));
	close(fd);
	return (obj);
}
