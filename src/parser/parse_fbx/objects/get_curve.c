/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_curve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:39 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/28 22:26:16 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_init_curve_arr(t_anim_curve *c, char **cur, int is_val)
{
	if (!c->n_keys)
	{
		*cur = ft_strchr(*cur, '*');
		if (!*cur)
			return (ERROR);
		c->n_keys = ft_atoi(++(*cur));
	}
	if (is_val)
		c->value = malloc(sizeof(double) * c->n_keys);
	else
		c->time = malloc(sizeof(double) * c->n_keys);
	if ((is_val && !c->value) || (!is_val && !c->time))
		return (ERROR);
	*cur = ft_strchr(*cur, '{');
	if (*cur)
		(*cur)++;
	return (SUCCESS);
}

/**
 *	KeyTime: *3 { 0, 153953860000, 307907720000 }
 */
static int	ft_parse_keytime(t_anim_curve *c, char **line, char *cur, int fd)
{
	int			i;
	long long	ktime;

	if (ft_init_curve_arr(c, &cur, 0) == ERROR)
		return (ERROR);
	i = -1;
	while (++i < c->n_keys)
	{
		ft_jump_to_next_value(&cur, line, fd);
		if (*cur == '}')
			break ;
		ktime = ft_atoll(cur);
		c->time[i] = (double)(ktime / FBX_SEC);
		while (*cur && (ft_isdigit(*cur) || *cur == '-'))
			cur++;
	}
	ft_skip_closing_brace(&cur, line, fd);
	ft_redistribute_keytimes(c);
	return (SUCCESS);
}

static int	ft_jump_to_float_val(char **cur, char **line, int fd)
{
	while (1)
	{
		if (!*cur || !**cur || **cur == '\n' || **cur == '\r')
		{
			free(*line);
			*line = get_next_line(fd);
			if (!*line)
				return (ERROR);
			*cur = *line;
		}
		else if (**cur == ' ' || **cur == '\t' || **cur == ','
			|| **cur == 'a' || **cur == ':')
			(*cur)++;
		else
			break ;
	}
	return (SUCCESS);
}

/**
 *	KeyValueFloat: *3 { 0.0, 5.0, 10.0 }
 */
static int	ft_parse_keyvalue(t_anim_curve *c, char **line, char *cur, int fd)
{
	int	i;

	if (ft_init_curve_arr(c, &cur, 1) == ERROR)
		return (ERROR);
	i = -1;
	while (++i < c->n_keys)
	{
		if (ft_jump_to_float_val(&cur, line, fd) == ERROR)
			return (ERROR);
		if (*cur == '}')
			break ;
		c->value[i] = ft_atof(cur);
		while (*cur && (ft_isdigit(*cur) || *cur == '-' || *cur == '+'
				|| *cur == '.' || *cur == 'e' || *cur == 'E'))
			cur++;
	}
	ft_skip_closing_brace(&cur, line, fd);
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
	t_anim_curve	*curve;
	char			*line;

	curve = ft_calloc(1, sizeof(t_anim_curve));
	if (!curve)
		return (NULL);
	curve->id = ft_atol(cursor);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (*cursor == '}')
			return (free(line), curve);
		if (IS_TAG(cursor, "KeyTime"))
			ft_parse_keytime(curve, &line, cursor, fd);
		else if (IS_TAG(cursor, "KeyValueFloat"))
			ft_parse_keyvalue(curve, &line, cursor, fd);
		free(line);
	}
	return (curve);
}
