/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:27:27 by vlad              #+#    #+#             */
/*   Updated: 2026/02/06 22:29:47 by vbleskin         ###   ########.fr       */
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

// *** DEBUG ***
void	ft_print_anim_stack(t_list *list)
{
	t_anim_stack	*stack;
	int				i;

	i = 0;
	while (list)
	{
		stack = list->content;
		i++;
		printf("Anim %d : %s\n", i, stack->name);
		list = list->next;
	}
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

static double	ft_get_max_duration(t_anim_node *node, double max_duration)
{
	double	end;

	if (!node)
		return (max_duration);
	if (node->x && node->x->n_keys > 0)
	{
		// printf("1\n"); // debug
		end = node->x->time[node->x->n_keys - 1];
		if (end > max_duration)
			max_duration = end;
	}
	if (node->y && node->y->n_keys > 0 && node->y->time)
	{
		// printf("2\n"); // debug
		end = node->y->time[node->y->n_keys - 1];
		if (end > max_duration)
			max_duration = end;
	}
	if (node->z && node->z->n_keys > 0 && node->y->time)
	{
		// printf ("3\n"); // debug
		end = node->z->time[node->z->n_keys - 1];
		if (end > max_duration)
			max_duration = end;
	}
	// printf("return\n"); // debug
	return (max_duration);
}

double	ft_get_anim_duration(t_anim_stack *anim)
{
	t_list			*layers;
	t_list			*nodes;
	t_anim_layer	*layer;
	double			max_duration;

	max_duration = 0.0;
	if (!anim)
		return (0.0);
	layers = anim->layers;
	while (layers)
	{
		layer = (t_anim_layer *)layers->content;
		nodes = layer->nodes;
		while (nodes)
		{
			max_duration = ft_get_max_duration((t_anim_node *)nodes->content, max_duration);
			nodes = nodes->next;
		}
		layers = layers->next;
	}
	return (max_duration);
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
	t_list			*anims;
	t_list			*current_in_list;

	if (!data->fbx || !data->fbx->anim_stack)
		return ;
	current_in_list = ft_find_node(data->fbx->anim_stack, data->fbx->current_anim);
	if (current_in_list && current_in_list->next)
		data->fbx->current_anim = (t_anim_stack *)(current_in_list->next->content);
	else if (data->fbx->anim_stack)
		data->fbx->current_anim = (t_anim_stack *)(data->fbx->anim_stack->content);
	anims = data->fbx->anim_stack;
	// printf("*N : NEXT ANIMATION : \n"); // debug
	// printf("- Current : %s\n", data->fbx->current_anim->name); // debug
	if (!data->fbx->current_anim)
	{
		// printf("- Next : %s\n", ((t_anim_stack *)anims->content)->name); // debug
		data->fbx->current_anim = (t_anim_stack *)anims->content;
		// printf("- New current : %s\n", data->fbx->current_anim->name); // debug
	}
	current_in_list = ft_find_in_list(anims, (void *)data->fbx->current_anim);
    if (!current_in_list || current_in_list->next)
	{
		// printf("- Next : %s\n", ((t_anim_stack *)(current_in_list->next->content))->name); // debug
		data->fbx->current_anim = (t_anim_stack *)(current_in_list->next->content);
		// printf("- New current : %s\n", data->fbx->current_anim->name); // debug
	}
    else
	{
		// printf("- Next : %s\n", ((t_anim_stack *)anims->content)->name); // debug
		data->fbx->current_anim = (t_anim_stack *)anims->content;
		// printf("- New current : %s\n", data->fbx->current_anim->name); // debug
	}
	// printf("- weight value before : %f\n", data->timer.weighted_value); // debug
	data->timer.weighted_value = 0;
	// printf("- weight value after : %f\n", data->timer.weighted_value); // debug
	// printf("- duration before : %f\n", data->timer.duration); // debug
	data->timer.duration = ft_get_anim_duration(data->fbx->current_anim);
	if (data->timer.duration == 0)
		data->timer.duration = 1.0;
	// printf("- duration after : %f\n", data->timer.duration); // debug
	// printf("Timer updated\n"); // debug
}

void	ft_prev_anim(t_fdf *data)
{
	t_list			*anims;
	t_list			*last;

	if (!data->fbx || !data->fbx->anim_stack)
		return ;
	anims = data->fbx->anim_stack;
	printf("*P : PREV ANIMATION : \n"); //debug
	printf("- Current : %s\n", data->fbx->current_anim->name); // debug
	if (data->fbx->current_anim == anims->content)
	{
		last = ft_lstlast(anims);
		if (last)
		{
			printf("- Prev : %s\n", ((t_anim_stack *)last->content)->name); // debug
			data->fbx->current_anim = (t_anim_stack *)last->content;
			printf("- New current : %s\n", data->fbx->current_anim->name); // debug
		}
	}
	else
	{
		while (anims->next)
		{
			if ((t_anim_stack *)anims->next->content == data->fbx->current_anim)
			{
				printf("- Prev : %s\n", ((t_anim_stack *)anims->content)->name); // debug
				data->fbx->current_anim = (t_anim_stack *)anims->content;
				printf("- New current : %s\n", data->fbx->current_anim->name); // debug
				break ;
			}
			anims = anims->next;
		}
	}
	printf("- weight value before : %f\n", data->timer.weighted_value); // debug
	data->timer.weighted_value = 0;
	printf("- weight value after : %f\n", data->timer.weighted_value); // debug
	printf("- duration before : %f\n", data->timer.duration); // debug
	data->timer.duration = ft_get_anim_duration(data->fbx->current_anim);
	if (data->timer.duration == 0)
		data->timer.duration = 1.0;
	printf("- duration after : %f\n", data->timer.duration); // debug
	printf("Timer updated\n"); // debug
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
