# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/29 13:31:53 by vbleskin          #+#    #+#              #
#    Updated: 2026/01/18 01:02:38 by vbleskin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------------------------------------------------------------
# EXE
# ------------------------------------------------------------------------------

NAME			=	fdf

# ------------------------------------------------------------------------------
# DEFINITIONS
# ------------------------------------------------------------------------------

CC				=	cc
CFLAGS			=	-Wall -Werror -Wextra -I includes -I $(LIBFT_DIR)includes -I $(MINLIB_DIR) -o3 -pthread -march=native -ffast-math
MAKEFLAGS 		+=	--no-print-directory
RM				=	rm -rf

# ------------------------------------------------------------------------------
# DIRECTORIES
# ------------------------------------------------------------------------------

SRC_DIR			=	src/
PARS_DIR		=	parsing/
REND_DIR		=	render/
UTIL_DIR		=	utils/
OBJ_DIR			=	obj/
LIB_DIR			=	lib/
LIBFT_DIR		=	$(LIB_DIR)libft/
MINLIB_DIR		=	$(LIB_DIR)minilibx-linux/

# ------------------------------------------------------------------------------
# FILES
# ------------------------------------------------------------------------------

SRC_PARS		=	$(PARS_DIR)parsing.c $(PARS_DIR)parsing_fdf.c $(PARS_DIR)parsing_obj.c
SRC_REND		=	$(REND_DIR)render.c $(REND_DIR)hooks.c $(REND_DIR)transform.c $(REND_DIR)project.c
SRC_UTIL		=	$(UTIL_DIR)utils.c $(UTIL_DIR)error.c $(UTIL_DIR)utils_data.c $(UTIL_DIR)utils_mlx.c
SRC_FILES		=	main.c $(SRC_PARS) $(SRC_REND) $(SRC_UTIL)
SRCS			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS			=	$(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))


# ------------------------------------------------------------------------------
# LIBRARIES
# ------------------------------------------------------------------------------

LIBS			=	-L$(LIBFT_DIR) -lft -L$(MINLIB_DIR) -lmlx_Linux -lXext -lX11 -lm -lz

# ------------------------------------------------------------------------------
# RULES
# ------------------------------------------------------------------------------

all :			$(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CFLAGS) -c $< -o $@
				@echo "Compiling $<"

$(NAME) :		$(OBJS)
				@make -C $(LIBFT_DIR)
				@echo "Libft compiled successfuly"
				@make -C $(MINLIB_DIR) > /dev/null 2>&1
				@echo "Minilibx compiled successfuly"
				@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
				@echo "$(NAME) compiled successfuly"

clean :
				@make clean -C $(LIBFT_DIR)
				@echo "Libft cleaned successfuly"
				@make clean -C $(MINLIB_DIR) > /dev/null 2>&1
				@echo "Minilibx cleaned successfuly"
				@$(RM) $(OBJ_DIR)
				@echo "$(NAME) cleaned successfuly"

fclean :		clean
				@make fclean -C $(LIBFT_DIR)
				@$(RM) $(NAME)

re :			fclean all

.PHONY:			all clean fclean re