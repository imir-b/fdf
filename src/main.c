/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:10 by vlad              #+#    #+#             */
/*   Updated: 2026/02/01 03:50:07 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int ac, char **av)
{
	const char	*filename;
	t_object	*object;

	if (ac != 2)
		return (ft_error("This program needs 2 args"));
	filename = (const char *)av[1];
	if (ft_check_filename(filename))
		return (ft_error("File name is not valid"));
	object = ft_parse_dispatch(filename);
	if (!object)
		return (ft_error("Parsing map failed"));
	ft_process_fdf(object);
	ft_free_object(object);
	return (SUCCESS);
}
