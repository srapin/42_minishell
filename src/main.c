/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:32:17 by Helene            #+#    #+#             */
/*   Updated: 2023/06/26 19:24:54 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Prints the hash map */
// void	print_ht(t_ht_hash_table *ht)
// {
// 	printf("ht->count = %ld\n\n", ht->count);
// 	for (int i = 0; i < ht->size; i++)
// 	{
// 		if (ht->items[i])
// 		{
// 			printf("hash->items[%d] :\nkey = %s \nvalue = %s\n\n",
// 					i,
// 					ht->items[i]->key,
// 					ht->items[i]->value);
// 		}
// 	}
// }

void 	exec_command(t_data *data, char *command)
{
	// t_token_list 	*tk_list;
	t_cmd 			*cmd;
	
	// checker s'il y a un \n. si c'est le cas, ne fait rien (pour eviter qu on nous pqsse des commqndes cheums en arg)
	data->first = tokenise(assign_type(command, ft_strlen(command)), ft_strlen(command), 
		command);	
	cmd = parse_and_execute(data);
	free_parsing_data(data);
	free_cmds(&cmd, false);
}

void	exec_script(t_data *data, char *path)
{
	int				fd;
	char			*line;
	// t_token_list	*tk_list;

	fd = open(path, O_RDONLY);
	if (!fd)
	{
		perror("open ");
		// return ; ?
	}
	line = get_next_line(fd);
	data->first = tokenise(assign_type(line, ft_strlen(line)), ft_strlen(line),
			line);
	parse_and_execute(data);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	unlink(path);
	safe_close(&fd);
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
	// char *pwd;
	t_data data;
	t_ht_hash_table *hash_map;

	rl_outstream = stderr;
	initialise_data(&data);
	signal(SIGINT, sigint_next_prompt);
	// printf("av0=%s\n", argv[0]);
	
	// si rop d arg bash regrade que premier
	
	// if (argc > 2)
	// {
	// 	printf("error : too many arguments\n");
	// 	return (0);
	// }
	if (!envp || !envp[0])
		hash_map = get_minimal_env(argv[0]);
	else
		hash_map = ht_get_env(envp, argv[0]);
	if (!hash_map)
		return (1); // do we return ? which exit status ?
	t_list *exp_hist = init_export_history(hash_map);
	data.env = hash_map;
	data.exp_history = exp_hist;
	if (argc == 1)
		read_lines(&data);
	else
		exec_command(&data, argv[1]);
	return (g_exit_status);
}
