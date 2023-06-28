NAME = minishell
NAME_BONUS = minishell_bonus

CC = cc
CFLAGS = -g3 -g -Wall -Wextra -Werror

FILES = ast \
		ast_simple_command \
		ast_set_cmd_args \
		ast_set_cmd_args_count \
		ast_set_cmd_utils \
		ast_utils \
		ast_subshell \
		ast_heredocs \
		cmd \
		common_process\
		deal_with_error\
		del_quotes_group_words \
		del_quotes_group_words_utils \
		exec_cmds \
		exec_builtins \
		file_struct_else\
		file_struct_init\
		free_tokens \
		free_cmds \
		free_cmds_utils \
		ft_cd \
		ft_cd_utils \
		ft_cd_path_utils \
		ft_echo \
		ft_env \
		ft_exit \
		ft_export \
		ft_export_print \
		ft_export_utils \
		ft_unset \
		ft_pwd \
		get_env \
		hash_table \
		ht_hash_function_utils \
		ht_hash_function_utils2 \
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
		safer_tools\
		syntax \
		syntax_simple_command \
		syntax_pipeline \
		signal \
		tools\
		token_struct_utils \
		var_expansion \
		var_expansion_var_name \
		var_expansion_utils \
		var_expansion_utils2 \
		var_expansion_special_cases \
		wildcards \
		wildcards_utils \
		wildcards_first_sort \
		wildcards_first_sort_utils \
		wildcards_second_sort \
		wildcards_second_sort_utils

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
				struct.h
				# priv.h\
				# pub.h\

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
	valgrind --suppressions=ignore_readline.txt --leak-check=full --show-leak-kinds=all \
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
