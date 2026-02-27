/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:01:45 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 11:41:59 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_add_element(char *cursor, int fd, t_list **list,
			t_get_func f_get)
{
	void	*content;
	t_list	*new;

	content = f_get(cursor, fd);
	if (!content)
		return (ERROR);
	new = ft_lstnew(content);
	if (!new)
		return (ERROR);
	ft_lstadd_front(list, new);
	return (SUCCESS);
}

void	ft_add_current_element(char *line, int fd, t_fbx *data)
{
	if (IS_TAG(line, "Geometry"))
		ft_add_element(line + 9, fd, &data->geo, (t_get_func)ft_get_geometry);
	else if (IS_TAG(line, "Model"))
		ft_add_element(line + 6, fd, &data->model, (t_get_func)ft_get_model);
	else if (IS_TAG(line, "Deformer"))
		ft_add_element(line + 9, fd, &data->deformer,
			(t_get_func)ft_get_deformer);
	else if (IS_TAG(line, "AnimationCurveNode"))
		ft_add_element(line + 19, fd, &data->anim_node,
			(t_get_func)ft_get_anim_node);
	else if (IS_TAG(line, "AnimationCurve"))
		ft_add_element(line + 15, fd, &data->anim_curve,
			(t_get_func)ft_get_anim_curve);
	else if (IS_TAG(line, "AnimationStack"))
		ft_add_element(line + 15, fd, &data->anim_stack,
			(t_get_func)ft_get_anim_stack);
	else if (IS_TAG(line, "AnimationLayer"))
		ft_add_element(line + 15, fd, &data->anim_layer,
			(t_get_func)ft_get_anim_layer);
}

/**
 * Fonction de parsing pour recuperer les donnees dans la partie 'Objects'
 * d'un fichier .fbx
 */
int	ft_parse_objects(t_fbx *data, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line || line[0] == '}')
			break ;
		cursor = ft_skip_spaces(line);
		ft_add_current_element(cursor, fd, data);
		free(line);
	}
	return (SUCCESS);
}
