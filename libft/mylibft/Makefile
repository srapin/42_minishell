# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: srapin <srapin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/05 01:15:01 by srapin            #+#    #+#              #
#    Updated: 2023/04/23 23:07:22 by srapin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= lib.a
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -g3
AR	= ar -rc
SRC	= ft_isalpha.c\
	ft_isdigit.c\
	ft_isalnum.c\
	ft_isascii.c\
	ft_isprint.c\
	ft_strlen.c\
	ft_memset.c\
	ft_bzero.c\
	ft_memcpy.c\
	ft_memmove.c\
	ft_strlcpy.c\
	ft_strlcat.c\
	ft_toupper.c\
	ft_tolower.c\
	ft_strchr.c\
	ft_strrchr.c\
	ft_calloc.c\
	ft_strncmp.c\
	ft_strnstr.c\
	ft_memchr.c\
	ft_memcmp.c\
	ft_strdup.c\
	ft_substr.c\
	ft_strjoin.c\
	ft_strtrim.c\
	ft_split.c\
	ft_itoa.c\
	ft_strmapi.c\
	ft_striteri.c\
	ft_putchar_fd.c\
	ft_putstr_fd.c\
	ft_putendl_fd.c\
	ft_putnbr_fd.c\
	ft_atoi.c\
	ft_lstnew.c\
	ft_lstadd_front.c\
	ft_lstsize.c\
	ft_lstlast.c\
	ft_lstadd_back.c\
	ft_lstdelone.c\
	ft_lstclear.c\
	ft_lstiter.c

RM	= rm -f
OBJS	= $(SRC:.c=.o)
HEADER	= ./includes

.o: .c
	$(CC) $(CFLAGS) -I$(HEADER) -c $< -o $(<:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:	
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean:	clean 
	$(RM) $(NAME) 

re:	fclean all

.PHONY:	all clean re fclean
