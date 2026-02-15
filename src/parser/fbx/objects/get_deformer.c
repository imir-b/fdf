/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_deformer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 20:59:15 by vlad              #+#    #+#             */
/*   Updated: 2026/02/15 15:59:45 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Helper pour avancer le curseur après avoir lu un nombre
static void ft_skip_number(char **cursor)
{
    while (**cursor && (ft_isdigit(**cursor) || **cursor == '-' || 
           **cursor == '.' || **cursor == 'e' || **cursor == 'E'))
        (*cursor)++;
}

/**
 * Indexes: *26 { a: 1480, 1481, ... }
 */
int *ft_parse_verticies(char **line, char *cursor, int fd)
{
    int     *indexes;
    int     count;
    int     i;

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
            break;
        indexes[i] = ft_atoi(cursor);
        ft_skip_number(&cursor);
        i++;
    }
    return (indexes);
}

/**
 * Weights: *26 { a: 0.047, 0.098, ... }
 */
double *ft_parse_weights(char **line, char *cursor, int fd)
{
    double  *weights;
    int     count;
    int     i;

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
        if (!*cursor)
            break;
        if (*cursor == '}')
            break;
    }
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
double *ft_parse_transform(char **line, char *cursor, int fd)
{
    double  *matrix;
    int     count;
    int     i;

    cursor = ft_strchr(cursor, '*');
    if (!cursor)
        return (NULL);
    count = ft_atoi(++cursor);
    matrix = malloc(sizeof(double) * count);
    if (!matrix)
        return (NULL);
    cursor = ft_strchr(cursor, '{');
    if (cursor)
        cursor++;
    i = 0;
    while (i < count)
    {
        ft_jump_to_next_value(&cursor, line, fd);
        if (*cursor == '}')
            break;
        matrix[i] = ft_atof(cursor);
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
 *			a: 1480,1481,1482,1487,1486,1500,1499,1498,1501,1502,1503,1504,1505,1506,1507,1510,1511,1521,1522,1529,1530,1534,1535,1536,1512,1520
 *		} 
 *		Weights: *26 {
 *			a: 0.0470588244497776,0.0980392172932625,0.705882370471954,0.803921580314636,0.0980392172932625,0.0980392172932625,0.454901963472366,0.352941185235977,0.0980392172932625,0.501960813999176,0.47843137383461,0.866666674613953,0.858823537826538,0.752941191196442,0.0980392172932625,0.0980392098426819,0.905882358551025,0.0823529437184334,0.0941176488995552,0.0823529437184334,0.352941185235977,0.0823529437184334,0.352941185235977,0.149019613862038,0.352941185235977,0.0823529437184334
 *		} 
 *		Transform: *16 {
 *			a: 0.385069936513901,0.830801069736481,-0.401859313249588,0,-0.127643644809723,-0.383307605981827,-0.914758145809174,0,-0.914017736911774,0.403540670871735,-0.0415537767112255,0,16.2794151306152,-10.8420238494873,-1.81345224380493,1
 *		} 
 *		TransformLink: *16 {
 *			a: 0.385069906711578,-0.127643629908562,-0.914017617702484,0,0.830800950527191,-0.383307576179504,0.403540581464767,0,-0.401859253644943,-0.914758145809174,-0.0415537394583225,0,2.01009845733643,-3.73673605918884,19.1795120239258,1
 *		} 
 *	}
 */
t_deformer	*ft_get_deformer(char *cursor, int fd)
{
	t_deformer	*deformer;
	char		*line;
	int			n_verticies;

	deformer = malloc(sizeof(t_deformer));
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
			deformer->verticies = ft_parse_verticies(&line, cursor, fd);
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
