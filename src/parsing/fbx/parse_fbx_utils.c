/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fbx_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:10:52 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/29 04:39:59 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_skip_to_content(char **cursor)
{
	if (!cursor || !*cursor)
		return ;
	while (**cursor && !ft_isdigit(**cursor) && **cursor != '-')
		(*cursor)++;
}

/**
 * Defini 'line_ptr' sur la prochaine ligne si 'cursor' est
 * nul '\0', a la fin du parsing ('}') ou en fin de ligne '\n'
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

void	ft_move_cursor(char **cursor)
{
	if (*cursor)
		(*cursor)++;
	else
		*cursor = "";
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
