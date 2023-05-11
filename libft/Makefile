# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: srapin <srapin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/21 21:53:58 by therodri          #+#    #+#              #
#    Updated: 2023/04/23 23:25:25 by srapin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

FILES=	ft_max\
		ft_strnlen\
		ft_num_len\
		ft_parse\
		ft_print_args\
		ft_print_cs\
		ft_print_mod\
		ft_print_nbrs\
		ft_printf\
		ft_putnbr_base\
		ft_reset_struct\
		ft_strstr\
		ft_write\
		gnl/get_next_line_utils\
		gnl/get_next_line_bonus

SRCS_DIR = ./
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILES)))

OBJS_DIR = ./
OBJS = $(addprefix $(SRCS_DIR), $(addsuffix .o, $(FILES)))

LIBFT_DIR = mylibft/
LIBFT_NAME = lib.a
LIBFT = $(LIBFT_DIR)$(LIBFT_NAME)

$(NAME): $(OBJS) $(LIBFT)
	cp ${LIBFT} $@
	${AR} $@ $(OBJS)


$(LIBFT) :
		make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

bonus: $(NAME)

.PHONY: all clean fclean re
