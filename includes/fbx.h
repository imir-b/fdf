/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fbx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:03:24 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/25 17:04:47 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FBX_H
# define FBX_H

# include "data.h"

# define IS_TAG(str, tag) (!ft_strncmp(str, tag, sizeof(tag) - 1))

typedef struct s_geometry
{
	long		id;
	t_object	*obj;
	t_geometry	*next;
}	t_geometry;

typedef struct s_properties
{
	char	type;
	double	x;
	double	y;
	double	z;
}	t_properties;

typedef struct s_model
{
	long			id;
	t_properties	pos;
	t_properties	rot;
	t_properties	scale;
	t_anim_node		*anim_pos;
	t_anim_node		*anim_rot;
	t_anim_node		*anim_scale;
	t_geometry		*geo;
	t_model			*parent;
	t_model			*next;
}	t_model;

typedef struct s_anim_curve
{
	long			id;
	int				n_keys;
	long long		*time;
	double			*value;
	t_anim_curve	*next;
}	t_anim_curve;

typedef struct s_anim_node
{
	long			id;
	char			type;
	t_anim_curve	*x;
	t_anim_curve	*y;
	t_anim_curve	*z;
	t_anim_node		*next;
}	t_anim_node;

typedef struct s_fbx
{
	t_geometry		*geo;
	t_model			*model;
	t_anim_curve	*anim_curve;
	t_anim_node		*anim_node;
}	t_fbx;

#endif