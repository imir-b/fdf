/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:29:51 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:32:41 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_connect_anim_to_model(t_fbx *data, char *line, long *ids)
{
	t_anim_node	*anim;
	t_model		*model;

	anim = (t_anim_node *)ft_get_by_id(data->anim_node, ids[0]);
	if (anim)
	{
		model = (t_model *)ft_get_by_id(data->model, ids[1]);
		if (model)
		{
			anim->target = model;
			line += 4;
			if (ft_is_tag(line, "Translation"))
				model->anim_pos = anim;
			else if (ft_is_tag(line, "Rotation"))
				model->anim_rot = anim;
			else if (ft_is_tag(line, "Scaling"))
				model->anim_scale = anim;
		}
	}
}

void	ft_connect_anim_to_anim(t_fbx *data, char *line, long *ids)
{
	t_anim_curve	*anim_curve;
	t_anim_node		*anim_node;

	anim_curve = (t_anim_curve *)ft_get_by_id(data->anim_curve, ids[0]);
	if (anim_curve)
	{
		anim_node = (t_anim_node *)ft_get_by_id(data->anim_node, ids[1]);
		if (anim_node)
		{
			line += 2;
			if (ft_is_tag(line, "X"))
				anim_node->x = anim_curve;
			else if (ft_is_tag(line, "Y"))
				anim_node->y = anim_curve;
			else if (ft_is_tag(line, "Z"))
				anim_node->z = anim_curve;
		}
	}
}
