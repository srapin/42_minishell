/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:03:15 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:13:04 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "lib.h"
#include <stdbool.h>

typedef enum type_parse
{
	pip,
	literal,
	cmd,
	node
}					type_parse;

typedef enum ctrl_op
{
	none,
	and,
	or
	,
	pointvirgule
}					ctrl_op;

typedef enum type_redirect
{
	noneu,
	fd,
	cmds_cmd,
	pipeu
}					type_redirect;

typedef struct cmd_value
{
	//int child_count;
	char			*value;
	char			**args;
	char			*path;
}					t_cmd_value;

typedef struct file
{
	char *name; //dans parsing
	int fd;     //si deja ouvert. dans parsing -> mettre a -1
	char *sep;  //mettre a null si pas heredoc
	int				flag;
	/* data */
}					t_file;

typedef struct redirect
{
	type_redirect	in_type;
	int				in_fd;
	void *in_content; // soit un t_node soit un file_struct
	type_redirect	out_type;
	int				out_fd;
	void *out_content; // soit un t_node soit un file_struct
	type_redirect	err_type;
	int				err_fd;
	void *err_content; // soit un t_node soit un file_struct
}					t_redirect;

typedef struct cmd
{
	//type_parse type;
	t_cmd_value val; // nom de la commande
	char			**args;
	t_redirect red; 
	char			**env;
	// to move into node
	ctrl_op			ctrl;
	struct cmd		*next;

}					t_cmd;

typedef struct s_node
{
	ctrl_op			ctrl;
	void			*next;
	
	type_parse		type;
	void			*content;
	
	int				ret;
	bool			already_exec;
}					t_node;

#endif