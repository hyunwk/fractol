# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/07 16:01:27 by hyunwkim          #+#    #+#              #
#    Updated: 2021/08/07 16:55:53 by hyunwkim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol
NAME_BONUS = fractol_bonus

CC = gcc
CFLAGS = -Wall -Werror -Wextra
MLX = -Imlx -Lmlx -lmlx -framework OpenGL -framework AppKit

SRC = draw.c hook.c init.c util.c
OB = $(SRC:.c=.o)

MLX_DIR = mlx
SRCS_DIR = srcs
BONUS_DIR = bonus

SRCS        = $(addprefix $(SRCS_DIR)/, $(SRC))
OBJS        = $(addprefix $(SRCS_DIR)/, $(OB))

SRCS_BONUS  = $(addprefix $(BONUS_DIR)/, $(SRC))
OBJS_BONUS  = $(addprefix $(BONUS_DIR)/, $(OB))

INCUDE = -I inc

all : $(NAME)

$(NAME) : $(OBJS)
	make -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(MLX) -o $(NAME)

bonus : $(OBJS_BONUS)
	make -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS_BONUS) $(MLX) -o $(NAME_BONUS)

clean :
	rm -f $(OBJS) $(OBJS_BONUS)

fclean : clean
	rm -f $(NAME) $(NAME_BONUS)

re : fclean all
