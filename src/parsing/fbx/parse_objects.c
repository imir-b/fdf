/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:01:45 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 04:35:35 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_add_new_geo(t_fbx *data, char *cursor, int fd)
{
	t_geometry	*geo;
	t_list		*new;

	geo = ft_get_geometry(cursor, fd);
	if (!geo)
		return (ERROR);
	new = ft_lstnew(geo);
	if (!new)
		return (ERROR);
	ft_lstadd_front(&data->geo, new);
	return (SUCCESS);
}

static int	ft_add_new_model(t_fbx *data, char *cursor, int fd)
{
	t_model	*model;
	t_list	*new;

	model = ft_get_model(cursor, fd);
	if (!model)
		return (ERROR);
	new = ft_lstnew(model);
	if (!new)
		return (ERROR);
	ft_lstadd_front(&data->model, new);
	return (SUCCESS);
}

static int	ft_add_new_anim_curve(t_fbx *data, char *cursor, int fd)
{
	t_anim_curve	*anim_curve;
	t_list			*new;

	anim_curve = ft_get_anim_curve(cursor, fd);
	if (!anim_curve)
		return (ERROR);
	new = ft_lstnew(anim_curve);
	if (!new)
		return (ERROR);
	ft_lstadd_front(&data->anim_curve, new);
	return (SUCCESS);
}

static int	ft_add_new_anim_node(t_fbx *data, char *cursor, int fd)
{
	t_anim_node	*anim_node;
	t_list		*new;

	anim_node = ft_get_anim_node(cursor, fd);
	if (!anim_node)
		return (ERROR);
	new = ft_lstnew(anim_node);
	if (!new)
		return (ERROR);
	ft_lstadd_front(&data->anim_node, new);
	return (SUCCESS);
}

/**
 * Fonction de parsing pour recuperer les donnees dans la partie 'Objects'
 * d'un fichier .fbx
 */
int	ft_parse_objects(t_fbx *fbx_data, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line || line[0] == '}')
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "Geometry"))
			ft_add_new_geo(fbx_data, cursor + 9, fd);
		else if (IS_TAG(cursor, "Model"))
			ft_add_new_model(fbx_data, cursor + 6, fd);
		else if (IS_TAG(cursor, "AnimationCurve"))
			ft_add_new_anim_curve(fbx_data, cursor + 15, fd);
		else if (IS_TAG(cursor, "AnimationCurveNode"))
			ft_add_new_anim_node(fbx_data, cursor + 19, fd);
		else if (IS_TAG(cursor, "AnimationStack"))
			ft_add_new_anim_stack();
		else if (IS_TAG(cursor, "AnimationLayer"))
		free(line);
	}
	return (SUCCESS);
}
