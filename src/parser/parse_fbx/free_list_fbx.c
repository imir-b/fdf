/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fbx_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:26:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:27:54 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_free_geometry_list(t_fbx *data)
{
	t_list	*tmp;

	while (data->geo)
	{
		tmp = data->geo->next;
		ft_free_geo(data->geo->content);
		free(data->geo);
		data->geo = tmp;
	}
	while (data->model)
	{
		tmp = data->model->next;
		free(data->model->content);
		free(data->model);
		data->model = tmp;
	}
}

static void	ft_free_animation_list(t_fbx *data)
{
	t_list	*tmp;

	while (data->anim_curve)
	{
		tmp = data->anim_curve->next;
		ft_free_anim(data->anim_curve->content);
		free(data->anim_curve);
		data->anim_curve = tmp;
	}
	while (data->anim_node)
	{
		tmp = data->anim_node->next;
		free(data->anim_node->content);
		free(data->anim_node);
		data->anim_node = tmp;
	}
}

static void	ft_free_layers_and_stacks(t_fbx *data)
{
	t_list	*tmp;

	while (data->anim_stack)
	{
		tmp = data->anim_stack->next;
		ft_free_stack(data->anim_stack->content);
		free(data->anim_stack);
		data->anim_stack = tmp;
	}
	while (data->anim_layer)
	{
		tmp = data->anim_layer->next;
		free(data->anim_layer->content);
		free(data->anim_layer);
		data->anim_layer = tmp;
	}
}

void	*ft_free_fbx_data(t_fbx *data)
{
	if (!data)
		return (NULL);
	ft_free_geometry_list(data);
	ft_free_animation_list(data);
	ft_free_layers_and_stacks(data);
	free(data);
	return (NULL);
}
