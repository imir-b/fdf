/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:15 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/19 03:08:58 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

# define RADIAN_30 0.523599
# define RADIAN_90 1.5708

#define COS_30 0.8660254
#define SIN_30 0.5

# define ISOMETRIC 0
# define PARALLEL 1

# define WHITE 16777215

# define THREADS_NB 4

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
	int		sx;
	int		sy;
	int		color;
}	t_vec3;

typedef struct s_face
{
	int	*indices;
	int	count;
}	t_face;

typedef struct s_object
{
	t_vec3	*vertices;
	int		nb_vertices;
	t_face	*faces;
	int		nb_faces;
	int		width;
	int		height;
}	t_object;

typedef struct s_camera
{
	int		shift_x;
	int		shift_y;
	int		zoom;
	float	z_scale;
	int		projection;
	double	angle_x;
	double	angle_y;
}	t_camera;

typedef struct s_maths
{
	float	sin_alpha;
	float	cos_alpha;
	float	sin_beta;
	float	cos_beta;
}	t_maths;

typedef struct s_mouse
{
	int	x;
	int	y;
	int	is_pressed;
}	t_mouse;

typedef struct s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_maths		trigo;
	t_object	*object;
	t_camera	*camera;
	t_mouse		mouse;
}	t_fdf;

typedef struct s_thread
{
	t_fdf	*data;
	int		id;
	int		start;
	int		end;
}	t_thread;

typedef struct s_bresenham
{
	int		diff_x;
	int		diff_y;
	int		step_x;
	int		step_y;
	int		err;
}	t_bresenham;

#endif