/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 23:25:02 by Helene            #+#    #+#             */
/*   Updated: 2023/06/20 09:29:21 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int     valid_name(char *name)
{
    int i;

    if (!name)
        return (-1);
    if (!name[0] 
        || (name[0] && !ft_isalpha(name[0]) && name[0] != '_'))
        return (0);
    i = 1;
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && (name[i] != '-' && name[i] != '_'))
            return (0);
        i++;
    }
    return (1);
}

t_list  *init_export_history(t_ht_hash_table *ht)
{
    t_list  *first = ft_lstnew(ft_strdup("OLDPWD"));
    return (first);
}

void insert_in_sorted_lst(t_list **head, t_list *elem)
{
    t_list *lst;
    t_list *prev;
    t_list *tmp;
    char *new_key;

    if (!head || !elem)
        return;
    lst = *head;
    if (!lst)
    {
        *head = elem;
        return; 
    }
    new_key = (char *)elem->content;
    if (ft_strcmp((char*)lst->content, new_key) >= 0)
        return ft_lstadd_front(head, elem);
    while (lst && ft_strcmp((char*)lst->content, new_key) < 0)
    {
        prev = lst;
        lst = lst->next;
    }
    if (!lst)
    {
        prev->next = elem;
        return;
    }
    tmp = prev->next;
    prev->next = elem;
    elem->next = tmp;
}

t_list *get_sorted_hist(t_ht_hash_table *ht)
{
    char *to_insert;
    t_list *lst;
    t_list *current;
    int i = 0;
    
    lst = NULL;
    while (i < ht->size)
    {
        while(i < ht->size && (!ht->items[i] || (!ht->items[i]->key && !ht->items[i]->value))) // (i < ht->size && !ht->items[i])
            i++;
        if (!(i< ht->size) || (!ft_strcmp("_", ht->items[i]->key))) // fait le choix de ne pas print la var d'env "_" (undef behaviour, et posix ne le fait pas non plus)
            break;
        to_insert = ht->items[i]->key;
        current = ft_lstnew(to_insert);
        insert_in_sorted_lst(&lst, current);
        i++;
    }
    return lst;
}

bool ft_lst_in(t_list *lst, char *str)
{
    while (lst)
    {
        if (ft_strisequal((char *)lst->content, str))
            return true;
        lst = lst->next;
    }
    return false;
}

// print toutes les variables ayant été exportées, triées par ordre alphabétique
void    print_export_history(t_ht_hash_table *ht, t_list *export_hist)
{
    int     i;
    int     j;
    char    **sorted_history;
    t_list  *current_var;
    t_list *lst;
    t_list *exp;
    t_list *exp_next;
    char    *to_insert;
    

    lst = get_sorted_hist(ht);
    exp = export_hist;
    while(exp)
    {
        exp_next = exp->next;
        insert_in_sorted_lst(&lst,ft_lstnew(exp->content));
        exp = exp_next;
    }
    current_var = lst;
    char *str;
    while(current_var)
    {
        str = ht_search(ht, current_var->content);
        if (!str && is_in_export_history(export_hist,(char *) current_var->content))
            printf("%s\n", (char *) current_var->content);
        else if (str)
        {
            printf("%s=\"%s\"\n", (char *) current_var->content, str);
        }
       current_var = current_var->next; 
    }
    ft_lstclear(&lst);
}

int     is_in_export_history(t_list *export_hist, char *var_name)
{
    t_list *current;

    current = export_hist;
    while (current)
    {
        if (!ft_strcmp(current->content, var_name))
            return (1);
        current = current->next;
    }
    return (0);
}

/* Deletes var_name from export history. 
Does nothing if the export_history does not contain var_name*/
void    del_from_export_history(t_list **export_hist, char *var_name)
{
    t_list  *current;
    t_list  *tmp;

    current = *export_hist;
    if (!ft_strcmp(current->content, var_name))
    {
        *export_hist = (*export_hist)->next;
        free(current);
        return ;
    }
    while (current->next)
    {
        if (!ft_strcmp(current->next->content, var_name))
        {
            tmp = current->next;
            current->next = tmp->next;
            free(tmp);
            return ;
        }
        current = current->next;
    }
}

int    export_with_value(t_cmd *cmd, char *var_name, int i, int j)
{
    char    *var_value;
    
    var_value = NULL;
    if (valid_name(var_name))
    {
        if (is_in_export_history(cmd->export_history, var_name))
            del_from_export_history(&cmd->export_history, var_name);
        var_value = ft_substr(cmd->val.args[i], j + 1, ft_strlen(cmd->val.args[i]));
        if (!ht_modify_value(cmd->env, var_name, var_value))
            ht_insert_item(cmd->env, var_name, var_value);
    }
    else
    {
        printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
        return (INVALID_VAR_ID);
    }
    return (0);
}

int    export_without_value(t_cmd *cmd, char *var_name, int i)
{
    if (valid_name(var_name))
    {
        if (!is_in_export_history(cmd->export_history, var_name))
            ft_lstadd_back(&(cmd->export_history), ft_lstnew(var_name));
    }
    else
    {
        printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
        return (INVALID_VAR_ID);
    }
    return (EXIT_OK);
}

int     export_variable(t_cmd *cmd, int i)
{
    int j;
    char *var_name;

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

// var en plus dans export quand arrive dans un nouveau shell : OLDPWD 

// modif : mettre la t_list **export_history dans la structure de t_cmd 
int    ft_export(t_cmd *cmd, t_cmd *first)
{
    int     i;
    char    *var_name;
    char    *var_value;

    i = 1;
    if (!cmd->val.args[i])
    {
        print_export_history(cmd->env, cmd->export_history);
        return (EXIT_OK);
    }
    while (cmd->val.args[i])
    {
        if (export_variable(cmd, i))
            return (INVALID_VAR_ID);
        i++;
    }
    return (EXIT_OK);
}