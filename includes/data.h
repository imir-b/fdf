/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:15 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 01:09:42 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

typedef struct s_map
{
	int	width;
	int	height;
	int	**grid;
}	t_map;

typedef struct s_camera
{
	int		shift_x;
	int		shift_y;
	int		zoom;
	float	z_scale;
	double	angle_x;
	double	angle_y;
}	t_camera;

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
	t_mouse		mouse;
}	t_fdf;

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