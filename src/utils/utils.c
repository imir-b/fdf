/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:18 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/31 05:41:17 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_count_words_sep(char *str, char sep)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (*str && *str != '\n')
		{
			count++;
			while (*str && *str != sep && *str != '\n')
				str++;
		}
		else if (*str == '\n')
			str++;
	}
	return (count);
}

void	*ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (NULL);
}

void	*ft_free_grid(t_map *map)
{
	int	i;

	if (!map->grid)
		return (NULL);
	i = 0;
	while (i < map->height)
		free(map->grid[i++]);
	free(map->grid);
	return (NULL);
}

void	*ft_free_data(t_fdf *data)
{
	if (data->mlx_ptr)
		free(data->mlx_ptr);
	if (data)
	{
		free(data);
		data = NULL;
	}
	return (NULL);
}

t_fdf	*ft_init_data(t_map *map)
{
	t_fdf	*data;

	data = malloc(sizeof(t_fdf));
	if (!data)
		return (NULL);
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (ft_free_data(data));
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, \
		"FDF vbleskin");
	if (!data->win_ptr)
		return (ft_free_data(data));
	data->img_ptr = mlx_new_image(data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	data->addr = mlx_get_data_addr(data->img_ptr, &data->bits_per_pixel, \
		&data->line_length, &data->endian);
	data->map = map;
	return (data);
}
