/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:03:15 by srapin            #+#    #+#             */
/*   Updated: 2023/06/23 00:49:19 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "lib.h"
#include <stdbool.h>

typedef struct s_ht_hash_table t_ht_hash_table;

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


#endif