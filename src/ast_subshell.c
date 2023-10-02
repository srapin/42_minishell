/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:09:03 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    -> Met à jour l'attribut t_ctrl_op de la variable t_cmd actuelle.
    -> Ajoute la variable t_cmd actuelle à la liste de commandes.
    -> Déclare et initialise une nouvelle variable t_cmd.
   
	-> Répète les opérations ci-dessus jusqu'à tomber à nouveau sur un opérateur de 
       contrôle, ou jusqu'à être arrivé à la fin de la liste de tokens.


(echo un | (echo deux && echo trois))
*/

int	assess_parentheses(t_token_list *current_tk, int *open_parentheses)
{
	if (current_tk->type == l_parenthesis)
		(*open_parentheses)++;
	else if (current_tk->type == r_parenthesis)
		(*open_parentheses)--;
	if (!(*open_parentheses))
		return (0);
	return (1);
}

char	*get_subshell_command(t_token_list **curr_tk)
{
	int				open_parentheses;
	char			*subshell_cmd;
	char			*tmp;
	t_token_list	*current_tk;

	open_parentheses = 1;
	current_tk = (*curr_tk)->next;
	tmp = NULL;
	subshell_cmd = NULL;
	while (current_tk)
	{
		if (!assess_parentheses(current_tk, &open_parentheses))
			break ;
		tmp = subshell_cmd;
		subshell_cmd = ft_strjoin(tmp, current_tk->content);
		free(tmp);
		current_tk = current_tk->next;
	}
	if (current_tk)
		current_tk = current_tk->next;
	*curr_tk = current_tk;
	return (subshell_cmd);
}

void	set_subshell(t_cmd *current_cmd, t_token_list **curr_tk)
{
	char	*subshell_cmd;
	char	*mini_path;

	mini_path = ht_search(current_cmd->env, "MINISHELL_PATH");
	if (!mini_path)
		mini_path = ft_strdup("minishell");
	if (!current_cmd)
		return ;
	subshell_cmd = get_subshell_command(curr_tk);
	current_cmd->val.value = ft_strdup(mini_path);
	current_cmd->val.args = malloc(sizeof(char *) * 3);
	if (!current_cmd->val.args)
	{
		perror("malloc ");
		return ;
	}
	current_cmd->val.args[0] = ft_strdup(mini_path);
	current_cmd->val.args[1] = ft_strdup(subshell_cmd);
	current_cmd->val.args[2] = NULL;
	free(subshell_cmd);
	subshell_cmd = NULL;
}
