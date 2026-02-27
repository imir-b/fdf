/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:27:27 by vlad              #+#    #+#             */
/*   Updated: 2026/02/15 16:21:54 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_display_fps(t_fdf *data)
{
	static long		last_check = 0;
	static int		frames = 0;
	static char		*final_str = NULL;
	char			*fps_str;
	long			current_time;

	current_time = ft_get_time_ms();
	frames++;
	if (current_time - last_check >= 1000)
	{
		if (final_str)
			free(final_str);
		fps_str = ft_itoa(frames);
		if (!fps_str)
			return (ERROR);
		final_str = ft_strjoin("FPS : ", fps_str);
		free(fps_str);
		if (!final_str)
			return (ERROR);
		frames = 0;
		last_check = current_time;
	}
	if (final_str)
		mlx_string_put(data->mlx_ptr, data->win_ptr, 50, 20, 0xFFFFFF, final_str);
	return (SUCCESS);
}

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

t_list	*ft_find_in_list(t_list *list, void *content)
{
	while (list)
	{
		if (list->content == content)
			return (list);
		list = list->next;
	}
	return (NULL);
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
