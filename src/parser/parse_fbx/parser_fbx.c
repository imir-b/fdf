/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fbx.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/27 14:19:35 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** Lit l'intégralité d'un fichier dans un buffer alloué dynamiquement.
** Calcule la taille totale et la stocke dans le pointeur total.
*/
static char	*ft_read_entire_file(const char *filename, ssize_t *total)
{
	int		fd;
	char	*buf;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	*total = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	if (*total <= 0)
		return (close(fd), NULL);
	buf = malloc(*total);
	if (!buf)
		return (close(fd), NULL);
	if (read(fd, buf, *total) != *total)
		return (close(fd), free(buf), NULL);
	close(fd);
	return (buf);
}

/*
** Remplace les octets nuls par des espaces, écrit le contenu nettoyé
** dans un fichier temporaire et retourne son descripteur de fichier.
*/
static int	ft_write_and_open_temp(char *buf, ssize_t total)
{
	int		fd_tmp;
	ssize_t	i;

	i = -1;
	while (++i < total)
	{
		if (buf[i] == '\0')
			buf[i] = ' ';
	}
	fd_tmp = open("/tmp/.fdf_clean.fbx", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_tmp < 0)
		return (free(buf), -1);
	if (write(fd_tmp, buf, total) != total)
	{
		close(fd_tmp);
		free(buf);
		return (-1);
	}
	close(fd_tmp);
	free(buf);
	return (open("/tmp/.fdf_clean.fbx", O_RDONLY));
}

/*
** Lit le contenu d'un fichier dans un buffer, remplace les octets NUL
** par des espaces, écrit le contenu nettoyé dans un fichier temporaire,
** et retourne son descripteur de fichier.
** Certains exportateurs FBX intègrent des octets \0 dans les chaînes
** de noms d'objets, ce qui casse les opérations sur les chaînes C
** (comme strchr ou strlen).
** Retourne -1 en cas d'erreur.
*/
static int	ft_open_clean_fbx(const char *filename)
{
	char	*buf;
	ssize_t	total;

	buf = ft_read_entire_file(filename, &total);
	if (!buf)
		return (-1);
	return (ft_write_and_open_temp(buf, total));
}

/**
 * Fonction de post-traitement du parsing FBX.
 * 1. Supprime le fichier temporaire généré pour la lecture propre.
 * 2. Parcourt les stacks d'animation fraîchement parsés pour calculer
 * et assigner la durée totale de chaque séquence.
 */
static inline void	ft_clean_and_finish_parse_fbx(t_fbx *fbx_data)
{
	t_list	*stacks;

	unlink("/tmp/.fdf_clean.fbx");
	{
		stacks = fbx_data->anim_stack;
		while (stacks)
		{
			ft_calculate_anim_duration((t_anim_stack *)stacks->content);
			stacks = stacks->next;
		}
	}
}

/**
 * Lit le fichier 'filename'.fbx pour remplir la structure 'fbx_data'.
 * On separe la lecture en deux parties :
 * 
 * 	Objects:	{
 * 		...
 * 	}
 * 
 * et :
 * 
 * 	Connections:  {
 * 		...
 * 	}
 * 
 */
t_fbx	*ft_parse_fbx(const char *filename)
{
	int		fd;
	char	*line;
	t_fbx	*fbx_data;

	fbx_data = ft_calloc(1, sizeof(t_fbx));
	if (!fbx_data)
		return (NULL);
	fd = ft_open_clean_fbx(filename);
	if (fd == -1)
		return (free(fbx_data), NULL);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		else if (IS_TAG(line, "Objects"))
			ft_parse_objects(fbx_data, fd);
		else if (IS_TAG(line, "Connections"))
			ft_parse_connections(fbx_data, fd);
		free(line);
	}
	close(fd);
	ft_clean_and_finish_parse_fbx(fbx_data);
	return (fbx_data);
}
