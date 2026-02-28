/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fbx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:10:52 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:54:37 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Defini 'line_ptr' sur la prochaine ligne si 'cursor' est
 * nul '\0', a la fin de la partie '}' ou en fin de ligne '\n'.
 */
int	ft_extract_line(char **cursor, char **line_ptr, int fd)
{
	if (*cursor && **cursor != '\0' && **cursor != '\n' && **cursor != '}')
		return (SUCCESS);
	if (*line_ptr)
		free(*line_ptr);
	*line_ptr = get_next_line(fd);
	if (!*line_ptr)
		return (ERROR);
	*cursor = *line_ptr;
	return (SUCCESS);
}

char	*ft_skip_spaces(char *str)
{
	if (!str)
		return (NULL);
	while ((*str >= 9 && *str <= 13) || *str == ' ')
	{
		str++;
		if (!*str)
			break ;
	}
	return (str);
}

void	ft_skip_closing_brace(char **cursor, char **line, int fd)
{
	if (*cursor && ft_strchr(*cursor, '}'))
		return ;
	while (TRUE)
	{
		if (*line)
			free(*line);
		*line = get_next_line(fd);
		if (!*line)
			break ;
		*cursor = *line;
		if (*cursor && ft_strchr(*cursor, '}'))
			break ;
	}
}

void	ft_jump_to_next_value(char **cursor, char **line, int fd)
{
	while (TRUE)
	{
		*cursor = ft_skip_spaces(*cursor);
		if (**cursor == ',')
		{
			(*cursor)++;
			continue ;
		}
		if (!**cursor)
		{
			if (ft_extract_line(cursor, line, fd))
				return ;
			continue ;
		}
		if (ft_isdigit(**cursor) || **cursor == '-')
			break ;
		if (**cursor == '}')
			break ;
		(*cursor)++;
	}
}

void	*ft_get_by_id(t_list *list, long id)
{
	long	*ptr;

	while (list)
	{
		if (list->content)
		{
			ptr = (long *)list->content;
			if (*ptr == id)
				return (list->content);
		}
		list = list->next;
	}
	return (NULL);
}
