/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:17:30 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/01 19:41:47 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	ft_check_filename(const char *filename)
{
	char	*format;
	int		format_len;
	int		len;

	format = ft_strrchr(filename, '.');
	if (!format)
		return (ERROR);
	len = ft_strlen(filename);
	format_len = ft_strlen((const char *)format);
	if (!ft_strncmp(format, ".fdf", 4) && format_len == 4 && format_len < len)
		return (SUCCESS);
	else
		return (ERROR);
}

int	ft_get_dimensions(t_map *map, int fd)
{
	char	*line;
	int		count;

	map->height = 0;
	map->width = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		map->height++;
		count = ft_count_words_sep(line, ' ');
		if (count == FAIL)
			return (free(line), ERROR);
		if (!map->width)
			map->width = count;
		if (map->width != count)
			return (free(line), ERROR);
		free(line);
	}
	return (SUCCESS);
}

int	ft_get_grid(t_map *map, int fd)
{
	int		height;
	int		width;
	char	*line;
	char	**grid;
	char	*comma;

	height = 0;
	while (height < map->height)
	{
		line = get_next_line(fd);
		if (!line)
			return (ERROR);
		grid = ft_split(line, ' ');
		free(line);
		if (!grid)
			return (ERROR);
		map->grid[height] = malloc(sizeof(int) * map->width);
		map->colors[height] = malloc(sizeof(int) * map->width);
		if (!map->grid[height] || !map->colors[height])
			return (ERROR);
		width = 0;
		while (width < map->width)
		{
			map->grid[height][width] = ft_atoi(grid[width]);
			comma = ft_strchr(grid[width], ',');
			if (comma)
				map->colors[height][width] = ft_atoi_hexa(comma + 1);
			else
				map->colors[height][width] = WHITE;
			width++;
		}
		ft_free_tab(grid);
		height++;
	}
	return (SUCCESS);
}

t_map	*ft_parse_map(const char *filename)
{
	int		fd;
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_map(map));
	if (ft_get_dimensions(map, fd))
		return (close(fd), ft_free_map(map));
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), ft_free_map(map));
	map->grid = malloc(sizeof(int *) * map->height);
	if (!map->grid)
		return (close(fd), ft_free_map(map));
	map->colors = malloc(sizeof(int *) * map->height);
	if (!map->colors)
		return (close(fd), ft_free_map(map));
	if (ft_get_grid(map, fd))
		return (close(fd), ft_free_map(map));
	close(fd);
	return (map);
}
