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

