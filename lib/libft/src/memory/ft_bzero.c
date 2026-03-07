/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:31:07 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/19 04:02:16 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;
	unsigned long	*l;

	p = (unsigned char *)s;
	while (n > 0 && (size_t)p % sizeof(unsigned long))
	{
		*p++ = 0;
		n--;
	}
	l = (unsigned long *)p;
	while (n >= sizeof(unsigned long))
	{
		*l++ = 0;
		n -= sizeof(unsigned long);
	}
	p = (unsigned char *)l;
	while (n > 0)
	{
		*p++ = 0;
		n--;
	}
}
