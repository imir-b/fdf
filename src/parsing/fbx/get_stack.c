/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_stack.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:28:35 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 19:04:21 by vlad             ###   ########.fr       */
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
	t_anim_stack	*anim_stack;

	(void)fd;
	anim_stack = malloc(sizeof(t_anim_stack));
	if (!anim_stack)
		return (NULL);
	anim_stack->id = ft_atoi(cursor);
	cursor = ft_strrchr(cursor, ':');
	if (*cursor)
		cursor++;
	anim_stack->name = ft_extract_name(cursor);
	return (anim_stack);
}

/**
 *	AnimationLayer: id, "AnimLayer::", "" {
 *	}
 */
t_anim_layer	*ft_get_anim_layer(char *cursor, int fd)
{
	t_anim_layer	*anim_layer;

	(void)fd;
	anim_layer = malloc(sizeof(t_anim_layer));
	if (!anim_layer)
		return (NULL);
	anim_layer->id = ft_atoi(cursor);
	return (anim_layer);
}