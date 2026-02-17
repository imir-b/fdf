/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:08:44 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/06 20:38:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static char	ft_extract_type(char *line)
{
	char	*quote;

	quote = ft_strchr(line, '"');
	if (!quote)
		return (0);
	quote++;
	if (*quote == 'T' || *quote == 'R' || *quote == 'S')
	{
		if (*(quote + 1) == '"' || *(quote + 1) == '\0')
			return (*quote);
		if (ft_strnstr(line, "::T\"", ft_strlen(line)))
			return ('T');
		if (ft_strnstr(line, "::R\"", ft_strlen(line)))
			return ('R');
		if (ft_strnstr(line, "::S\"", ft_strlen(line)))
			return ('S');
		return (*quote);
	}
	return (0);
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
