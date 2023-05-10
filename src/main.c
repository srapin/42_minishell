/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:28:06 by srapin            #+#    #+#             */
/*   Updated: 2023/05/10 20:12:01 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void init_node(t_node *node, char **envp)
{
	node->val.value = NULL;
	node->args = NULL;
	node->redirects = NULL;
	node->env = envp;
	node->ctrl = none;	
	node->next = NULL;
}

void make_cat_node(t_node *node, t_redirect *red, t_file *f, char **envp)
{
	f->name = "out";
	f->fd = -1;
	f->to_exchange_whit = STDOUT_FILENO;
	f->sep = NULL;
	
	red->type = fd;
	red->content = &f;
	
	init_node(node, envp);
	node->redirects = red;
	node->val.value = "cat";
	node->ctrl = and;

}

void make_echo_node(t_node *node, char ** envp)
{
	init_node(node, envp);
	node->val.value = "echo";
	node->args = "hello world";
}

int main(int ac, char **av, char **envp)
{
	t_node cat_node;
	t_redirect cat_red;
	t_file cat_f;
	
	t_node echo_node;


	
	make_cat_node(&cat_node, &cat_red, &cat_f, envp);
	make_echo_node(&echo_node, envp);
	

	return 0;
}


//    cat out && ls | grep "c" | wc -l || echo "hello world"


//    <in >out ls | wc -l >out2 2>out.err | cat out >out3 && echo coucou || cat out