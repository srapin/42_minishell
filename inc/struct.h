/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:03:15 by srapin            #+#    #+#             */
/*   Updated: 2023/06/21 02:53:21 by hlesny           ###   ########.fr       */
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

typedef enum type_redirect
{
	noneu,
	fd,
	cmds_cmd,
	pipeu
}					type_redirect;

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


	/*
	type_redirect	in_type;
	int				in_fd;
	void *in_content; // soit un t_node soit un file_struct
	type_redirect	out_type;
	int				out_fd;
	void *out_content; // soit un t_node soit un file_struct
	type_redirect	err_type;
	int				err_fd;
	void *err_content; // soit un t_node soit un file_struct
	*/
}					t_redirect;

typedef struct cmd
{
	t_cmd_value		val; // nom de la commande
	t_redirect 		red; 
	t_ht_hash_table *env;
	t_list 			*export_history;
	int				pid;
	ctrl_op			ctrl;
	struct cmd		*next;

}					t_cmd;


#endif