/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/31 00:39:20 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_object *ft_convert_fbx_to_object(t_fbx *fbx)
{
t_object    *big_obj;
    t_geometry  *geo;
    t_list      *curr; // Curseur pour parcourir la liste générique
    int         total_v;
    int         total_f;

    // ---------------------------------------------------------
    // ÉTAPE 1 : Calculer la taille totale (Sommets et Faces)
    // ---------------------------------------------------------
    total_v = 0;
    total_f = 0;
    curr = fbx->geo; // On commence au début de la liste
    while (curr)
    {
        geo = (t_geometry *)curr->content; // On récupère la géométrie
        if (geo && geo->obj)
        {
            total_v += geo->obj->nb_vertices;
            total_f += geo->obj->nb_faces;
        }
        curr = curr->next;
    }

    // ---------------------------------------------------------
    // ÉTAPE 2 : Allouer le Mega-Objet
    // ---------------------------------------------------------
    big_obj = ft_calloc(1, sizeof(t_object));
    if (!big_obj) return (NULL);
    
    big_obj->nb_vertices = total_v;
    big_obj->nb_faces = total_f;
    big_obj->vertices = malloc(sizeof(t_vec3) * total_v);
    big_obj->faces = malloc(sizeof(t_face) * total_f);
    
    if (!big_obj->vertices || !big_obj->faces)
        return (ft_free_object(big_obj), NULL); // Fonction de free à adapter

    // ---------------------------------------------------------
    // ÉTAPE 3 : Copier et Fusionner les données
    // ---------------------------------------------------------
    int v_offset = 0; // Décalage pour les sommets
    int f_offset = 0; // Décalage pour les faces
    
    curr = fbx->geo;
    while (curr)
    {
        geo = (t_geometry *)curr->content;
        if (geo && geo->obj)
        {
            // A. Copie des sommets
            int i = 0;
            while (i < geo->obj->nb_vertices)
            {
                big_obj->vertices[v_offset + i] = geo->obj->vertices[i];
                i++;
            }

            // B. Copie des faces (avec ajustement des indices !)
            int j = 0;
            while (j < geo->obj->nb_faces)
            {
                // Copie de la structure de la face
                big_obj->faces[f_offset + j].count = geo->obj->faces[j].count;
                
                // Allocation des indices pour la nouvelle face
                big_obj->faces[f_offset + j].indices = malloc(sizeof(int) * geo->obj->faces[j].count);
                // (Ajouter sécu malloc ici si nécessaire)

                // Copie et Décalage des indices
                int k = 0;
                while (k < geo->obj->faces[j].count)
                {
                    // L'ancien index (ex: 0) devient (0 + nombre de sommets précédents)
                    big_obj->faces[f_offset + j].indices[k] = \
                        geo->obj->faces[j].indices[k] + v_offset;
                    k++;
                }
                j++;
            }
            
            // Mise à jour des offsets pour le prochain tour
            v_offset += geo->obj->nb_vertices;
            f_offset += geo->obj->nb_faces;
        }
        curr = curr->next;
    }
    
    return (big_obj);
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
t_object	*ft_parse_fbx(const char *filename, t_object *obj)
{
	int		fd;
	char	*line;
	t_fbx	*fbx_data;

	fbx_data = ft_calloc(1, sizeof(t_fbx));
	if (!fbx_data)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (free(fbx_data), ft_free_object(obj));
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
	printf("parsing de fbx carre\n"); //debug
	obj = ft_convert_fbx_to_object(fbx_data);
	return (obj);
}
