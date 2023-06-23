/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:03:15 by srapin            #+#    #+#             */
/*   Updated: 2023/06/23 14:58:03 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "lib.h"
#include <stdbool.h>

#define HT_INITIAL_SIZE     100
#define HT_PRIME_FACTOR_A   419 // The variable a should be a prime number larger than the size of the alphabet. We're hashing ASCII strings, which has an alphabet size of 128, so we should choose a prime larger than that.
#define HT_PRIME_FACTOR_B   311
#define WHITESPACES        	" \t"

/* ----------------------- HASH TABLE ------------------------*/

typedef struct  s_ht_item
{
    char *key;
    char *value;
}               t_ht_item;

typedef struct  s_ht_hash_table
{
    size_t      count;
    size_t      base_size;
    size_t      size;
    t_ht_item   **items;
}               t_ht_hash_table;

/* ----------------------- TOKENS STREAM ----------------------*/

enum e_t_types
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

enum e_quotes
{
    none,
    single,
    double_q
};

typedef struct  s_token
{
    int     type;
    char    content;
}               t_token;

typedef struct s_word_data
{
    char                *content;
    size_t              length;
    int                 quotes;
    size_t              word_start_index; // index of this word in the merged words' resulting word (ie the big one)
    struct s_word_data  *next;
} t_word_data;

// utilisé dans l'expansion de filenames (ie gestion des wildcards)
typedef struct s_filename
{
    char                *filename;
    struct s_filename   *next;
}               t_filename;

typedef struct  s_token_list
{
    int                 type;
    char                *content;
    size_t              length;
    int                 quotes;
    t_word_data         *merged_words;
    struct s_token_list *prev;
    struct s_token_list *next;
}               t_token_list;

/* ----------------------- AST -----------------------*/

typedef enum type_parse
{
	pip,
	literal,
	cmd,
	node
}					type_parse;

typedef enum ctrl_op
{
	pointvirgule,
	and,
	or
}					ctrl_op;

typedef struct s_cmd_value
{
	//int child_count;
	char			*value;
	char			**args;
	char			*path;
	// char			**env;
}					t_cmd_value;

typedef struct file
{
	char *name; //dans parsing
	int fd;     //si deja ouvert. dans parsing -> mettre a -1
	//char *sep;  //mettre a null si pas heredoc. si heredoc, est le limiteur
	int				flag;
	/* data */
}					t_file;

typedef struct cmd t_cmd;

typedef struct redirect
{
	t_list 	*in_list; 
	t_list 	*out_list;
	t_file 	*out; // test
	t_cmd	*next_cmd;
	int in_fd;
	int out_fd;
}					t_redirect;

typedef struct cmd
{
	t_cmd_value		val; // nom de la commande
	t_redirect 		red; 
	t_ht_hash_table *env;
	t_list 			*export_history;
	t_list 			*filenames;
	int				pid;
	ctrl_op			ctrl;
	struct cmd		*next;
}					t_cmd;

typedef struct  s_data
{
    t_ht_hash_table *env;
    t_token_list    **first;
    t_list          *exp_history;
    t_list          *files;
}               t_data;

#endif