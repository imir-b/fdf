/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:13:32 by vlad              #+#    #+#             */
/*   Updated: 2026/01/01 22:42:37 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "mlx.h"
# include "data.h"
# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

# define SUCCESS 0
# define ERROR 1
# define FAIL -1
# define TRUE 1
# define FALSE 0

//
// ------ UTILS ------
//
int					ft_count_words_sep(char *str, char sep);
int					ft_atoi_hexa(char *str);
int					ft_absolute(int n);
int					ft_direction(int x1, int x2);
void				my_mlx_pixel_put(t_fdf *data, t_pixel pxl, int color);
int					ft_close_window(t_fdf *data);
t_fdf				*ft_init_data(t_map *map, t_camera *camera, t_maths *maths);
t_bresenham			ft_init_graphics(t_point p1, t_point p2);
void				*ft_free_tab(char **tab);
void				*ft_free_int_tab(int **tab);
void				*ft_free_map(t_map *map);
void				*ft_free_data(t_fdf *data);
int					ft_error(char *str);

//
// ------ RENDER ------
//
int					ft_process_fdf(t_map *map);
void				ft_transform(t_point *p, t_fdf *data);
void				ft_render(t_fdf *data);
void				ft_events(t_fdf *data);
void				ft_iso_project(t_point *p, t_maths *maths);
void				ft_parallel_project(t_fdf *data, float angle_x, \
						float angle_y);


//
// ------ PARSING ------
//
int					ft_check_filename(const char *filename);
int					ft_get_dimensions(t_map *map, int fd);
int					ft_get_grid(t_map *map, int fd);
t_map				*ft_parse_map(const char *filename);

#endif