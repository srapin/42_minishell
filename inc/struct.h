/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:03:15 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 15:38:43 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "lib.h"
# include <stdbool.h>

# define HT_INITIAL_SIZE 100
# define HT_PRIME_FACTOR_A 419
# define HT_PRIME_FACTOR_B 311
# define WHITESPACES " \t"

	/* ----------------------- HASH TABLE ------------------------*/

typedef struct s_ht_item
{
	char				*key;
	char				*value;
}						t_ht_item;

typedef struct s_ht_hash_table
{
	size_t				count;
	size_t				base_size;
	size_t				size;
	t_ht_item			**items;
}						t_ht_hash_table;

/* ----------------------- TOKENS STREAM ----------------------*/

enum					e_t_types
{
	word,
	simple_quote,
	double_quote,
	r_parenthesis,
	l_parenthesis,
	l_io_redirect,
	r_io_redirect,
	and_tk,
	or_tk,
	whitespace
};

enum					e_quotes
{
	none,
	single,
	double_q
};

typedef struct s_token
{
	int					type;
	char				content;
}						t_token;

typedef struct s_word_data
{
	char				*content;
	size_t				length;
	int					quotes;
	size_t				word_start_index;
	struct s_word_data	*next;
}						t_word_data;

// utilisé dans l'expansion de filenames (ie gestion des wildcards)
typedef struct s_filename
{
	char				*filename;
	struct s_filename	*next;
}						t_filename;

typedef struct s_token_list
{
	int					type;
	char				*content;
	size_t				length;
	int					quotes;
	t_word_data			*merged_words;
	struct s_token_list	*prev;
	struct s_token_list	*next;
}						t_token_list;

/* ----------------------- AST -----------------------*/

typedef enum e_ctrl_op
{
	pointvirgule,
	and,
	or
}						t_ctrl_op;

typedef struct s_cmd_value
{
	char				*value;
	char				**args;
	char				*path;
}						t_cmd_value;

typedef struct s_file
{
	char				*name;
	int					fd;
	int					flag;
	bool				out;
}						t_file;

typedef struct cmd		t_cmd;

typedef struct s_redirect
{
	t_list				*files;
	t_cmd				*next_cmd;
	int					in_fd;
	int					out_fd;
}						t_redirect;

typedef struct cmd
{
	t_cmd_value			val;
	t_redirect			red;
	t_ht_hash_table		*env;
	t_list				**export_history;
	t_list				*filenames;
	int					pid;
	t_ctrl_op			ctrl;
	struct cmd			*next;
}						t_cmd;

typedef struct s_data
{
	t_ht_hash_table		*env;
	t_token_list		**first;
	t_list				*exp_history;
	t_list				*files;
}						t_data;

#endif