/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:27:27 by vlad              #+#    #+#             */
/*   Updated: 2026/02/03 01:07:02 by vlad             ###   ########.fr       */
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

t_list  *ft_find_node(t_list *list, void *content_to_find)
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
	t_list	*current_node;

    current_node = ft_find_node(data->fbx->anim_stack, data->fbx->current_anim);
    if (current_node && current_node->next)
		data->fbx->current_anim = (t_anim_stack *)(current_node->next->content);
    else
		data->fbx->current_anim = (t_anim_stack *)(data->fbx->anim_stack->content);
	data->timer.weighted_value = 0;
}

void	ft_prev_anim(t_fdf *data)
{
	t_list	*temp;

	if (data->fbx->current_anim == (t_anim_stack *)data->fbx->anim_stack->content)
	{
		t_list *last = ft_lstlast(data->fbx->anim_stack);
		data->fbx->current_anim = (t_anim_stack *)last->content;
		return ;
	}
	temp = data->fbx->anim_stack;
	while (temp->next)
	{
		if ((t_anim_stack *)temp->next->content == data->fbx->current_anim)
		{
			data->fbx->current_anim = (t_anim_stack *)temp->content;
			return ;
		}
		temp = temp->next;
	}
}

int	ft_display_anim_menu(t_fdf *data)
{
	t_list			*anims;
	t_anim_stack	*current;

	anims = data->fbx->anim_stack;
	if (data->fbx->current_anim)
		current = data->fbx->current_anim;
	else
		current = (t_anim_stack *)anims->content;
	mlx_string_put(data->mlx_ptr, data->win_ptr, 50, 100, 0xFFFFFF, current->name);
	return (SUCCESS);
}
