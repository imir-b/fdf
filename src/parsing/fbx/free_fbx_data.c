/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fbx_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:26:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 18:58:30 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	*ft_free_geo(t_geometry *geo)
{
	if (geo->obj)
		ft_free_object(geo->obj);
	if (geo)
		free(geo);
	return (NULL);
}

void	*ft_free_anim(t_anim_curve *anim_curve)
{
	if (anim_curve->value)
		free(anim_curve->value);
	if (anim_curve->time)
		free(anim_curve->time);
	if (anim_curve)
		free(anim_curve);
	return (NULL);
}

void	*ft_free_stack(t_anim_stack *anim_stack)
{
	if (anim_stack->name)
		free(anim_stack->name);
	if (anim_stack)
		free(anim_stack);
	return (NULL);
}

void	*ft_free_fbx_data(t_fbx *data)
{
	while (data->geo)
	{
		ft_free_geo(data->geo->content);
		data->geo = data->geo->next;
	}
	while (data->model)
	{
		free(data->model->content);
		data->model = data->model->next;
	}
	while (data->anim_curve)
	{
		ft_free_anim(data->anim_curve->content);
		data->anim_curve = data->anim_curve->next;
	}
	while (data->anim_node)
	{
		free(data->anim_node->content);
		data->anim_node = data->anim_node->next;
	}
	while (data->anim_stack)
	{
		ft_free_stack(data->anim_stack->content);
		data->anim_stack = data->anim_stack->next;
	}
	while (data->anim_layer)
	{
		free(data->anim_layer->content);
		data->anim_layer = data->anim_layer->next;
	}
	if (data)
		free(data);
	return (NULL);
}
