/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:09:03 by Helene            #+#    #+#             */
/*   Updated: 2023/06/24 10:32:36 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// peut regrouper cette fonction et random_filename() utilisée dans les here docs en une seule,
// en passant comme argument a la fonction si veut ecrire "/tmp/subshell_args_" ou "/tmp/here_doc_tmp_"
char	*random_subshell_fname(void)
{
	static size_t	files_count;
	char			*count;
	char			*filename;

	count = ft_itoa(files_count);
	filename = ft_strjoin("/tmp/subshell_args_", count);
	while (access(filename, F_OK) == 0)
	{
		free(filename);
		free(count);
		files_count++;
		count = ft_itoa(files_count);
		filename = ft_strjoin("/tmp/subshell_args_", count);
	}
	free(count);
	return (filename);
}



/* Retourne une liste chainée de pipelines, avec chaque {commande
	+ redirections} 
regroupées dans un élément de type t_cmd.

Parse le flux de tokens :

Déclare et initialise une variable t_cmd.
Tant que ne tombe pas sur un opérateur de controle (ie un && ou un ||), 
met à jour cette commande avec les tokens que l'on lit :
    -> Dès que tombe sur un opérateur de redirection,
	met à jour l'attribut t_redirect de t_cmd
    -> Sinon, met à jour le nom et les arguments de la commande.
Dès que tombe sur un opérateur de controle :
    -> Met à jour l'attribut ctrl_op de la variable t_cmd actuelle.
    -> Ajoute la variable t_cmd actuelle à la liste de commandes.
    -> Déclare et initialise une nouvelle variable t_cmd.
   
	-> Répète les opérations ci-dessus jusqu'à tomber à nouveau sur un opérateur de 
       contrôle, ou jusqu'à être arrivé à la fin de la liste de tokens.


(echo un | (echo deux && echo trois))
*/

void	write_in_file(t_token_list **curr_tk, int fd_subshell)
{
	int				open_parentheses;
	t_token_list	*current_tk;

	open_parentheses = 1;
	current_tk = (*curr_tk)->next;
	while (current_tk)
	{
		if (current_tk->type == l_parenthesis)
			open_parentheses++;
		if (current_tk->type == r_parenthesis)
			open_parentheses--;
		if (!open_parentheses)
			break ;
		if (write(fd_subshell, current_tk->content, current_tk->length) == -1)
			perror("write ");
		current_tk = current_tk->next;
	}
	if (current_tk)
		current_tk = current_tk->next;
	*curr_tk = current_tk;
}

char	*get_subshell_command(t_token_list **curr_tk)
{
	int				open_parentheses;
	char 			*subshell_cmd;
	char 			*tmp;
	t_token_list	*current_tk;

	open_parentheses = 1;
	current_tk = (*curr_tk)->next;
	tmp = NULL;
	subshell_cmd = NULL;
	while (current_tk)
	{
		if (current_tk->type == l_parenthesis)
			open_parentheses++;
		if (current_tk->type == r_parenthesis)
			open_parentheses--;
		if (!open_parentheses)
			break ;
		tmp = subshell_cmd;
		subshell_cmd = ft_strjoin(tmp, current_tk->content);
		free(tmp);
		/* if (write(fd_subshell, current_tk->content, current_tk->length) == -1)
			perror("write "); */
		current_tk = current_tk->next;
	}
	if (current_tk)
		current_tk = current_tk->next;
	*curr_tk = current_tk;
	return (subshell_cmd);
}

void	set_subshell_attributs(t_cmd *current_cmd, t_token_list **curr_tk)
{
	char 	*subshell_cmd;

	subshell_cmd = get_subshell_command(curr_tk);
	current_cmd->val.value = ft_strdup("./minishell");
	current_cmd->val.args = malloc(sizeof(char *) * 3);
	if (!current_cmd->val.args)
	{
		perror("malloc ");
		return ;
	}
	current_cmd->val.args[0] = ft_strdup("./minishell");
	current_cmd->val.args[1] = ft_strdup(subshell_cmd);
	current_cmd->val.args[2] = NULL;
	free(subshell_cmd);
	subshell_cmd = NULL;
}

void	set_subshell(t_cmd *current_cmd, t_token_list **curr_tk)
{
	char 	*subshell_cmd;
	char *mini_path = ht_search(current_cmd->env, "MINISHELL_PATH");
	
	if (!current_cmd)
		return ;
	subshell_cmd = get_subshell_command(curr_tk);
	current_cmd->val.value = ft_strdup(mini_path);
	// current_cmd->val.value = ft_strdup("./minishell");
	current_cmd->val.args = malloc(sizeof(char *) * 3);
	// dprintf(1, "set sub=%s\n", test);
	if (!current_cmd->val.args)
	{
		perror("malloc ");
		return ;
	}
	current_cmd->val.args[0] = ft_strdup(mini_path);
	// current_cmd->val.args[0] = ft_strdup("./minishell");
	current_cmd->val.args[1] = ft_strdup(subshell_cmd);
	current_cmd->val.args[2] = NULL;
	free(subshell_cmd);
	subshell_cmd = NULL;
	//set_subshell_attributs(current_cmd, curr_tk);
}
