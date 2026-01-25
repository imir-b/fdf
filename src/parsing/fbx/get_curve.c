/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_curve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:39 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/25 17:10:25 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_parse_keytime(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	cursor = ft_strchr(cursor, '*') + 1;
	anim_curve->n_keys = ft_atoi(cursor);
	anim_curve->time = malloc(sizeof(long long) * anim_curve->n_keys);
	if (!anim_curve->time)
		return (ERROR);
	cursor = ft_strchr(cursor, '{') + 1;
	line = NULL;
	index = 0;
	while (index < anim_curve->n_keys)
	{
		cursor = ft_skip_spaces(cursor);
		line = ft_extract_line(&cursor, fd);
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

static int	ft_parse_keyvalue(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	anim_curve->value = malloc(sizeof(double) * anim_curve->n_keys);
	if (!anim_curve->value)
		return (ERROR);
	index = 0;
	cursor = ft_strchr(cursor, '{') + 1;
	while (index < anim_curve->n_keys)
	{
		cursor = ft_skip_spaces(cursor);
		line = ft_extract_line(&cursor, fd);
		if (*cursor == ',')
			cursor++;
		anim_curve->value[index++] = ft_atof(cursor);
		cursor = ft_strchr(cursor, ',');
		if (!cursor)
			cursor = "";
		else
			cursor++;
	}
	if (line)
		free(line);
	return (SUCCESS);
}

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
		if (IS_TAG(cursor, "KeyTime:"))
			ft_parse_keytime(anim_curve, cursor, fd);
		else if (IS_TAG(cursor, "KeyValueFloat:"))
			ft_parse_keyvalue(anim_curve, cursor, fd);
		free(line);
	}
	return (anim_curve);
}
