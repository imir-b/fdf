/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:17:30 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/31 21:08:18 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_is_extension(const char *filename, const char *extension)
{
	char	*filename_end;

	filename_end = ft_strrchr(filename, '.');
	if (!ft_strncmp(filename_end, extension, 4))
		return (TRUE);
	return (FALSE);
}

int	ft_check_filename(const char *filename)
{
	char	*format;
	int		format_len;
	int		len;

	format = ft_strrchr(filename, '.');
	if (!format)
		return (ERROR);
	len = ft_strlen(filename);
	format_len = ft_strlen((const char *)format);
	if ((!ft_strncmp(format, ".fdf", 4) || \
		!ft_strncmp(format, ".obj", 4) || \
		!ft_strncmp(format, ".fbx", 4)) \
			&& format_len == 4 && format_len < len)
		return (SUCCESS);
	else
		return (ERROR);
}

t_object	*ft_parse_dispatch(const char *filename)
{
	t_object	*object;

	object = ft_calloc(1, sizeof(t_object));
	if (!object)
		return (NULL);
	if (ft_is_extension(filename, ".fdf"))
		return (ft_parse_fdf(filename, object));
	else if (ft_is_extension(filename, ".obj"))
		return (ft_parse_obj(filename, object));
	else if (ft_is_extension(filename, ".fbx"))
	{
		object = ft_parse_fbx(filename, object);
		if (!object)
			return (NULL);
		return (object);
	}
	ft_free_object(object);
	return (NULL);
}
