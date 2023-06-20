/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:29:36 by Helene            #+#    #+#             */
/*   Updated: 2023/06/20 02:28:50 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void    free_merged_words(t_word_data *wd)
{
    t_word_data *tmp;

    while (wd)
    {
        tmp = wd;
        wd = wd->next;

        free(tmp->content);
        free(tmp);
        tmp = NULL;
    }
}

void    free_tokens(t_token_list **first)
{
    t_token_list    *current;
    t_token_list    *tmp;

    if (!first || !(*first))
        return ;
    current = *first;
    while (current)
    {
        tmp = current;
        current = current->next;
        
        free(tmp->content);
        free_merged_words(tmp->merged_words);
        //ft_lstclear(&tmp->before_exp);
        free(tmp);
        tmp = NULL;
    }
    free(first);
    first = NULL;
}

void    free_parsing_data(t_data *data)
{
    if (data->first)
        free_tokens(data->first);
    if (data->env)
        free_pwd(data->env);
    if (data->env)
        ht_del_hash_table(data->env);
    if (data->exp_history)    
        ft_lstfree(&data->exp_history, free);
    data->env=NULL;
    data->exp_history=NULL;
    data->first = NULL;
}