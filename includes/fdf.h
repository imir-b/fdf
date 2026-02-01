/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:13:32 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 23:54:45 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "mlx.h"
# include "data.h"
# include <math.h>
# include <pthread.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/time.h>
# include <stdbool.h>

// **************** DEBUG ****************
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
long				ft_atol(const char *s);
long long			ft_atoll(const char *s);
int					ft_absolute(int n);
int					ft_direction(int x1, int x2);
void				my_mlx_pixel_put(t_fdf *data, int x, int y, int color);
int					ft_close_window(t_fdf *data);
t_fdf				*ft_init_data(t_object *obj, t_camera *camera, t_fbx *fbx);
t_bresenham			ft_init_graphics(t_point p1, t_point p2);
void				*ft_free_tab(char **tab);
void				*ft_free_int_tab(int **tab);
void				*ft_free_object(t_object *obj);
void				*ft_free_data(t_fdf *data);
int					ft_error(char *str);

// -----------------------------------------------------------------------------
// *** RENDER ***
// -----------------------------------------------------------------------------
int					ft_process_fdf(t_object *obj, t_fbx *fbx);
void				ft_project_point(t_vec3 *v, t_fdf *data);
void				ft_transform_axis_point(double *x, double *y, double *z, \
						t_fdf *data);
void				ft_render_image(t_fdf *data);
void				ft_iso_project(double *x, double *y, double *z);
void				ft_parallel_project(t_fdf *data, float angle_x, \
						float angle_y);
void				ft_draw_axes(t_fdf *data);
void				ft_draw_threads(t_fdf *data);
void				ft_draw_line(t_fdf *data, t_point p1, t_point p2, \
									int color);
long 				ft_get_time_ms(void);

// -----------------------------------------------------------------------------
// *** USER INTERFACE ***
// -----------------------------------------------------------------------------
int					ft_display_fps(t_fdf *data);
int					ft_display_anim_menu(t_fdf *data);

// -----------------------------------------------------------------------------
// *** EVENTS ***
// -----------------------------------------------------------------------------
void				ft_events(t_fdf *data);
int					ft_mouse_down(int button, int x, int y, t_fdf *data);
int					ft_mouse_up(int button, int x, int y, t_fdf *data);
int					ft_mouse_move(int x, int y, t_fdf *data);

// -----------------------------------------------------------------------------
// *** PARSING ***
// -----------------------------------------------------------------------------
int					ft_check_filename(const char *filename);
t_object			*ft_parse_dispatch(const char *filename, t_fbx **fbx);
t_object			*ft_parse_fdf(const char *filename, t_object *obj);
t_object			*ft_parse_obj(const char *filename, t_object *obj);
t_fbx				*ft_parse_fbx(const char *filename);
t_object			*ft_convert_fbx_to_object(t_fbx *fbx);
char				*ft_skip_spaces(char *str);
void				ft_jump_to_next_value(char **cursor, char **line, int fd);
void				ft_skip_to_content(char **cursor);
void				ft_skip_closing_brace(char **cursor, char **line, int fd);
void				ft_move_cursor(char **cursor);
int					ft_extract_line(char **cursor, char **line, int fd);
void				*ft_get_by_id(t_list *list, long id);
int					ft_parse_objects(t_fbx *fbx_data, int fd);
int					ft_parse_connections(t_fbx *fbx_data, int fd);
void				*ft_free_fbx_data(t_fbx *data);
int					ft_parse_face(t_object *obj, char **line, char *cursor, int fd);
t_geometry			*ft_get_geometry(char *cursor, int fd);
void				*ft_free_geo(t_geometry *geo);
t_model				*ft_get_model(char *cursor, int fd);
t_anim_curve		*ft_get_anim_curve(char *cursor, int fd);
t_anim_node			*ft_get_anim_node(char *cursor, int fd);
t_anim_stack		*ft_get_anim_stack(char *cursor, int fd);
t_anim_layer		*ft_get_anim_layer(char *cursor, int fd);

#endif