/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_stack.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:28:35 by vlad              #+#    #+#             */
/*   Updated: 2026/02/27 14:59:35 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static char	*ft_extract_name(char *cursor)
{
	char	*name;
	int		i;

	i = 0;
	while (cursor[i] && cursor[i] != '\"')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		return (NULL);
	i = 0;
	while (*cursor && *cursor != '\"')
	{
		name[i] = *cursor;
		i++;
		cursor++;
	}
	name[i] = '\0';
	return (name);
}

/*
** Extrait et assigne le nom de l'animation en cherchant
** le délimiteur approprié (deux-points ou guillemets).
*/
static void	ft_set_anim_name(t_anim_stack *anim, char *cursor)
{
	char	*colon;

	colon = ft_strrchr(cursor, ':');
	if (colon && *colon)
		cursor = colon + 1;
	else
	{
		cursor = ft_strchr(cursor, '"');
		if (cursor)
			cursor++;
	}
	if (cursor)
		anim->name = ft_extract_name(cursor);
	else
		anim->name = ft_strdup("Unknown");
}

/**
 *	AnimationStack: id, "AnimStack::AnimationName", "" {
 *		Properties70:  {
 *			P: "LocalStart", "KTime", "Time", "", 0
 *			P: "LocalStop", "KTime", "Time", "", 461861580000
 *			P: "ReferenceStart", "KTime", "Time", "", 0
 *			P: "ReferenceStop", "KTime", "Time", "", 461861580000
 *		}
 *	}
 */
t_anim_stack	*ft_get_anim_stack(char *cursor, int fd)
{
	t_anim_stack	*anim;
	char			*line;

	anim = ft_calloc(1, sizeof(t_anim_stack));
	if (!anim)
		return (NULL);
	anim->id = ft_atol(cursor);
	ft_set_anim_name(anim, cursor);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (*ft_skip_spaces(line) == '}')
			return (free(line), anim);
		free(line);
	}
	return (anim);
}

/**
 *	AnimationLayer: id, "AnimLayer::", "" {
 *	}
 */
t_anim_layer	*ft_get_anim_layer(char *cursor, int fd)
{
	t_anim_layer	*anim_layer;
	char			*line;

	anim_layer = ft_calloc(1, sizeof(t_anim_layer));
	if (!anim_layer)
		return (NULL);
	anim_layer->id = ft_atol(cursor);
	if (!ft_strchr(cursor, '{'))
		return (anim_layer);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (*cursor == '}')
			return (free(line), anim_layer);
		free(line);
	}
	return (anim_layer);
}
