NAME = minishell
NAME_BONUS = pipex_bonus

CC = cc
#CFLAGS = -Wall -Wextra -Werror -g3 -g
CFLAGS =-g3 -g

FILES = cmd_private\
		cmd_pub\
		common_process\
		exec_cmds \
		file_struct_priv\
		file_struct_pub\
		here_doc\
		main \
		next\
		redir_pub\
		safer\
		tools\


SRCS_DIR = src
SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))

OBJS_DIR = obj
OBJS = ${patsubst ${SRCS_DIR}/%.c, ${OBJS_DIR}/%.o, ${SRCS}}

SRCS_DIR_BONUS = src_bonus
SRCS_BONUS = $(addprefix $(SRCS_DIR_BONUS)/, $(addsuffix .c, $(FILES)))

OBJS_DIR_BONUS = obj_bonus
OBJS_BONUS = ${patsubst ${SRCS_DIR_BONUS}/%.c, ${OBJS_DIR_BONUS}/%.o, ${SRCS_BONUS}}

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a


INCLUDES_DIR = inc
INCLUDES_FILES =lib.h \
				minishell.h \
				priv.h\
				pub.h\
				struct.h

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	rm -rf $(OBJS_DIR_BONUS)
	$(CC) $(OBJS) $(LIBFT) $(CFLAGS) -o $@

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c $(INCLUDES)
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

bonus : $(NAME_BONUS)

$(NAME_BONUS) : $(LIBFT) $(OBJS_BONUS)
	$(CC) $(OBJS_BONUS) $(LIBFT) $(CFLAGS) -o $@
	rm -rf $(OBJS_DIR)

${OBJS_DIR_BONUS}/%.o: ${SRCS_DIR_BONUS}/%.c $(INCLUDES)
	@mkdir -p $(OBJS_DIR_BONUS)
	$(CC)  -o $@ -c $<

clean: cleanlibs
	rm -rf $(OBJS_DIR)
	rm -rf $(OBJS_DIR_BONUS)

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: clean all

$(LIBFT):
	make -C $(LIBFT_DIR)

cleanlibs:
	make fclean -C $(LIBFT_DIR)

.PHONY: all clean fclean re makelib cleanlib