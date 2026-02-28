/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fbx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:28:39 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:28:44 by vbleskin         ###   ########.fr       */
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
