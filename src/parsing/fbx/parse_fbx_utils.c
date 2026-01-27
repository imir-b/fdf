/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fbx_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:10:52 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/27 10:48:10 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

char	*ft_extract_line(char **cursor, int fd)
{
	char	*line;

	line = NULL;
	if (**cursor == '\0' || **cursor == '\n' || **cursor == '}')
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		*cursor = line;
		while (**cursor && !ft_isdigit(**cursor) && **cursor != '-')
			(*cursor)++;
	}
	return (line);
}

void	ft_move_cursor(char **cursor)
{
	if (*cursor)
		(*cursor)++;
	else
		*cursor = "";
}

/*modifier la fonction pour free tous les elements de la liste chainee*/
void	*ft_free_fbx_data(t_fbx *data)
{
	if (data->geo)
		free(data->geo);
	if (data->model)
		free(data->model);
	if (data->anim_curve->time)
		free(data->anim_curve->time);
	if (data->anim_curve->value)
		free(data->anim_curve->value);
	if (data->anim_curve)
		free(data->anim_curve);
	if (data->anim_node)
		free(data->anim_node);
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
