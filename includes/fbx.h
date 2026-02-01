/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fbx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 17:03:24 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 17:16:30 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FBX_H
# define FBX_H

# include "data.h"

# define IS_TAG(str, tag) (!ft_strncmp(str, tag, sizeof(tag) - 1))

typedef struct s_geometry		t_geometry;
typedef struct s_properties		t_properties;
typedef struct s_model			t_model;
typedef struct s_anim_curve		t_anim_curve;
typedef struct s_anim_node		t_anim_node;
typedef struct s_anim_layer		t_anim_layer;
typedef struct s_anim_stack		t_anim_stack;
typedef struct s_fbx			t_fbx;

struct s_geometry
{
	long		id;
	t_object	*obj;
};

struct s_properties
{
	char	type;
	double	x;
	double	y;
	double	z;
};

struct s_model
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
};

struct s_anim_curve
{
	long			id;
	int				n_keys;
	long long		*time;
	double			*value;
};

struct s_anim_node
{
	long			id;
	char			type;
	t_anim_curve	*x;
	t_anim_curve	*y;
	t_anim_curve	*z;
};

struct s_anim_layer
{
	long	id;
	t_list	*nodes;
};

struct s_anim_stack
{
	long	id;
	char	*name;
	t_list	*layers;
};

struct s_fbx
{
	t_list	*geo;
	t_list	*model;
	t_list	*anim_curve;
	t_list	*anim_node;
	t_list	*anim_stack;
	t_list	*anim_layer;
};

#endif