/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:10 by vlad              #+#    #+#             */
/*   Updated: 2025/12/29 13:46:20 by vbleskin         ###   ########.fr       */
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

int	ft_error(char *str)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ERROR);
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

int	ft_get_dimensions(t_map *map, int fd)
{
	char	*line;
	int		count;

	map->height = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		map->height++;
		count = ft_count_digits(line);
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

/*
ou alors faire une fonction count qui prend un char separateur et une fonction 
ft_is* (alpha, digit...)
*/
int	ft_count_digits(char *str)
{
	
}

/*
alloue un tab concatene de tab1 et tab2 et free tab1, si pas de tab1 on ret tab2
*/
char	**ft_tabjoin(char **tab1, char **tab2)
{
	char	**ret;
	int		len1;
	int		len2;
	int		i;

	if (!tab1)
		return (tab2);
	len1 = 0;
	len2 = 0;
	i = 0;
	while (tab1[len1])
		len1++;
	while (tab2[len2])
		len2++;
	ret = malloc(sizeof(char *) * (len1 + len2 + 1));
	if (!ret)
		return (NULL);
	while (*tab1)
		ret[i++] = tab1++;
	while (*tab2)
		ret[i++] = tab2++;
	ret[i] = NULL;
	return (free(tab1), tab1 = NULL, ret);
}

char	**ft_get_grid(int fd)
{
	char	*line;
	char	**tmp;
	char	**grid;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		tmp = ft_split(line, ' ');
		if (!tmp)
			return (free(line), NULL);
		free(line);
		grid = ft_tabjoin(grid, tmp);
		if (!grid)
			return (ft_free_tab(tmp), NULL);
		ft_free_tab(tmp);
	}
	return (grid);
}

t_map	*ft_parse_map(const char *filename)
{
	char	**grid;
	int		fd;
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), NULL);
	if (ft_get_dimensions(map, fd))
		return (close(fd), NULL);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_error(strerror(errno)), NULL);
	grid = ft_get_grid(fd);
	if (!grid)
		return (close(fd), NULL);
	close(fd);
	map->grid = strs_to_ints(grid);
	return (map);
}

int main(int ac, char **av)
{
	const char	*filename;
	int			fd;
	t_map		*map;

    if (ac != 2)
        return (ft_error("This program needs 2 args"));
	filename = (const char *)av[1];
	if (ft_check_filename(filename))
		return (ft_error("File name is not valid"));
	map = ft_parse_map(fd);
	if (!map)
		return (ft_error("Parsing map failed"));
	close(fd);
	ft_render_map(map);
	free(map);
    return (SUCCESS);
}
