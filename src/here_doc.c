/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:37:36 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 01:57:13 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool check_end_of_heredoc(char *sep, char *line)
{
	char *with_backslash;
	bool flag;

	with_backslash = NULL;
	flag = false;
	if ((line[ft_strlen(line) - 1] == '\n') && sep[ft_strlen(sep) - 1] != '\n')
		with_backslash = ft_strjoin(sep, "\n");
	flag = (ft_strisequal(sep, line)  || ft_strisequal(with_backslash, line));
	free(with_backslash);
	return flag;

}

void	heredoc_process(t_cmd *cmd, int *pipe_tab)
{
	char	*line;

	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	// dprintf(5, "child_proc %s, in_fd =  %d\n", cmd->val.value, cmd->red.in_fd);
	dup_cmd_file(cmd);

	line = get_next_line(STDIN_FILENO);
	// safe_close
	// dup2(here_fd, STDOUT_FILENO);

	while (!check_end_of_heredoc(((t_file*) (cmd->red.in_content))->sep, line))
	{
		ft_printf("%s", line);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	
	
	// unlink("tmp");
	exit(0);
}


void heredoc(t_cmd *cmd)
{
	// int here_fd;
	int pid;
	int pip_tab[2];
	int status;

	// here_fd = open("tmp", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	safe_pipe(pip_tab);
	pid = fork();
	if (pid < 0)
		fail_process();
	if (pid == 0)
		heredoc_process(cmd, pip_tab);
	
	safe_close(&(pip_tab[1]));
	// wait(&status);
	// dprintf(STDOUT_FILENO, "here fd = %d\n", here_fd);
	// // sleep(5);
	// // int fd2 = open("out", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	// // dprintf(fd2, get_next_line(fd));
	cmd->red.in_fd = pip_tab[0];
}