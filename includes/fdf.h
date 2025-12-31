/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:13:32 by vlad              #+#    #+#             */
/*   Updated: 2025/12/31 05:42:05 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "keys.h"
# include "matrice.h"
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

# define ERROR 1
# define SUCCESS 0
# define FAIL -1
# define TRUE 1

typedef struct s_map
{
	int	width;
	int	height;
	int	**grid;
}	t_map;

typedef struct s_fdf
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	t_map	*map;
}	t_fdf;

//
// ------ UTILS ------
//
int				ft_count_words_sep(char *str, char sep);
void			*ft_free_tab(char **tab);
void			*ft_free_grid(t_map *map);
void			*ft_free_data(t_fdf *data);
t_fdf			*ft_init_data(t_map *map);
int				ft_error(char *str);

//
// ------ RENDER ------
//
t_fdf			*ft_init_data(t_map *map);
int				ft_process_map(t_map *map);

//
// ------ PARSING ------
//
int				ft_check_filename(const char *filename);
int				ft_get_dimensions(t_map *map, int fd);
int				ft_get_grid(t_map *map, int fd);
t_map			*ft_parse_map(const char *filename);

#endif