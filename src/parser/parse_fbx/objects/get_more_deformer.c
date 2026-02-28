/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_deformer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:23:57 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 21:25:38 by vbleskin         ###   ########.fr       */
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
	i = -1;
	while (++i < count)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (!*cursor || *cursor == '}')
			break ;
		weights[i] = ft_atof(cursor);
		ft_skip_number(&cursor);
	}
	ft_skip_closing_brace(&cursor, line, fd);
	return (weights);
}

/**
 * TransformLink: *16 { a: 0.385, ... }
 * Matrice 4x4 (Inverse Bind Matrix potentielle)
 * ou :
 * Transform: *16 { a: 0.385, ... }
 * Matrice 4x4
 */
t_mat4	*ft_parse_transform(char **line, char *cursor, int fd)
{
	t_mat4	*matrix;
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

static int	*ft_init_verticies(char **cursor, int *count)
{
	int	*idx;

	*cursor = ft_strchr(*cursor, '*');
	if (!*cursor)
		return (NULL);
	(*cursor)++;
	*count = ft_atoi(*cursor);
	idx = malloc(sizeof(int) * (*count));
	if (!idx)
		return (NULL);
	*cursor = ft_strchr(*cursor, '{');
	if (*cursor)
		(*cursor)++;
	return (idx);
}

/**
 * Indexes: *26 { a: 1480, 1481, ... }
 */
int	*ft_parse_verticies(char **line, char *cursor, int fd, int *out_count)
{
	int	*indexes;
	int	count;
	int	i;

	indexes = ft_init_verticies(&cursor, &count);
	if (!indexes)
		return (NULL);
	i = -1;
	while (++i < count)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		indexes[i] = ft_atoi(cursor);
		ft_skip_number(&cursor);
	}
	ft_skip_closing_brace(&cursor, line, fd);
	if (out_count)
		*out_count = count;
	return (indexes);
}
