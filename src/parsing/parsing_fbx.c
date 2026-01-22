/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fbx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 22:36:04 by vbleskin          #+#    #+#             */
/*   Updated: 2026/01/22 07:26:51 by vbleskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	if (data->anim_curve)
		free(data->anim_curve);
	if (data->anim_node)
		free(data->anim_node);
	if (data)
		free(data);
	return (NULL);
}

void	ft_skip_spaces(char **str)
{
	while ((**str >= 9 && **str <= 13) || **str == ' ')
		(*str)++;

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

t_geometry	*ft_get_geometry(char *line, t_object *obj, int fd)
{
	t_geometry	*geo;

	geo = malloc(sizeof(t_geometry));
	if (!geo)
		return (NULL);
	geo->id = ft_atoi(line[10]);
	free(line);
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line) // || ft_strchr(line, '}')
			break ;
		ft_skip_spaces(&line);
		if (!ft_strncmp(line, "Verticies:", 10))
			ft_parse_vertex(obj, line);
		else if (!ft_strncmp(line, "PolygonVertexIndex:", 19))
			ft_parse_face(obj, line);
		free(line);
	}
	obj->height = 0;
	obj->width = 0;
	geo->obj = obj;
	return (geo);
}

t_anim_curve	*ft_get_anim_curve(char *line, int fd)
{
	t_anim_curve	*anim_curve;
	int				index;

	anim_curve = malloc(sizeof(t_anim_curve));
	if (!anim_curve)
		return (NULL);
	anim_curve->id = ft_atoi(line[15]);
	anim_curve->n_keys = ft_atoi();
	index = 0;
	while (index < anim_curve->n_keys)
		anim_curve->time[index++] = ft_atoll();
	index = 0;
	while (index < anim_curve->n_keys)
		anim_curve->value[index++] = ft_atof();
}

t_anim_node	*ft_get_anim_node(char *line, int fd)
{
	t_anim_node	*anim_node;

	anim_node = malloc(sizeof(t_anim_node));
	if (!anim_node)
		return (NULL);
	anim_node->id = ft_atoi(line[19]);
	anim_node->type = 
}

t_model	*ft_get_model(char *line, int fd)
{
	t_model	*model;

	model = malloc(sizeof(t_model));
	if (!model)
		return (NULL);
	model->id = ft_atoi(line[6]);
	model->parent = NULL;
}

int	ft_parse_objects(t_fbx *fbx_data, t_object *obj, int fd)
{
	char	*line;

	while (TRUE) // comment savoir que je suis a la fin de la partie "Objects:" ?
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		else if (!ft_strncmp(line, "Geometry:", 10))
			fbx_data->geo = ft_get_geometry(line, obj, fd);
		else if (!ft_strncmp(line, "Model:", 6))
			fbx_data->model = ft_get_model(line, fd);
		else if (!ft_strncmp(line, "AnimationCurve:", 15))
			fbx_data->anim_curve = ft_get_anim_curve(line, fd);
		else if (!ft_strncmp(line, "AnimationCurveNode:", 19))
			fbx_data->anim_node = ft_get_anim_node(line, fd);
		free(line);
	}
	return (SUCCESS);
}

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
		else if (!ft_strncmp(line, "Objects:", 8))
			ft_parse_objects(fbx_data, obj, fd);
		else if (!ft_strncmp(line, "Connections:", 12))
			ft_parse_connections(fbx_data, fd);
		free(line);
	}
	return (obj);
}
