/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_curve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:39 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/06 22:15:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 *	KeyTime: *3 { 0, 153953860000, 307907720000 }
 */
static int	ft_parse_keytime(t_anim_curve *anim_curve, char **line, char *cursor, int fd)
{
	int			index;
	long long	ktime;

	printf("Parsing KeyTime\n"); // debug
	if (!anim_curve->n_keys)
	{
		cursor = ft_strchr(cursor, '*');
		if (!cursor)
			return (ERROR);
		cursor++;
		anim_curve->n_keys = ft_atoi(cursor);
	}
	printf("- n keys : %d\n", anim_curve->n_keys); // debug
	anim_curve->time = malloc(sizeof(double) * anim_curve->n_keys);
	if (!anim_curve->time)
		return (ERROR);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	index = 0;
	while (index < anim_curve->n_keys)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		ktime = ft_atoll(cursor);
		anim_curve->time[index] = (double)(ktime / FBX_SEC);
		printf("- KeyTime [%d] : %f\n", index, anim_curve->time[index]);
		while (*cursor && (ft_isdigit(*cursor) || *cursor == '-'))
			cursor++;
		index++;
	}
	ft_skip_closing_brace(&cursor, line, fd);
	return (SUCCESS);
}

/**
 *	KeyValueFloat: *3 { 0.0, 5.0, 10.0 }
 */
static int	ft_parse_keyvalue(t_anim_curve *anim_curve, char **line, char *cursor, int fd)
{
	int		index;

	printf ("Parsing KeyValueFloat\n"); // debug
	if (!anim_curve->n_keys)
	{
		cursor = ft_strchr(cursor, '*');
		if (!cursor)
			return (ERROR);
		cursor++;
		anim_curve->n_keys = ft_atoi(cursor);
	}
	printf ("- n keys : %d\n", anim_curve->n_keys); // debug
	anim_curve->value = malloc(sizeof(double) * anim_curve->n_keys);
	if (!anim_curve->value)
		return (ERROR);
	cursor = ft_strchr(cursor, '{');
	if (cursor)
		cursor++;
	index = 0;
	while (index < anim_curve->n_keys)
	{
		ft_jump_to_next_value(&cursor, line, fd);
		if (*cursor == '}')
			break ;
		anim_curve->value[index] = ft_atof(cursor);
		printf(" - KeyValueFloat [%d] : %f\n", index, anim_curve->value[index]);
		if (*cursor == '-' || *cursor == '+')
			cursor++;
		while (ft_isdigit(*cursor))
			cursor++;
		if (*cursor == '.')
		{
			cursor++;
			while (ft_isdigit(*cursor))
				cursor++;
		}
		if (*cursor == 'e' || *cursor == 'E')
		{
			cursor++;
			if (*cursor == '-' || *cursor == '+')
				cursor++;
			while (ft_isdigit(*cursor))
				cursor++;
		}
		index++;
	}
	ft_skip_closing_brace(&cursor, line, fd);
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

	anim_curve = ft_calloc(1, sizeof(t_anim_curve));
	if (!anim_curve)
		return (NULL);
	anim_curve->id = ft_atol(cursor);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		// printf("DEBUG - Line read: [%s]\n", line); // debug
		cursor = ft_skip_spaces(line);
		if (*cursor == '}')
		{
			free(line);
			break ;
		}
		if (IS_TAG(cursor, "KeyTime"))
			ft_parse_keytime(anim_curve, &line, cursor, fd);
		else if (IS_TAG(cursor, "KeyValueFloat"))
			ft_parse_keyvalue(anim_curve, &line, cursor, fd);
		free(line);
	}
	return (anim_curve);
}
