/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:10 by vlad              #+#    #+#             */
/*   Updated: 2026/02/06 22:20:56 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_print_model(t_model *model)
{
	printf("- Model id : %li\n", model->id);
}

void	ft_print_curve(t_anim_curve *curve)
{
	if (!curve)
		return ;
	printf("- Curve id : %li\n", curve->id);
	printf("- n keys : %d\n", curve->n_keys);
	if (!curve->time || !curve->value) 
	{
		printf("  [!] WARNING: Tableaux NULL pour la courbe %li\n", curve->id);
		return ;
	}
	int i = 0;
	while (i < curve->n_keys)
	{
		if (!curve->time[i])
		{
			printf("no value %d for time !\n", i);
			return ;
		}
		printf("- time : %f\n", curve->time[i]);
		i++;
	}
	i = 0;
	while (i < curve->n_keys)
	{
		if (!curve->value[i])
		{
			printf("no value %d for value !\n", i);
			return ;
		}
		printf("- value %d : %f\n", i, curve->value[i]);
		i++;
	}
}

void	ft_print_anims(t_fbx *fbx)
{
	t_list			*anims = fbx->anim_stack;
	t_anim_stack	*anim;
	t_list			*layers;
	t_anim_layer	*layer;
	t_list			*nodes;
	t_anim_node		*node;

	while (anims)
	{
		anim = (t_anim_stack *)anims->content;
		printf("ANIMATION CONTENT\n");
		printf("- Stack id : %li\n",  anim->id);
		printf("- Name : %s\n", anim->name);
		layers = anim->layers;
		while (layers)
		{
			layer = (t_anim_layer *)layers->content;
			printf("- Layer id : %li\n", layer->id);
			nodes = layer->nodes;
			while (nodes)
			{
				node = (t_anim_node *)nodes->content;
				printf("- Node id : %li\n", node->id);
				printf("- Type : %c\n", node->type);
				ft_print_model(node->target);
				if (node->x)
					ft_print_curve(node->x);
				else
					printf("!node->x\n");
				if (node->y)
					ft_print_curve(node->y);
				else
					printf("!node->y\n");
				if (node->z)
					ft_print_curve(node->z);
				else
					printf("!node->z\n");
				nodes = nodes->next;
			}
			layers = layers->next;
		}
		anims = anims->next;
	}
}

int	main(int ac, char **av)
{
	const char	*filename;
	t_object	*object;
	t_fbx		*fbx;

	if (ac != 2)
		return (ft_error("This program needs 2 args"));
	filename = (const char *)av[1];
	if (ft_check_filename(filename))
		return (ft_error("File name is not valid"));
	fbx = NULL;
	object = ft_parse_dispatch(filename, &fbx);
	if (fbx)
	{
		if (fbx->anim_stack)
			fbx->current_anim = (t_anim_stack *)ft_lstlast(fbx->anim_stack)->content;
	}
	if (!object)
		return (ft_error("Parsing map failed"));
	ft_process_fdf(object, fbx);
	ft_free_object(object);
	return (SUCCESS);
}

