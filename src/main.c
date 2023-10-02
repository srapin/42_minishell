/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:32:17 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 14:01:51 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command(t_data *data, char *command)
{
	t_cmd	*cmd;

	data->first = tokenise(assign_type(command, ft_strlen(command)),
			ft_strlen(command), command);
	cmd = parse_and_execute(data);
	free_parsing_data(data);
	free_cmds(&cmd, false);
}

void	initialise_data(t_data *data)
{
	data->first = NULL;
	data->files = NULL;
	data->exp_history = NULL;
	data->env = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data			data;
	t_ht_hash_table	*hash_map;
	t_list			*exp_hist;

	rl_outstream = stderr;
	initialise_data(&data);
	signal(SIGINT, sigint_next_prompt);
	signal(SIGQUIT, SIG_IGN);
	if (!envp || !envp[0])
		hash_map = get_minimal_env(argv[0]);
	else
		hash_map = ht_get_env(envp, argv[0]);
	if (!hash_map)
		return (1);
	exp_hist = NULL;
	data.env = hash_map;
	data.exp_history = exp_hist;
	if (argc == 1)
		read_lines(&data);
	else
		exec_command(&data, argv[1]);
	return (g_exit_status);
}
