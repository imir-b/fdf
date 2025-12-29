/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:20:18 by vbleskin          #+#    #+#             */
/*   Updated: 2025/12/30 00:19:50 by vbleskin         ###   ########.fr       */
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
