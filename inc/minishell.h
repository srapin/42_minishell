/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/05/11 23:18:37 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
# include "../libft/ft_printf.h"
# include "../libft/gnl/get_next_line_bonus.h"
# include "../libft/mylibft/libft.h"

typedef enum type_parse
{
	pip,
	literal,
	cmd
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
	pipeu
}					type_redirect;

typedef struct cmd_value
{
	//int child_count;
	char			*value;
}					t_cmd_value;

typedef struct file
{
	char *name; //dans parsing
	int fd;     //si deja ouvert. dans parsing -> mettre a -1
	char *sep;  //mettre a null si pas heredoc
	/* data */
}					t_file;

typedef struct redirect
{
	type_redirect	in_type;
	int				in_fd;
	void *in_content; // soit un t_node soit un truc
	type_redirect	out_type;
	int				out_fd;
	void *out_content; // soit un t_node soit un truc
	type_redirect	err_type;
	int				err_fd;
	void *err_content; // soit un t_node soit un truc
}					t_redirect;

typedef struct node
{
	//type_parse type;
	t_cmd_value val; // nom de la commande
	char			**args;
	t_redirect  red; //null terminated
	char			**env;
	ctrl_op			ctrl;
	struct node		*next;

}					t_node;

void exec_cmds(t_node *node);

#endif