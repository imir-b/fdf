/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_anim_menu.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 03:38:14 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 03:43:00 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_list	*ft_find_node(t_list *list, void *content_to_find)
{
	while (list)
	{
		if (list->content == content_to_find)
			return (list);
		list = list->next;
	}
	return (NULL);
}

int	ft_pause(t_fdf *data)
{
	if (!data || !data->fbx)
		return (ERROR);
	data->timer.is_paused = !data->timer.is_paused;
	if (!data->timer.is_paused)
		data->timer.last_frame = ft_get_time_ms();
	return (SUCCESS);
}

void	ft_next_anim(t_fdf *data)
{
	t_list			*current_in_list;

	if (!data->fbx || !data->fbx->anim_stack)
		return ;
	current_in_list = ft_find_node(data->fbx->anim_stack, \
		data->fbx->current_anim);
	if (current_in_list && current_in_list->next)
		data->fbx->current_anim = \
			(t_anim_stack *)(current_in_list->next->content);
	else
		data->fbx->current_anim = \
			(t_anim_stack *)(data->fbx->anim_stack->content);
	ft_reset_models_to_base(data);
	data->timer.weighted_value = 0;
	data->timer.duration = data->fbx->current_anim->duration;
	if (data->timer.duration == 0)
		data->timer.duration = 1.0;
}

void	ft_prev_anim(t_fdf *data)
{
	t_list			*anims;
	t_list			*last;

	if (!data->fbx || !data->fbx->anim_stack)
		return ;
	anims = data->fbx->anim_stack;
	if (data->fbx->current_anim == anims->content)
	{
		last = ft_lstlast(anims);
		if (last)
			data->fbx->current_anim = (t_anim_stack *)last->content;
	}
	else
	{
		while (anims->next)
		{
			if ((t_anim_stack *)anims->next->content == \
				data->fbx->current_anim)
			{
				data->fbx->current_anim = \
					(t_anim_stack *)anims->content;
				break ;
			}
			anims = anims->next;
		}
	}
	ft_reset_models_to_base(data);
	data->timer.weighted_value = 0;
	data->timer.duration = data->fbx->current_anim->duration;
	if (data->timer.duration == 0)
		data->timer.duration = 1.0;
}

int	ft_display_anim_menu(t_fdf *data)
{
	t_list			*anims;
	t_anim_stack	*current;

	if (!data->fbx || !data->fbx->anim_stack)
		return (SUCCESS);
	anims = data->fbx->anim_stack;
	if (data->fbx->current_anim)
		current = data->fbx->current_anim;
	else if (anims && anims->content)
		current = (t_anim_stack *)anims->content;
	else
		return (SUCCESS);
	if (current && current->name)
		mlx_string_put(data->mlx_ptr, data->win_ptr, 50, 100, 0xFFFFFF, \
							current->name);
	if (data->timer.is_paused)
		mlx_string_put(data->mlx_ptr, data->win_ptr, 80, 200, 0xFFFFFF, \
						"PAUSED");
	return (SUCCESS);
}
