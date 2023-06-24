NAME = minishell
NAME_BONUS = minishell_bonus # ???

CC = cc
#CFLAGS = -Wall -Wextra -Werror -g3 -g
CFLAGS = -g3 -g -Wall -Wextra -Werror

FILES = ast \
		ast_simple_command \
		ast_set_cmd_args \
		ast_set_cmd_args_count \
		ast_set_cmd_utils \
		ast_utils \
		ast_subshell \
		ast_heredocs \
		cmd_private\
		cmd \
		common_process\
		del_quotes_group_words \
		del_quotes_group_words_utils \
		exec_cmds \
		exec_builtins \
		file_struct_priv\
		file_struct_pub\
		free_tokens \
		free_cmds \
		ft_env \
		ft_export \
		ft_unset \
		ft_cd \
		ft_pwd \
		ft_exit \
		get_env \
		hash_table \
		ht_search_modify \
		ht_insert \
		ht_delete \
		lexer \
		main \
		next\
		parser \
		redir_pub \
		readline \
		safer\
		syntax \
		syntax_simple_command \
		syntax_pipeline \
		signal \
		tools\
		token_struct_utils \
		var_expansion \
		var_expansion_var_name \
		var_expansion_utils \
		wildcards \
		wildcards_utils \
		wildcards_first_sort \
		wildcards_first_sort_utils \
		wildcards_second_sort \
		wildcards_second_sort_utils

#PARSING_FILES = 	ast \
					ast_heredocs


SRCS_DIR = src
#PARSING_DIR = $(SRCS_DIR)/parsing

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))#:$(addprefix $(PARSING_DIR)/, $(addsuffix .c, $(PARSING_FILES)))

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

export C_INCLUDE_PATH = $(INCLUDES_DIR):$(LIBFT_DIR)
export LIBRARY_PATH = $(LIBFT_DIR)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	rm -rf $(OBJS_DIR_BONUS)
	$(CC) $(OBJS) $(LIBFT) $(CFLAGS) -o $@ -lreadline

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

leaks :
	valgrind --suppressions=ignore_readline_aurele.txt --leak-check=full --show-leak-kinds=all \
	--track-origins=yes --track-fds=yes --trace-children=yes ./$(NAME) 


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
