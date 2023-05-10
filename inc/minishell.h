/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/05/10 20:12:54 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <wait.h>


typedef enum type_parse
{
    pip,
    literal,
    cmd
} type_parse;

typedef enum ctrl_op
{
	none,
	and,
	or,
	pointvirgule
} ctrl_op;

typedef enum type_redirect
{
    fd,
    pipeu
}type_redirect;

typedef struct cmd_value
{
    //int child_count;
    char *value;
} t_cmd_value;


typedef struct file
{
    char * name; //dans parsing
    int fd; //si deja ouvert. dans parsing -> mettre a -1
    int to_exchange_whit; 
	char *sep; //mettre a null si pas heredoc
    /* data */
} t_file;

typedef struct redirect
{
    type_redirect type;
    void *content; // soit un t_node soit un truc
    /* data */
}t_redirect;




typedef struct node
{
    //type_parse type;
    t_cmd_value val; // nom de la commande
    char *args;
    t_redirect *redirects; //null terminated
    char **env;
    ctrl_op ctrl;
    struct node *next;
    /* data */
} t_node;

