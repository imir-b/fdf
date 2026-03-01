/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:17:30 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 13:47:25 by vbleskin         ###   ########.fr       */
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

/**
 * Vérifie que l'extension du fichier passé en argument est bien supportée
 * par le programme (.fdf, .obj, ou .fbx).
 */
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
	if ((!ft_strncmp(format, ".fdf", 4) || !ft_strncmp(format, ".obj", 4)
			|| !ft_strncmp(format, ".fbx", 4))
		&& format_len == 4 && format_len < len)
		return (SUCCESS);
	else
		return (ERROR);
}

/**
 * Fonction de routage principale (dispatcher) pour le parsing.
 * Détecte l'extension du fichier et appelle le parser correspondant
 * (fdf, obj ou fbx). Convertit ensuite la géométrie pour retourner l'objet final.
 */
t_object	*ft_parse_dispatch(const char *filename, t_fbx **fbx)
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
		*fbx = ft_parse_fbx(filename);
		if (!*fbx)
			return (ft_free_object(object));
		object = ft_convert_fbx_to_object(*fbx);
		if (!object)
			return (NULL);
		return (object);
	}
	ft_free_object(object);
	return (NULL);
}
