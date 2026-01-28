/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fbx_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:10:52 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/28 06:11:29 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_extract_line(char **cursor, char **line_ptr, int fd)
{
	if (**cursor == '\0' || **cursor == '\n' || **cursor == '}')
	{
		if (*line_ptr)
			free(*line_ptr);
		*line_ptr = get_next_line(fd);
		if (!*line_ptr)
			return (ERROR);
		*cursor = *line_ptr;
		while (**cursor && !ft_isdigit(**cursor) && **cursor != '-')
			(*cursor)++;
	}
	return (SUCCESS);
}

void	ft_move_cursor(char **cursor)
{
	if (*cursor)
		(*cursor)++;
	else
		*cursor = "";
}

/*modifier la fonction pour free tous les elements de la liste*/
void	*ft_free_fbx_data(t_fbx *data)
{
	if (data)
		free(data);
	return (NULL);
}

char	*ft_skip_spaces(char *str)
{
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	return (str);
}
