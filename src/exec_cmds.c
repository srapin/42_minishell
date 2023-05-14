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


int count_cmds(t_node *first_node)
{
	int nb;
	t_node *node;

	node = first_node;

	nb = 1;
	while(node->red.out_type == cmds_node)
	{
		nb++;
		node = node->red.out_content;
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

void	heredoc_process(t_node *node, int here_fd)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	dup2(here_fd, STDOUT_FILENO);
	while (!check_end_of_heredoc(((t_file*) (node->red.in_content))->sep   ,  line ))
	{
		dprintf(here_fd, "%s", line);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	// unlink("tmp");
	exit(0);
}


int heredoc(t_node *node)
{
	int fd;
	int pid;
	int status;

	fd = open("tmp", O_CREAT | O_RDWR);
	pid = fork();
	if (pid < 0)
		fail_process();
	if (pid == 0)
		heredoc_process(node, fd);
	wait(&status);
	dprintf(STDOUT_FILENO, "befsleep");
	sleep(5);
	dprintf(STDOUT_FILENO, get_next_line(fd));
	node->red.in_fd = fd;
	return fd;
}

void open_node_files(t_node * node)
{
	if (node->red.in_type == fd && !((t_file *) (node->red.in_content))->sep)
	{
		set_fd(node->red.in_content, &(node->red.in_fd), O_RDONLY);
	}
	if (node->red.out_type == fd)
		set_fd(node->red.out_content, &(node->red.out_fd), O_WRONLY);
	if (node->red.err_type == fd)
		set_fd(node->red.err_content, &(node->red.err_fd), O_WRONLY);
}


void safe_close_node_fd(t_node *node)
{
	safe_close(&(node->red.in_fd));
	if (node->red.in_type == fd)
	{
		((t_file *) node->red.in_content)->fd = -1;
		if (((t_file *) node->red.in_content)->sep)
			unlink("tmp");
	}
	safe_close(&(node->red.out_fd));
	if (node->red.out_type == fd)
		((t_file *) node->red.out_content)->fd = -1;
	safe_close(&(node->red.err_fd));
	if (node->red.err_type == fd)
		((t_file *) node->red.err_content)->fd = -1;
}


void	parent_process(t_node **node, int pipe_tab[2])
{
	
	safe_close(&(pipe_tab[1]));
	if (!node || !(*node))
		return;
	safe_close_node_fd(*node);
	//if ((*node)->red.in_type == fd && ((t_file *) ((*node)->red.in_content))->sep)
		//close
	if ((*node)->red.out_type == cmds_node)
		*node = (*node)->red.out_content;
	//dprintf(STDOUT_FILENO, "parent pro %s\n", (*node)->val.value);
	// if (!node || !(*node))
	// 	return;
	(*node)->red.in_fd = pipe_tab[0];
}

void dup_node_file(t_node *node)
{
	open_node_files(node);
	if (node->red.in_fd > -1)
	{
		dup2(node->red.in_fd, STDIN_FILENO);
		safe_close(&(node->red.in_fd));
	}
	if (node->red.out_fd > -1)
	{

		dup2(node->red.out_fd, STDOUT_FILENO);
		safe_close(&(node->red.out_fd));
	}
	if (node->red.err_fd > -1)
	{

		dup2(node->red.err_fd, STDERR_FILENO);
		safe_close(&(node->red.err_fd));
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

char *get_node_value(t_node *node)
{
	return (node->val.value);
}

bool	check_acces(char **paths, t_node *node)
{
	int		i;
	char	*path;

	i = 0;
	while (paths && paths[i])
	{
		path = ft_strjoin(paths[i], get_node_value(node));
		if (path && access(path, X_OK) == 0)
		{
			node->val.path = path;
			return (true);
		}
		free(path);
		i++;
	}
	if (access(node->val.value, X_OK) != 0)
		return (false);
	node->val.path = node->val.value;
	return (true);
}


void	child_process(t_node *node, int pipe_tab[2], int *to_free)
{
	char	**paths;

	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		node->red.out_fd = pipe_tab[1];
	}
	dup_node_file(node);
	// swap_io(param, data, i, to_free);
	paths = get_path(node->env);
	// data->arg = ft_split(param->cmds[i], ' ');
	//dprintf(STDOUT_FILENO, "child_proc %s\n", node->val.value);
	if (check_acces(paths, node))
	{
	 	execve(node->val.path, node->args, node->env);
	}
	perror("cmd not found");
	free_tab(paths);
	free_tab(node->args);
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

bool check_ret(t_node *node, int ret)
{
	if (node->ctrl == none)
		return true;
	else if (node->ctrl == and)
	{
		if (ret == 0)
			return true;
		return false;
	}
	if (ret != 0)
		return true;
	return false;
}

void exec_cmds(t_node *first_node)
{
	int		nb_cmds;
	int 	i;
	int		pip_tab[2];
	int		*pid;
	t_node *node;
	t_node *next;
	t_node *ret_node;
	int ret;

	node = first_node;

	
	while(node)
	{
		i = 0;
		nb_cmds = count_cmds(node);
		pid = malloc(nb_cmds * sizeof(int));
		next = node->next;
		ret_node = node;
		dprintf(STDOUT_FILENO, "at beginig of big while %s\n", node->val.value);
		while(i < nb_cmds)
		{
			if (i < nb_cmds - 1)
				safe_pipe(pip_tab);
			else
			{
				pip_tab[0] = -1;
				pip_tab[1] = -1;
			}
			// dprintf(STDOUT_FILENO, "before fork %s\n", node->val.value);
			if (node->red.in_type == fd && ((t_file *) (node->red.in_content))->sep)
				heredoc(node);

			pid[i] = fork();
			if (pid[i] < 0)
				fail_process();
			if (pid[i] == 0)
				child_process(node, pip_tab, pid);
			if (pid[i] > 0)
				parent_process(&node, pip_tab);
			i++;
		}
		// dprintf(STDOUT_FILENO, "after little while %s\n", node->val.value);
		ret = wait_childs(nb_cmds, pid);
			//perror("lol");
		while (!check_ret(ret_node, ret))
		{
			node = next;
			if (node)
				next = node->next;
			ret_node = node;
			//dprintf(STDOUT_FILENO, "after check ret %s\n", node->val.value);
		}
		// dprintf(STDOUT_FILENO, "after check ret %s\n", node->val.value);
			//break;
		node = next;
		//dprintf(STDOUT_FILENO, "at end of big while %s\n", node->val.value);
		
	}
}