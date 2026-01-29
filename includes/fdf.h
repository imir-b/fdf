/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:13:32 by vlad              #+#    #+#             */
/*   Updated: 2026/01/29 03:47:42 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "mlx.h"
# include "data.h"
# include "fbx.h"
# include <math.h>
# include <pthread.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

// **************** DEBUG *****************-------------------------------------
# include <stdio.h>

# define SUCCESS 0
# define ERROR 1
# define FAIL -1
# define TRUE 1
# define FALSE 0

// -----------------------------------------------------------------------------
// UTILS
// -----------------------------------------------------------------------------
int					ft_count_words_sep(char *str, char sep);
int					ft_atoi_hexa(char *str);
double				ft_atof(const char *str);
long long			ft_atoll(const char *s);
int					ft_absolute(int n);
int					ft_direction(int x1, int x2);
void				my_mlx_pixel_put(t_fdf *data, int x, int y, int color);
int					ft_close_window(t_fdf *data);
t_fdf				*ft_init_data(t_object *obj, t_camera *camera);
t_bresenham			ft_init_graphics(t_point p1, t_point p2);
void				*ft_free_tab(char **tab);
void				*ft_free_int_tab(int **tab);
void				*ft_free_object(t_object *obj);
void				*ft_free_data(t_fdf *data);
int					ft_error(char *str);

// -----------------------------------------------------------------------------
// RENDER
// -----------------------------------------------------------------------------
int					ft_process_fdf(t_object *obj);
void				ft_project_point(t_vec3 *v, t_fdf *data);
void				ft_transform_axis_point(double *x, double *y, double *z, \
						t_fdf *data);
void				ft_render_image(t_fdf *data);
void				ft_iso_project(double *x, double *y, double *z);
void				ft_parallel_project(t_fdf *data, float angle_x, \
						float angle_y);
void				ft_draw_axes(t_fdf *data);
void				ft_draw_threads(t_fdf *data);
void				ft_draw_line(t_fdf *data, t_point p1, t_point p2, int color);

// -----------------------------------------------------------------------------
// EVENTS
// -----------------------------------------------------------------------------
void				ft_events(t_fdf *data);
int					ft_mouse_down(int button, int x, int y, t_fdf *data);
int					ft_mouse_up(int button, int x, int y, t_fdf *data);
int					ft_mouse_move(int x, int y, t_fdf *data);

// -----------------------------------------------------------------------------
// PARSING
// -----------------------------------------------------------------------------
int					ft_check_filename(const char *filename);
t_object			*ft_parse_dispatch(const char *filename);
t_object			*ft_parse_fdf(const char *filename, t_object *obj);
t_object			*ft_parse_obj(const char *filename, t_object *obj);
t_fbx				*ft_parse_fbx(const char *filename, t_object *obj);
int					ft_parse_objects(t_fbx *fbx_data, int fd);
int					ft_parse_connections(t_fbx *fbx_data, int fd);
t_geometry			*ft_get_geometry(char *cursor, int fd);
t_model				*ft_get_model(char *cursor, int fd);
t_anim_curve		*ft_get_anim_curve(char *cursor, int fd);
t_anim_node			*ft_get_anim_node(char *cursor, int fd);
char				*ft_skip_spaces(char *str);
void				ft_skip_to_content(char **cursor);
void				ft_move_cursor(char **cursor);
int					ft_extract_line(char **cursor, char **line, int fd);
void				*ft_free_fbx_data(t_fbx *data);
void				*ft_free_geo(t_geometry *geo);

#endif