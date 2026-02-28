/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 13:50:13 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:52:06 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * On regarde s'il y a une virgule apres la valeur dans le .fdf,
 * si oui on recupere la couleur en hexa, sinon on dessine en blanc.
 */
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

static int	ft_parse_row(t_object *obj, char **split, int y)
{
	int		x;
	int		idx;
	int		f_idx;

	x = 0;
	while (x < obj->width)
	{
		idx = (y * obj->width) + x;
		obj->vertices[idx] = ft_get_verticies(x, y, split[x]);
		if (x < obj->width - 1 && y < obj->height - 1)
		{
			f_idx = (y * (obj->width - 1)) + x;
			if (ft_get_face(&obj->faces[f_idx], idx, obj->width))
				return (1);
		}
		x++;
	}
	return (0);
}

/**
 * Deuxieme lecture pour remplir les tableaux de donnees.
 */
int	ft_fill_fdf_data(t_object *obj, int fd)
{
	char	*line;
	char	**split;
	int		y;

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
		if (ft_parse_row(obj, split, y))
			return (ft_free_tab(split), ERROR);
		ft_free_tab(split);
		y++;
	}
	return (SUCCESS);
}
