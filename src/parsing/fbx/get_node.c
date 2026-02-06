/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:44 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 17:42:12 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static char	ft_extract_type(char *line)
{
	char	*cursor;

	cursor = ft_strrchr(line, ',');
	if (!cursor)
		return (0);
	cursor = ft_strchr(cursor, '\"');
	if (!cursor)
		return (0);
	return (cursor[1]);
}

/**
 *	AnimationCurveNode: 400, "Node::T", "T" {
 *		Properties70: {
 *			P: ...
 *		}
 *	}
 */
t_anim_node	*ft_get_anim_node(char *cursor, int fd)
{
	t_anim_node	*anim_node;
	char		*line;

	anim_node = ft_calloc(1, sizeof(t_anim_node));
	if (!anim_node)
		return (NULL);
	anim_node->id = ft_atol(cursor);
	anim_node->type = ft_extract_type(cursor);
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
		free(line);
	}
	return (anim_node);
}
