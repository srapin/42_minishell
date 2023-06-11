/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 17:29:36 by Helene            #+#    #+#             */
/*   Updated: 2023/06/10 18:58:59 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

    if (!first)
        return ;
    current = *first;
    while (current)
    {
        tmp = current;
        current = current->next;
        
        free(tmp->content);
        free_merged_words(tmp->merged_words);
        free(tmp);
        tmp = NULL;
    }
}