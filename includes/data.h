/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlad <vlad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:15 by vbleskin          #+#    #+#             */
/*   Updated: 2026/02/15 16:19:16 by vlad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# include <stdbool.h>

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

# define RADIAN_30 0.523599
# define RADIAN_90 1.5708

# define COS_30 0.8660254
# define SIN_30 0.5

# define ISOMETRIC 0
# define PARALLEL 1

# define WHITE 16777215

# define THREADS_NB 4

# define FBX_SEC 46186158000.0

# define M_PI 3.14159265358979323846

# define IS_TAG(str, tag) (!ft_strncmp(str, tag, sizeof(tag) - 1))

/*
 * Structure simple représentant un point avec des coordonnées
 * entières en 3D (souvent utilisé pour les coordonnées grille).
 */
typedef struct s_point			t_point;

/*
 * Vecteur 3D de haute précision (double) utilisé pour les calculs.
 * Contient aussi les coordonnées projetées à l'écran (sx, sy) et la couleur.
 */
typedef struct s_vec3			t_vec3;

typedef struct s_matrix			t_matrix;

/*
 * Définit la topologie d'une face (polygone) via une liste d'indices
 * renvoyant vers le tableau de sommets de l'objet.
 */
typedef struct s_face			t_face;

/*
 * Représente un objet 3D complet (Mesh) : contient le tableau
 * de sommets (vertices) et le tableau de faces.
 */
typedef struct s_object			t_object;

/*
 * Gère les paramètres de la vue et de la caméra : décalages (pan),
 * zoom, écrasement Z (z_scale), type de projection et rotations.
 */
typedef struct s_camera			t_camera;

/*
 * Cache pour les valeurs trigonométriques précalculées (sin/cos)
 * afin d'optimiser les performances lors des rotations répétées.
 */
typedef struct s_maths			t_maths;

/*
 * Variables de travail pour l'algorithme de tracé de ligne de Bresenham
 * (différences, pas, erreur cumulée).
 */
typedef struct s_bresenham		t_bresenham;

/*
 * Stocke l'état actuel de la souris : position (x, y)
 * et état du bouton (enfoncé ou non).
 */
typedef struct s_mouse			t_mouse;

/*
 * Gestion du temps pour assurer une animation fluide indépendante du framerate.
 * Calcule le Delta Time et stocke les temps de référence.
 */
typedef struct s_timer			t_timer;

/*
 * Wrapper pour les images de la MiniLibX (MLX).
 * Contient le pointeur d'image, l'adresse des pixels et les métadonnées.
 */
typedef struct s_img			t_img;

/*
 * Structure principale (God Struct) regroupant tout l'état de l'application :
 * pointeurs MLX, image, modèle mathématique, objet, caméra, FBX, inputs, etc.
 */
typedef struct s_fdf			t_fdf;

/*
 * Contexte pour le multithreading.
 * Définit la portion de travail (start/end) attribuée à un thread spécifique.
 */
typedef struct s_thread			t_thread;


// -----------------------------------------------------------------------------
// ****** Data parsing fbx ******
// -----------------------------------------------------------------------------
/*
 * Associe un identifiant unique à un objet 3D (maillage)
 * utilisé dans le parsing FBX.
 */
typedef struct s_geometry		t_geometry;

/*
 * Stocke les propriétés de transformation (Position, Rotation, Scale)
 * sous forme de coordonnées 3D génériques.
 */
typedef struct s_properties		t_properties;

/*
 * Représente un nœud dans le graphe de scène (Scene Graph).
 * Contient les transformations locales, les liens vers les animations,
 * la géométrie associée et la référence vers le parent hiérarchique.
 */
typedef struct s_model			t_model;

typedef struct s_deformer		t_deformer;

/*
 * Courbe d'animation brute définissant l'évolution d'une valeur
 * au fil du temps via des keyframes (temps et valeur).
 */
typedef struct s_anim_curve		t_anim_curve;

/*
 * Nœud d'animation regroupant les courbes indépendantes
 * pour les axes X, Y et Z d'une propriété donnée.
 */
typedef struct s_anim_node		t_anim_node;

/*
 * Couche d'animation contenant une liste de nœuds animés,
 * permettant de grouper plusieurs animations ensemble.
 */
typedef struct s_anim_layer		t_anim_layer;

/*
 * Pile d'animation (ou 'Take') représentant une séquence complète
 * (par exemple : "Idle", "Walk", "Run").
 */
typedef struct s_anim_stack		t_anim_stack;

/*
 * Structure racine pour les données importées via FBX.
 * Contient toutes les listes chaînées des éléments parsés
 * (géométries, modèles, courbes, stacks, etc.).
 */
typedef struct s_fbx			t_fbx;

struct s_geometry
{
	long		id;
	t_object	*obj;
	t_list		*deformers;
};

struct s_properties
{
	char	type;
	double	x;
	double	y;
	double	z;
};

struct s_model
{
	long			id;
	t_properties	pos;
	t_properties	rot;
	t_properties	scale;
	t_properties	base_pos;
	t_properties	base_rot;
	t_properties	base_scale;
	t_anim_node		*anim_pos;
	t_anim_node		*anim_rot;
	t_anim_node		*anim_scale;
	t_geometry		*geo;
	t_model			*parent;
};

struct	s_deformer
{
	long		id;
	int			n_vertices;
	int			*verticies;
	double		*weights;
	double		*transform;
	double		*t_link;
	t_model		*bone;
};

struct s_anim_curve
{
	long	id;
	int		n_keys;
	double	*time;
	double	*value;
};

struct s_anim_node
{
	long			id;
	char			type;
	t_model			*target;
	t_anim_curve	*x;
	t_anim_curve	*y;
	t_anim_curve	*z;
};

struct s_anim_layer
{
	long	id;
	t_list	*nodes;
};

struct s_anim_stack
{
	long	id;
	char	*name;
	double	duration;
	t_list	*layers;
};

struct s_fbx
{
	t_list			*geo;
	t_list			*model;
	t_list			*deformer;
	t_list			*anim_curve;
	t_list			*anim_node;
	t_list			*anim_stack;
	t_anim_stack	*current_anim;
	t_list			*anim_layer;
};

struct s_point
{
	int	x;
	int	y;
	int	z;
};

struct s_vec3
{
	double	x;
	double	y;
	double	z;
	int		sx;
	int		sy;
	int		color;
};

struct s_matrix
{
	double	matrix[16];
};

struct s_face
{
	int	*indices;
	int	count;
};

struct s_object
{
	t_vec3	*vertices;
	int		nb_vertices;
	t_face	*faces;
	int		nb_faces;
	int		width;
	int		height;
};

struct s_camera
{
	int		shift_x;
	int		shift_y;
	double	zoom;
	float	z_scale;
	int		projection;
	double	angle_x;
	double	angle_y;
};

struct s_maths
{
	float	sin_alpha;
	float	cos_alpha;
	float	sin_beta;
	float	cos_beta;
};

struct s_bresenham
{
	int		diff_x;
	int		diff_y;
	int		step_x;
	int		step_y;
	int		err;
};

struct s_mouse
{
	int	x;
	int	y;
	int	is_pressed;
};

struct s_timer {
    long    start_time;
    long    last_frame;
    double  delta_time;
    double	weighted_value;
    double  duration;
	bool	is_paused;
};

struct s_img
{
	void	*ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
};

struct s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	t_maths		trigo;
	t_object	*object;
	t_camera	*camera;
	t_fbx		*fbx;
	t_mouse		mouse;
	t_timer		timer;
};

struct s_thread
{
	t_fdf	*data;
	int		id;
	int		start;
	int		end;
};

#endif