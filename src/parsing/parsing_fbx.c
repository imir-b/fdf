/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/24 15:11:04 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define IS_TAG(str, tag) (!ft_strncmp(str, tag, sizeof(tag) - 1))

typedef struct s_geometry
{
	long		id;
	t_object	*obj;
	t_geometry	*next;
}	t_geometry;

typedef struct s_properties
{
	char	type; // (T, R, S, V....)
	double	x;
	double	y;
	double	z;
}	t_properties;

typedef struct s_model
{
	long			id;
	t_properties	pos;
	t_properties	rot;
	t_properties	scale;
	t_anim_node		*anim_pos;
	t_anim_node		*anim_rot;
	t_anim_node		*anim_scale;
	t_geometry		*geo;
	t_model			*parent;
	t_model			*next;
}	t_model;

typedef struct s_anim_curve
{
	long			id;
	int				n_keys;
	long long		*time;
	double			*value;
	t_anim_curve	*next;
}	t_anim_curve;

typedef struct s_anim_node
{
	long			id;
	char			type;
	t_anim_curve	*x;
	t_anim_curve	*y;
	t_anim_curve	*z;
	t_anim_node		*next;
}	t_anim_node;

typedef struct s_fbx
{
	t_geometry		*geo;
	t_model			*model;
	t_anim_curve	*anim_curve;
	t_anim_node		*anim_node;
}	t_fbx;

/*modifier la fonction pour free tous les elements de la liste chainee*/
void	*ft_free_fbx_data(t_fbx *data)
{
	if (data->geo)
		free(data->geo);
	if (data->model)
		free(data->model);
	if (data->anim_curve->time)
		free(data->anim_curve->time);
	if (data->anim_curve->value)
		free(data->anim_curve->value);
	if (data->anim_curve)
		free(data->anim_curve);
	if (data->anim_node)
		free(data->anim_node);
	if (data)
		free(data);
	return (NULL);
}

char	*ft_skip_spaces(char *str)
{
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	return (str);
}

static int	ft_parse_vertex(t_object *obj, char *line)
{
	int		index;
	char	**split;

	obj->nb_vertices = ft_atoi(line[10]);
	split = ft_split(line, ',');
	index = 0;
	if (split)
	{
		obj->vertices[index].x = ft_atof(split[1]);
		obj->vertices[index].y = ft_atof(split[2]);
		obj->vertices[index].z = ft_atof(split[3]);
		obj->vertices[index].color = WHITE;
		obj->vertices[index].sx = 0;
		obj->vertices[index].sy = 0;
		index++;
	}
	ft_free_tab(split);
}

static int	ft_parse_face(t_object *obj, char *line)
{
	obj->nb_faces = ft_atoi(line[19]);
}

t_geometry	*ft_get_geometry(char *cursor, t_object *obj, int fd)
{
	char		*line;
	char		*cursor;
	t_geometry	*geo;

	geo = malloc(sizeof(t_geometry));
	if (!geo)
		return (NULL);
	geo->id = ft_atoi(cursor);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line || *line == '}')
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "Verticies:"))
			ft_parse_vertex(obj, cursor);
		else if (IS_TAG(cursor, "PolygonVertexIndex:"))
			ft_parse_face(obj, cursor);
		free(line);
	}
	obj->height = 0;
	obj->width = 0;
	geo->obj = obj;
	geo->next = NULL;
	return (geo);
}

int	ft_parse_keytime(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	cursor = ft_strchr(cursor, '*') + 1;
	anim_curve->n_keys = ft_atoi(cursor);
	anim_curve->time = malloc(sizeof(long long) * anim_curve->n_keys);
	if (!anim_curve->time)
		return (ERROR);
	index = 0;
	cursor = ft_strchr(cursor, '{') + 1;
	while (index < anim_curve->n_keys)
		anim_curve->time[index++] = ft_atoll(cursor);
	return (SUCCESS);
}

int	ft_parse_keyvalue(t_anim_curve *anim_curve, char *cursor, int fd)
{
	int		index;
	char	*line;

	anim_curve->value = malloc(sizeof(double) * anim_curve->n_keys);
	if (!anim_curve->value)
		return (ERROR);
	index = 0;
	cursor = ft_strchr(cursor, '{') + 1;
	while (index < anim_curve->n_keys)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		//incrementer cursor ici dans une boucle tant que cursor est pas fin de ligne ou acolade de fin
			anim_curve->value[index++] = ft_atof(cursor);
	}
	return (SUCCESS);
}

t_anim_curve	*ft_get_anim_curve(char *cursor, int fd)
{
	t_anim_curve	*anim_curve;
	char			*line;

	anim_curve = malloc(sizeof(t_anim_curve));
	if (!anim_curve)
		return (NULL);
	anim_curve->id = ft_atoi(cursor);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "KeyTime:"))
			ft_parse_keytime(anim_curve, cursor, fd);
		else if (IS_TAG(cursor, "KeyValueFloat:"))
			ft_parse_keyvalue(anim_curve, cursor, fd);
		free(line);
	}
	return (anim_curve);
}

void	ft_extract_type(char **cursor)
{
	*cursor = ft_strrchr(*cursor, ',');
	*cursor = ft_skip_spaces(*cursor);
	if (*cursor)
		*cursor++;
	return (*cursor);
}

t_anim_node	*ft_get_anim_node(char *cursor, int fd)
{
	t_anim_node	*anim_node;
	char		*line;

	anim_node = malloc(sizeof(t_anim_node));
	if (!anim_node)
		return (NULL);
	anim_node->id = ft_atoi(cursor);
	cursor = ft_extract_type(&cursor);
	anim_node->type = *cursor;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "Properties70:"))
			//parsing
		free(line);
	}
	return (anim_node);
}

/**
 * Fonction de parsing qui recupere 'type' donne en argument et
 * les dernieres valeurs 'x', 'y' et 'z' dans 'line'.
 * Exemple de ligne :
 * 'P: "Nom", "Type", "Label", "Flags", x, y, z'
 */
void	ft_extract_property_values(t_properties *property, char *line, char type)
{
	int	i;

	property->type = type;
	i = 0;
	while (i < 4)
	{
		line = ft_strchr(line, ',');
		if (!line)
			return ;
		line++;
		i++;
	}
	property->x = ft_atof(line);
	line = ft_strchr(line, ',');
	line = ft_skip_spaces(line);
	if (*line)
		line++;
	property->y = ft_atof(line);
	line = ft_strchr(line, ',');
	line = ft_skip_spaces(line);
	if (*line)
		line++;
	property->z = ft_atof(line);
}

/**
 * Fonction de parsing pour lire les proprietes d'un model
 * Exemple de format :
 * 	Properties70:  {
 * 		...
 * 	}
 */
void	ft_parse_properties(t_model *model, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
			cursor = ft_skip_spaces(line);
		if (*cursor == '}')
			return (free(line));
		if (IS_TAG(cursor, "P:"))
		{
			if (ft_strnstr(cursor, "\"Lcl Translation\"", 50))
				ft_extract_property_values(&model->pos, cursor, 'T');
			else if (ft_strnstr(cursor, "\"Lcl Rotation\"", 50))
				ft_extract_property_values(&model->rot, cursor, 'R');
			else if (ft_strnstr(cursor, "\"Lcl Scaling\"", 50))
				ft_extract_property_values(&model->scale, cursor, 'S');
		}
		free(line);
	}
}

t_model	*ft_get_model(char *cursor, int fd)
{
	char	*line;
	t_model	*model;

	model = malloc(sizeof(t_model));
	if (!model)
		return (NULL);
	model->id = ft_atoi(cursor);
	model->parent = NULL;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		cursor = ft_skip_spaces(line);
		if (*cursor == '}');
			return (free(line), model);
		if (IS_TAG(cursor, "Properties70:"))
			ft_parse_properties(model, fd);
		free(line);
	}
}

/**
 * Fonction de parsing pour recuperer les donnees dans la partie 'Objects'
 * d'un fichier .fbx
 */
int	ft_parse_objects(t_fbx *fbx_data, t_object *obj, int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line || line[0] == '}')
			break ;
		cursor = ft_skip_spaces(line);
		if (IS_TAG(cursor, "Geometry:"))
			fbx_data->geo = ft_get_geometry(cursor + 9, obj, fd);
		else if (IS_TAG(cursor, "Model:"))
			fbx_data->model = ft_get_model(cursor + 6, fd);
		else if (IS_TAG(cursor, "AnimationCurve:"))
			fbx_data->anim_curve = ft_get_anim_curve(cursor + 15, fd);
		else if (IS_TAG(cursor, "AnimationCurveNode:"))
			fbx_data->anim_node = ft_get_anim_node(cursor + 19, fd);
		free(line);
	}
	return (SUCCESS);
}

/**
 * Fonction de parsing pour recuperer les donnes dans la partie 'Connections'
 * d'un fichier .fbx
 */
int	ft_parse_connections(t_fbx *fbx_data, int fd)
{
	return (SUCCESS);
}

t_object	*ft_parse_fbx(const char *filename, t_object *obj)
{
	int		fd;
	char	*line;
	t_fbx	*fbx_data;

	fd = open(filename, O_RDONLY);
	if (fd == FAIL)
		return (ft_free_object(obj));
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		else if (IS_TAG(line, "Objects:"))
			ft_parse_objects(fbx_data, obj, fd);
		else if (IS_TAG(line, "Connections:"))
			ft_parse_connections(fbx_data, fd);
		free(line);
	}
	return (obj);
}
