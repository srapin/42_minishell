/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 22:47:25 by srapin            #+#    #+#             */
/*   Updated: 2023/05/11 23:31:07 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void dup_node_file(t_node *node)
{
	
}

int count_cmds(t_node *first_node)
{
	int nb;
	t_node *node;

	node = first_node;

	nb = 1;
	while(node->red.out_type == pipeu)
	{
		nb++;
		node = node->red.out_content;
	}
	//printf("%d\n",nb);
	return nb;
}

void	safe_pipe(int pipe_tab[2])
{
	if (pipe(pipe_tab) == -1)
	{
		perror("pipe creation failed");
		exit(EXIT_FAILURE);
	}
}

void	fail_process(void)
{
	perror("fork error");
}

void	safe_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -1;
	}
}

void	parent_process(t_node *node, int pipe_tab[2])
{
	safe_close(&(pipe_tab[1]));
	safe_close(&(data->to_read));
	data->to_read = data->pip[0];
	data->pip[0] = -1;
	data->pip[1] = -1;
}

void exec_cmds(t_node *first_node)
{
	int		nb_cmds;
	int 	i;
	int		pip_tab[2];
	int		*pid;
	t_node *next;

	i = 0;
	nb_cmds = count_cmds(first_node);
	pid = malloc(nb_cmds * sizeof(int));
	
	while(i < nb_cmds)
	{
		if (i < nb_cmds - 1)
			safe_pipe(pip_tab);

		pid[i] = fork();
		if (pid[i] < 0)
			fail_process();
		if (pid[i] == 0);
			//child_process(param, data, i, pid);
		if (pid[i] > 0);
			//parent_process(data);
		i++;
	}
}