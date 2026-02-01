/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:15 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/01 23:10:05 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

# define RADIAN_30 0.523599
# define RADIAN_90 1.5708

# define COS_30 0.8660254
# define SIN_30 0.5

# define ISOMETRIC 0
# define PARALLEL 1

# define WHITE 16777215

# define THREADS_NB 4

# define IS_TAG(str, tag) (!ft_strncmp(str, tag, sizeof(tag) - 1))

typedef struct s_point			t_point;
typedef struct s_vec3			t_vec3;
typedef struct s_face			t_face;
typedef struct s_object			t_object;
typedef struct s_camera			t_camera;
typedef struct s_maths			t_maths;
typedef struct s_mouse			t_mouse;
typedef struct s_timer			t_timer;
typedef struct s_img			t_img;
typedef struct s_fdf			t_fdf;
typedef struct s_thread			t_thread;
typedef struct s_bresenham		t_bresenham;
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

struct s_point
{
	int	x;
	int	y;
	int	z;
};

struct s_vec3
{
	double	x;
	double	y;
	double	z;
	int		sx;
	int		sy;
	int		color;
};

struct s_face
{
	int	*indices;
	int	count;
};

struct s_object
{
	t_vec3	*vertices;
	int		nb_vertices;
	t_face	*faces;
	int		nb_faces;
	int		width;
	int		height;
};

struct s_camera
{
	int		shift_x;
	int		shift_y;
	int		zoom;
	float	z_scale;
	int		projection;
	double	angle_x;
	double	angle_y;
};

struct s_maths
{
	float	sin_alpha;
	float	cos_alpha;
	float	sin_beta;
	float	cos_beta;
};

struct s_bresenham
{
	int		diff_x;
	int		diff_y;
	int		step_x;
	int		step_y;
	int		err;
};

struct s_mouse
{
	int	x;
	int	y;
	int	is_pressed;
};

struct s_timer {
    long    start_time;    // Temps au lancement (en ms)
    long    last_frame;    // Temps de la frame précédente (en ms)
    double  delta_time;    // Temps écoulé entre 2 frames (en secondes)
    double	weighted_value;
    double  duration;
};

struct s_img
{
	void	*ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
};

struct s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	t_maths		trigo;
	t_object	*object;
	t_camera	*camera;
	t_fbx		*fbx;
	t_mouse		mouse;
	t_timer		timer;
};

struct s_thread
{
	t_fdf	*data;
	int		id;
	int		start;
	int		end;
};

#endif