/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 04:16:54 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/23 02:48:26 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_shell_level(t_ht_hash_table *ht)
{
	int		i;
	int		nb;
	char	*shlvl;
	char	*new_shlvl;

	shlvl = ht_search(ht, "SHLVL");
	if (!shlvl)
		ht_modify_value(ht, "SHLVL", ft_strdup("1"));
	else
	{
		i = 0;
		while (shlvl[i])
		{
			if (!ft_isdigit(shlvl[i]))
			{
				ht_modify_value(ht, "SHLVL", ft_strdup("1"));
				return ;
			}
			i++;
		}
		nb = ft_atoi(shlvl);
		new_shlvl = ft_itoa(nb + 1);
		ht_modify_value(ht, "SHLVL", new_shlvl);
	}
}

t_ht_hash_table	*ht_get_env(char **envp)
{
	int				i;
	int				j;
	t_ht_hash_table	*ht;

	i = 0;
	ht = ht_new(HT_INITIAL_SIZE);
	if (!ht)
		return (NULL);
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		ht_insert_item(ht, ft_substr(envp[i], 0, j), ft_substr(envp[i], j + 1,
					ft_strlen(envp[i])));
		i++;
	}
	if (!ht_search(ht, "SHLVL"))
		ht_insert_item(ht, ft_strdup("SHLVL"), ft_strdup("0"));
	else
		set_shell_level(ht);
	return (ht);
}

t_ht_hash_table	*get_minimal_env(void)
{
	size_t size;
	char *pwd;
	t_ht_hash_table *ht;

	ht = ht_new(HT_INITIAL_SIZE);
	size = GETCWD_INITIAL_BUF_SIZE;
	pwd = ft_calloc(sizeof(char), size);
	if (!pwd)
		return (NULL);
	getcwd(pwd, size);
	while (!pwd)
	{
		free(pwd);
		size *= 2;
		pwd = ft_calloc(sizeof(char), size);
		if (!pwd)
			return (NULL);
		getcwd(pwd, size);
	}
	ht_insert_item(ht, ft_strdup("SHLVL"), ft_strdup("1"));
	ht_insert_item(ht, ft_strdup("PWD"), pwd);
	ht_insert_item(ht, ft_strdup("_"), ft_strdup("/usr/bin/env"));
	return (ht);
}