/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 23:25:02 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 15:40:00 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	search_and_insert(t_list **head, t_list *elem)
{
	t_list	*lst;
	t_list	*prev;
	t_list	*tmp;
	char	*new_key;

	lst = *head;
	new_key = (char *)elem->content;
	while (lst && ft_strcmp((char *)lst->content, new_key) < 0)
	{
		prev = lst;
		lst = lst->next;
	}
	if (!lst)
	{
		prev->next = elem;
		return ;
	}
	tmp = prev->next;
	prev->next = elem;
	elem->next = tmp;
}

int	export_with_value(t_cmd *cmd, char *var_name, int i, int j)
{
	char	*var_value;

	var_value = NULL;
	if (valid_name(var_name))
	{
		if (is_in_export_history(*cmd->export_history, var_name))
			del_from_export_history(cmd->export_history, var_name);
		var_value = ft_substr(cmd->val.args[i], j + 1,
				ft_strlen(cmd->val.args[i]));
		if (!ht_modify_value(cmd->env, var_name, var_value))
			ht_insert_item(cmd->env, var_name, var_value);
		else
			free(var_name);
	}
	else
	{
		printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
		free(var_name);
		return (INVALID_VAR_ID);
	}
	return (0);
}

int	export_without_value(t_cmd *cmd, char *var_name, int i)
{
	if (valid_name(var_name))
	{
		if (!is_in_export_history(*cmd->export_history, var_name))
			ft_lstadd_back(cmd->export_history,
				ft_lstnew(ft_strdup(var_name)));
	}
	else
	{
		printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
		return (INVALID_VAR_ID);
	}
	return (EXIT_OK);
}

int	export_variable(t_cmd *cmd, int i)
{
	int		j;
	char	*var_name;

	j = 0;
	var_name = NULL;
	while (cmd->val.args[i][j] && cmd->val.args[i][j] != '=')
		j++;
	if (cmd->val.args[i][j])
	{
		var_name = ft_substr(cmd->val.args[i], 0, j);
		if (export_with_value(cmd, var_name, i, j))
			return (INVALID_VAR_ID);
	}
	else
	{
		var_name = cmd->val.args[i];
		if (export_without_value(cmd, var_name, i))
			return (INVALID_VAR_ID);
	}
	return (EXIT_OK);
}

int	ft_export(t_cmd *cmd, t_cmd *first)
{
	int	i;
	int	exit_status;

	(void)first;
	i = 1;
	exit_status = EXIT_OK;
	if (!cmd->val.args[i])
	{
		print_export_history(cmd->env, *cmd->export_history);
		return (EXIT_OK);
	}
	while (cmd->val.args[i])
	{
		if (export_variable(cmd, i))
			exit_status = INVALID_VAR_ID;
		i++;
	}
	return (exit_status);
}
