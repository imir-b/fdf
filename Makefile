# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbleskin <vbleskin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/29 13:31:53 by vbleskin          #+#    #+#              #
#    Updated: 2026/01/01 01:44:09 by vbleskin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME

NAME			=	fdf

# DEF

CC				=	cc
CFLAGS			=	-Wall -Werror -Wextra -I includes -I $(LIBFT_DIR)includes -I $(MINLIB_DIR) -g3
RM				=	rm -rf

# DIR

SRC_DIR			=	src/
PARS_DIR		=	parsing/
REND_DIR		=	render/
UTIL_DIR		=	utils/
OBJ_DIR			=	obj/
LIB_DIR			=	lib/
LIBFT_DIR		=	$(LIB_DIR)libft/
MINLIB_DIR		=	$(LIB_DIR)minilibx-linux/

# FILES

SRC_PARS		=	$(PARS_DIR)parsing.c
SRC_REND		=	$(REND_DIR)render.c $(REND_DIR)hooks.c $(REND_DIR)transform.c
SRC_UTIL		=	$(UTIL_DIR)utils.c $(UTIL_DIR)error.c $(UTIL_DIR)utils_data.c
SRC_FILES		=	main.c $(SRC_PARS) $(SRC_REND) $(SRC_UTIL)
SRCS			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS			=	$(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))
LIBS			=	-L$(LIBFT_DIR) -lft -L$(MINLIB_DIR) -lmlx_Linux -lXext -lX11 -lm -lz

# RULES

all :			$(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
				mkdir -p $(dir $@)
				
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME) :		$(OBJS)
				@make -C $(LIBFT_DIR)
				@make -C $(MINLIB_DIR)
				$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean :
				@make clean -C $(LIBFT_DIR)
				@make clean -C $(MINLIB_DIR)
				$(RM) $(OBJ_DIR)

fclean :		clean
				@make fclean -C $(LIBFT_DIR)
				$(RM) $(NAME)

re :			fclean all

.PHONY:			all clean fclean re