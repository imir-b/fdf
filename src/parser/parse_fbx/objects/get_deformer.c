/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_deformer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 20:59:15 by vlad              #+#    #+#             */
/*   Updated: 2026/02/28 21:25:21 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_parse_deformer_tags(t_deformer *def, char **line, char *cur,
			int fd)
{
	if (IS_TAG(cur, "Indexes"))
		def->verticies = ft_parse_verticies(line, cur, fd, &def->n_vertices);
	else if (IS_TAG(cur, "Weights"))
		def->weights = ft_parse_weights(line, cur, fd);
	else if (IS_TAG(cur, "TransformLink"))
		def->t_link = ft_parse_transform(line, cur, fd);
	else if (IS_TAG(cur, "Transform"))
		def->transform = ft_parse_transform(line, cur, fd);
}

/**
 * 	Deformer: 1000082, "SubDeformer::", "Cluster" {
 *		Version: 100
 *		UserData: "", ""
 *		Indexes: *26 {
 *			a: 1480,1481,1482,...
 *		} 
 *		Weights: *26 {
 *			a: 0.0470588244497776,0.0980392172932625,0.705882370471954,...
 *		} 
 *		Transform: *16 {
 *			a: 0.385069936513901,0.830801069736481,-0.401859313249588,...
 *		} 
 *		TransformLink: *16 {
 *			a: 0.385069906711578,-0.127643629908562,-0.914017617702484,...
 *	}
 */
t_deformer	*ft_get_deformer(char *cursor, int fd)
{
	t_deformer	*def;
	char		*line;

	def = ft_calloc(1, sizeof(t_deformer));
	if (!def)
		return (NULL);
	def->id = ft_atol(cursor);
	while (1)
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
		ft_parse_deformer_tags(def, &line, cursor, fd);
		free(line);
	}
	return (def);
}
