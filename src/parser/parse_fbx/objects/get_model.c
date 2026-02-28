/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_model.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:46 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:55:52 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Fonction de parsing qui recupere 'type' donne en argument et
 * les dernieres valeurs 'x', 'y' et 'z' dans 'line'.
 * Exemple de ligne :
 * 'P: "Nom", "Type", "Label", "Flags", x, y, z'
 */
static void	ft_extract_property_values(t_properties *p, char *line, char type)
{
	int	i;

	p->type = type;
	i = -1;
	while (++i < 4 && line)
	{
		line = ft_strchr(line, ',');
		if (line)
			line++;
	}
	if (!line)
		return ;
	p->x = ft_atof(ft_skip_spaces(line));
	line = ft_strchr(line, ',');
	if (line)
		p->y = ft_atof(ft_skip_spaces(++line));
	line = ft_strchr(line, ',');
	if (line)
		p->z = ft_atof(ft_skip_spaces(++line));
}

/**
 * Fonction de parsing pour lire les proprietes d'un model
 * Exemple de format :
 * 	Properties70:  {
 * 		...
 * 	}
 */
static void	ft_parse_properties(t_model *model, int fd)
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
			return (free(line));
		if (IS_TAG(cursor, "P:"))
		{
			if (ft_strnstr(cursor, "\"Lcl Translation\"", 50))
				ft_extract_property_values(&model->pos, cursor, 'T');
			else if (ft_strnstr(cursor, "\"Lcl Rotation\"", 50))
				ft_extract_property_values(&model->rot, cursor, 'R');
			else if (ft_strnstr(cursor, "\"Lcl Scaling\"", 50))
				ft_extract_property_values(&model->scale, cursor, 'S');
		}
		free(line);
	}
}

static void	ft_parse_model_lines(t_model *model, int fd)
{
	char	*line;
	char	*cursor;

	while (1)
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
		if (IS_TAG(cursor, "Properties70"))
			ft_parse_properties(model, fd);
		free(line);
	}
}

/**
 *	Model: 200, "Model::MonCube", "Mesh" {
 *		Properties70:  {
 *			P: "Lcl Translation", "Lcl Translation", "", "A", 0, 10, 0
 *		}
 *	}
 */
t_model	*ft_get_model(char *cursor, int fd)
{
	t_model	*model;

	model = ft_calloc(1, sizeof(t_model));
	if (!model)
		return (NULL);
	model->id = ft_atol(cursor);
	ft_parse_model_lines(model, fd);
	model->base_pos = model->pos;
	model->base_rot = model->rot;
	model->base_scale = model->scale;
	return (model);
}
