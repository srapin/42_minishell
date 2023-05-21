/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 22:47:25 by srapin            #+#    #+#             */
/*   Updated: 2023/05/13 02:12:22y srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	ft_strisequal(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (true);
	if (!s1 || !s2)
		return (false);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (false);
	if (!ft_strnstr(s1, s2, ft_strlen(s1)))
		return (false);
	return (true);
}


int count_cmds(t_cmd *first_cmd)
{
	int nb;
	t_cmd *cmd;

	cmd = first_cmd;

	nb = 1;
	while(cmd->red.out_type == cmds_cmd)
	{
		nb++;
		cmd = cmd->red.out_content;
	}
	//printf("%d\n",nb);
	return nb;
}

void	safe_pipe(int pipe_tab[2])
{
	if (pipe(pipe_tab) == 2)
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
	if (*fd > -1)
	{
		close(*fd);
		*fd = -1;
	}
}


void set_fd(t_file * f_struct, int *fd, int flag)
{
	//heredoc?
	if (access(f_struct->name, flag) != 0)
	{
		perror("cannot open file");
		exit(EXIT_FAILURE);
	}
	if (f_struct->name && f_struct->fd < 0)
		f_struct->fd = open(f_struct->name, flag);
	if (f_struct->fd > -1)
		*fd = f_struct->fd;
}


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

void open_cmd_files(t_cmd * cmd)
{
	if (cmd->red.in_type == fd && !((t_file *) (cmd->red.in_content))->sep)
	{
		set_fd(cmd->red.in_content, &(cmd->red.in_fd), O_RDONLY);
	}
	// else if (cmd->red.in_type == fd)
	// 	heredoc(cmd);
	if (cmd->red.out_type == fd)
		set_fd(cmd->red.out_content, &(cmd->red.out_fd), O_WRONLY);
	if (cmd->red.err_type == fd)
		set_fd(cmd->red.err_content, &(cmd->red.err_fd), O_WRONLY);
}


void safe_close_cmd_fd(t_cmd *cmd)
{
	safe_close(&(cmd->red.in_fd));
	if (cmd->red.in_type == fd)
	{
		((t_file *) cmd->red.in_content)->fd = -1;
		// if (((t_file *) cmd->red.in_content)->sep);
		// 	unlink("tmp");
	}
	safe_close(&(cmd->red.out_fd));
	if (cmd->red.out_type == fd)
		((t_file *) cmd->red.out_content)->fd = -1;
	safe_close(&(cmd->red.err_fd));
	if (cmd->red.err_type == fd)
		((t_file *) cmd->red.err_content)->fd = -1;
}


void	parent_process(t_cmd **cmd, int pipe_tab[2])
{
	
	safe_close(&(pipe_tab[1]));
	if (!cmd || !(*cmd))
		return;
	safe_close_cmd_fd(*cmd);
	//if ((*cmd)->red.in_type == fd && ((t_file *) ((*cmd)->red.in_content))->sep)
		//close
	if ((*cmd)->red.out_type == cmds_cmd)
		*cmd = (*cmd)->red.out_content;
	//dprintf(STDOUT_FILENO, "parent pro %s\n", (*cmd)->val.value);
	// if (!cmd || !(*cmd))
	// 	return;
	(*cmd)->red.in_fd = pipe_tab[0];
}

void dup_cmd_file(t_cmd *cmd)
{
	open_cmd_files(cmd);
	if (cmd->red.in_fd > -1)
	{
		dup2(cmd->red.in_fd, STDIN_FILENO);
		safe_close(&(cmd->red.in_fd));
	}
	if (cmd->red.out_fd > -1)
	{

		dup2(cmd->red.out_fd, STDOUT_FILENO);
		safe_close(&(cmd->red.out_fd));
	}
	if (cmd->red.err_fd > -1)
	{

		dup2(cmd->red.err_fd, STDERR_FILENO);
		safe_close(&(cmd->red.err_fd));
	}
}



void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	add_slash(char **paths)
{
	int		i;
	char	*tmp;

	i = 0;
	while (paths && paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(tmp, "/");
		free(tmp);
		if (!paths[i])
		{
			while (paths[++i])
				free(paths[i]);
			free_tab(paths);
		}
		i++;
	}
}



char	**get_path(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	if (!envp)
		perror("envp recuparation prob");
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			paths = ft_split(envp[i] + 5 * sizeof(char), ':');
			break ;
		}
		i++;
	}
	add_slash(paths);
	return (paths);
}

char *get_cmd_value(t_cmd *cmd)
{
	return (cmd->val.value);
}

bool	check_acces(char **paths, t_cmd *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (paths && paths[i])
	{
		path = ft_strjoin(paths[i], get_cmd_value(cmd));
		if (path && access(path, X_OK) == 0)
		{
			cmd->val.path = path;
			return (true);
		}
		free(path);
		i++;
	}
	if (access(cmd->val.value, X_OK) != 0)
		return (false);
	cmd->val.path = cmd->val.value;
	return (true);
}


void	child_process(t_cmd *cmd, int pipe_tab[2], int *to_free)
{
	char	**paths;

	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	// dprintf(5, "child_proc %s, in_fd =  %d\n", cmd->val.value, cmd->red.in_fd);
	dup_cmd_file(cmd);
	// swap_io(param, data, i, to_free);
	paths = get_path(cmd->env);
	// data->arg = ft_split(param->cmds[i], ' ');
	if (check_acces(paths, cmd))
	{
	 	execve(cmd->val.path, cmd->val.args, cmd->env);
	}
	perror("cmd not found");
	free_tab(paths);
	free_tab(cmd->val.args);
	// free(data->path);
	free(to_free);
	exit(EXIT_FAILURE);
}

int	wait_childs(int size, int *childs_pid)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < size)
	{
		waitpid(childs_pid[i], &status, 0);
		i++;
	}
	free(childs_pid);
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

bool check_ret(t_cmd *cmd, int ret)
{
	if (cmd->ctrl == none)
		return true;
	else if (cmd->ctrl == and)
	{
		if (ret == 0)
			return true;
		return false;
	}
	if (ret != 0)
		return true;
	return false;
}

void exec_cmds(t_cmd *first_cmd)
{
	int		nb_cmds;
	int 	i;
	int		pip_tab[2];
	int		*pid;
	t_cmd *cmd;
	t_cmd *next;
	t_cmd *ret_cmd;
	int ret;

	cmd = first_cmd;

	
	while(cmd)
	{
		i = 0;
		nb_cmds = count_cmds(cmd);
		pid = malloc(nb_cmds * sizeof(int));
		next = cmd->next;
		ret_cmd = cmd;
		// dprintf(STDOUT_FILENO, "at beginig of big while %s\n", cmd->val.value);
		while(i < nb_cmds)
		{
			if (i < nb_cmds - 1)
				safe_pipe(pip_tab);
			else
			{
				pip_tab[0] = -1;
				pip_tab[1] = -1;
			}
			// dprintf(STDOUT_FILENO, "before fork %s, i = %d\n", cmd->val.value, i);
			if (cmd->red.in_type == fd && ((t_file *) (cmd->red.in_content))->sep)
				heredoc(cmd);

			pid[i] = fork();
			if (pid[i] < 0)
				fail_process();
			if (pid[i] == 0)
				child_process(cmd, pip_tab, pid);
			if (pid[i] > 0)
				parent_process(&cmd, pip_tab);
			i++;
		}
		// dprintf(STDOUT_FILENO, "after little while %s\n", cmd->val.value);
		ret = wait_childs(nb_cmds, pid);
			//perror("lol");
		while (!check_ret(ret_cmd, ret))
		{
			cmd = next;
			if (cmd)
				next = cmd->next;
			ret_cmd = cmd;
			//dprintf(STDOUT_FILENO, "after check ret %s\n", cmd->val.value);
		}
		// dprintf(STDOUT_FILENO, "after check ret %s\n", cmd->val.value);
			//break;
		cmd = next;
		//dprintf(STDOUT_FILENO, "at end of big while %s\n", cmd->val.value);
		
	}
}