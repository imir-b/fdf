/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_deformer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 20:59:15 by vlad              #+#    #+#             */
/*   Updated: 2026/02/27 11:38:44 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Helper pour avancer le curseur après avoir lu un nombre
static void	ft_skip_number(char **cursor)
{
	while (**cursor && (ft_isdigit(**cursor) || **cursor == '-'
			|| **cursor == '.' || **cursor == 'e' || **cursor == 'E'))
		(*cursor)++;
}

/**
 * Indexes: *26 { a: 1480, 1481, ... }
 */
int	*ft_parse_verticies(char **line, char *cursor, int fd, int *out_count)
{
	int	*indexes;
	int	count;
	int	i;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (NULL);
	count = ft_atoi(++cursor);
	indexes = malloc(sizeof(int) * count);
	if (!indexes)
		return (NULL);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	i = 0;
	while (i < count)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		indexes[i] = ft_atoi(cursor);
		ft_skip_number(&cursor);
		i++;
	}
	ft_skip_closing_brace(&cursor, line, fd);
	if (out_count)
		*out_count = count;
	return (indexes);
}

/**
 * Weights: *26 { a: 0.047, 0.098, ... }
 */
double	*ft_parse_weights(char **line, char *cursor, int fd)
{
	double	*weights;
	int		count;
	int		i;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (NULL);
	count = ft_atoi(++cursor);
	weights = malloc(sizeof(double) * count);
	if (!weights)
		return (NULL);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	i = 0;
	while (i < count)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (!*cursor || *cursor == '}')
			break ;
		weights[i] = ft_atof(cursor);
		ft_skip_number(&cursor);
		i++;
	}
	ft_skip_closing_brace(&cursor, line, fd);
	return (weights);
}

/**
 * TransformLink: *16 { a: 0.385, ... }
 * Matrice 4x4 (Inverse Bind Matrix potentielle)
 */
/**
 * Transform: *16 { a: 0.385, ... }
 * Matrice 4x4
 */
t_mat4	*ft_parse_transform(char **line, char *cursor, int fd)
{
	t_mat4 	*matrix;
	int		count;
	int		i;

	cursor = ft_strchr(cursor, '*');
	if (!cursor)
		return (NULL);
	count = ft_atoi(++cursor);
	matrix = malloc(sizeof(t_mat4));
	if (!matrix)
		return (NULL);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	i = 0;
	while (i < count && i < 16)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		matrix->m[i / 4][i % 4] = ft_atof(cursor);
		ft_skip_number(&cursor);
		i++;
	}
	return (matrix);
}

/**
 * 	Deformer: 1000082, "SubDeformer::", "Cluster" {
 *		Version: 100
 *		UserData: "", ""
 *		Indexes: *26 {
 *			a: 1480,1481,1482,...
 *		} 
 *		Weights: *26 {
 *			a: 0.0470588244497776,0.0980392172932625,0.705882370471954,...
 *		} 
 *		Transform: *16 {
 *			a: 0.385069936513901,0.830801069736481,-0.401859313249588,...
 *		} 
 *		TransformLink: *16 {
 *			a: 0.385069906711578,-0.127643629908562,-0.914017617702484,...
 *	}
 */
t_deformer	*ft_get_deformer(char *cursor, int fd)
{
	t_deformer	*deformer;
	char		*line;

	deformer = ft_calloc(1, sizeof(t_deformer));
	if (!deformer)
		return (NULL);
	deformer->id = atol(cursor);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (*cursor == '}')
			return (free(line), deformer);
		if (IS_TAG(cursor, "Indexes"))
			deformer->verticies = ft_parse_verticies(&line, cursor, fd,
					&deformer->n_vertices);
		else if (IS_TAG(cursor, "Weights"))
			deformer->weights = ft_parse_weights(&line, cursor, fd);
		else if (IS_TAG(cursor, "TransformLink"))
			deformer->t_link = ft_parse_transform(&line, cursor, fd);
		else if (IS_TAG(cursor, "Transform"))
			deformer->transform = ft_parse_transform(&line, cursor, fd);
		free(line);
	}
	return (deformer);
}
