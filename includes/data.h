/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:15 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/02 15:21:32 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

# define RADIAN_30 0.523599
# define RADIAN_90 1.5708

# define ISOMETRIC 0
# define PARALLEL 1

# define WHITE 16777215

# define THREADS_NB 4

typedef struct s_map
{
	int	width;
	int	height;
	int	**colors;
	int	**grid;
}	t_map;

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
	float	cos_x;
	float	sin_x;
	float	cos_y;
	float	sin_y;
	float	cos_30;
	float	sin_30;
	int		divide_x;
	int		divide_y;
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
	t_map		*map;
	t_camera	*camera;
	t_maths		*maths;
	t_mouse		mouse;
}	t_fdf;

typedef struct s_thread
{
	t_fdf	*data;
	int		id;
	int		start;
	int		end;
}	t_thread;

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

typedef struct s_pixel
{
	int	x;
	int	y;
}	t_pixel;

typedef struct s_bresenham
{
	int		diff_x;
	int		diff_y;
	int		step_x;
	int		step_y;
	int		err;
}	t_bresenham;

#endif