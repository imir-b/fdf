/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:10 by vlad              #+#    #+#             */
/*   Updated: 2025/12/30 11:41:54 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int ac, char **av)
{
	const char	*filename;
	t_map		*map;

	if (ac != 2)
		return (ft_error("This program needs 2 args"));
	filename = (const char *)av[1];
	if (ft_check_filename(filename))
		return (ft_error("File name is not valid"));
	map = ft_parse_map(filename);
	if (!map)
		return (ft_error("Parsing map failed"));
	ft_process_map(map);
	ft_free_grid(map);
	free(map);
	return (SUCCESS);
}
