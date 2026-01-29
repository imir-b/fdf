/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_curve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:39 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/29 07:06:48 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 *	KeyTime: *3 { 0, 153953860000, 307907720000 }
 */
static int	ft_parse_keytime(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	cursor = ft_strchr(cursor, '*') + 1;
	anim_curve->n_keys = ft_atoi(cursor);
	anim_curve->time = malloc(sizeof(long long) * anim_curve->n_keys);
	if (!anim_curve->time)
		return (ERROR);
	cursor = ft_strchr(cursor, '{');
	if (!cursor)
		return (ERROR);
	cursor++;
	line = NULL;
	index = 0;
	while (index < anim_curve->n_keys)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, &line, fd))
			break ;
		ft_skip_to_content(&cursor);
		if (*cursor == ',')
			cursor++;
		anim_curve->time[index++] = ft_atoll(cursor);
		cursor = ft_strchr(cursor, ',');
		ft_move_cursor(&cursor);
	}
	if (line)
		free(line);
	return (SUCCESS);
}

/**
 *	KeyValueFloat: *3 { 0.0, 5.0, 10.0 }
 */
static int	ft_parse_keyvalue(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	anim_curve->value = malloc(sizeof(double) * anim_curve->n_keys);
	if (!anim_curve->value)
		return (ERROR);
	index = 0;
	cursor = ft_strchr(cursor, '{');
	if (!cursor)
		return (ERROR);
	cursor++;
	while (index < anim_curve->n_keys)
	{
		cursor = ft_skip_spaces(cursor);
		if (ft_extract_line(&cursor, &line, fd))
			break ;
		ft_skip_to_content(&cursor);
		if (*cursor == ',')
			cursor++;
		anim_curve->value[index++] = ft_atof(cursor);
		cursor = ft_strchr(cursor, ',');
		ft_move_cursor(&cursor);
	}
	if (line)
		free(line);
	return (SUCCESS);
}

/**
 *	AnimationCurve: 300, "Curve::Anim_X", "" {
 *		KeyTime: ...
 *		KeyValueFloat: ...
 *	}
 */
t_anim_curve	*ft_get_anim_curve(char *cursor, int fd)
{
	t_anim_curve	*anim_curve;
	char			*line;

	anim_curve = malloc(sizeof(t_anim_curve));
	if (!anim_curve)
		return (NULL);
	anim_curve->id = ft_atoi(cursor);
	while (TRUE)
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
		if (IS_TAG(cursor, "KeyTime"))
			ft_parse_keytime(anim_curve, cursor, fd);
		else if (IS_TAG(cursor, "KeyValueFloat"))
			ft_parse_keyvalue(anim_curve, cursor, fd);
		free(line);
	}
	return (anim_curve);
}
