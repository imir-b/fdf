/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:13:32 by vlad              #+#    #+#             */
/*   Updated: 2025/12/25 23:29:08 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
#define FDF_H

# include "libft.h"
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
    int width;
    int height;
    int **grid;
}   t_map;

#endif