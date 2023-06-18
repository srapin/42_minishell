/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 23:25:02 by Helene            #+#    #+#             */
/*   Updated: 2023/06/18 13:58:46 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Implementation of the export builtin with no options.
export() : The supplied variables are marked for automatic export to the environment
of subsequently executed commands (is it makes them global variables)
*/

/*
Si n'a aucun argument, alors liste toutes les variables d'env précédées de "export "
Si a une seule string, sans '=', alors export mais a la valeur NULL, donc n'apparait pas dans la hasp_map de l'env -> ne fait rien

ATTENTION : TRAITER LES CAS D'ERREUR (on recode ici le builtin from scratch)
*/

/*
aura qqch de la forme export "premier arg"="deuxieme arg"
-> pendant le parsing, finira avec des tokens <word>, qui seront passés
    dans un char** à l'exec

si a uniquement `export var_name`, doit chercher si var_name a été définie dans le process actuel,
ie si a eu `var_name=var_value` avant. 

peut avoir export PATH+="value" : pas a gérer

Sans aucun argument, la commande doit display toutes les variables exportées (ie variables d'env ?)
in our system, regardless of the shell (to only display those of the current shell, the -p option is required)
*/

// a valid variable name in bash can contain letters, digits and underscores, 
// and can only start with a letter or an underscore
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

// !!!!!! When no arguments are given, the results are unspecified. !!!!!!


// n'a au début qu'une variable en plus par rapport à l'env
t_list  *init_export_history(t_ht_hash_table *ht)
{
    t_list  *first = ft_lstnew(ft_strdup("OLDPWD"));
    return (first);
}

int     is_in_tab(char **tab, char *str)
{
    int i;

    i = 0;
    while (tab[i])
    {
        // printf("%s\n%s\n", tab[i], str);
        // printf("%d\n", ft_strcmp(tab[i], str));
        if (!ft_strcmp(tab[i], str))
            return (1);
        i++;
    }
    return (0);
}

void    get_and_print_var(t_ht_hash_table *ht, t_list *export_list, char *name)
{
    char    *value;

    value = ht_search(ht, name);
    if (value) // ie name est dans l'env
        printf("export %s=\"%s\"\n", name, value);
    else // ie name est uniquement dans export, mais n'y a pas assigné de valeur
        printf("export %s\n", name);
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
        *head= elem;
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
    
    // dprintf(1, "hello\n");
    lst = NULL;
    // dprintf(1, "hey\n");
    while (i < ht->size)
    {
        // ln : a rajouté une condition pour si la variable a été delete de la hash_map (ie on a item->key = NULL et item->value = NULL)
        while(i < ht->size && (!ht->items[i] || (!ht->items[i]->key && !ht->items[i]->value))) // (i < ht->size && !ht->items[i])
            i++;
        if (!(i< ht->size) || (!ft_strcmp("_", ht->items[i]->key))) // fait le choix de ne pas print la var d'env "_" (undef behaviour, et posix ne le fait pas non plus)
            break;
        to_insert = ht->items[i]->key;
        current = ft_lstnew(to_insert);
        // dprintf(1, "hello 2, %s\n", (char *) to_insert);
        insert_in_sorted_lst(&lst, current);
        i++;
    }
    // while (lst)
    // {
    //     dprintf(1, "end of sort hist, %s\n", (char * )lst->content);
    //     lst = lst->next;
    //     /* code */
    // }
    
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

    // dprintf(1, "in print_export_history()\n");
    // j = 0;
    // i = 0;
    // sorted_history = ft_calloc(sizeof(char *), ht->count + ft_lstsize(export_hist));
    // if (!sorted_history)
    // {
    //     perror("malloc ");
    //     return ;
    // }
    
    // to_insert = ht->items[0]->key; // choix arbitraire
    // while (j < (ht->count + ft_lstsize(export_hist)))
    // {
    //     printf("debut du while. to_insert = %s\n", to_insert);
    //     i = 0;
    //     current_var = export_hist;
    //     while (i < ht->count)
    //     {
    //         while (i < ht->count && (!ht->items[i] || ft_strcmp(to_insert, ht->items[i]->key) < 0))
    //             i++;
                
    //         if (i < ht->count) // ie ft_strcmp(to_insert, current) > 0 (== 0 est impossible)
    //         {
    //             printf("ht->items[i]->key = %s\n", ht->items[i]->key);
    //             if (!ft_strcmp("_", ht->items[i]->key)) // fait le choix de ne pas print la variable "_" de l'env (undefined behaviour suite a mes tests)
    //             {
    //                 if (!is_in_tab(sorted_history, to_insert))
    //                 {
    //                     printf("not already in tab\n");
    //                     to_insert = ht->items[i]->key;
    //                 }
    //             }
    //         }
    //         i++;
    //     }
    //     while (current_var)
    //     {
    //         while (current_var && ft_strcmp(to_insert, current_var->content) < 0)
    //             current_var = current_var->next;
    //         if (current_var) // ie ft_strcmp(to_insert, current) > 0 (== 0 est impossible)
    //         {
    //             if (!is_in_tab(sorted_history, to_insert))
    //             {
    //                 printf("not already in tab\n");
    //                 to_insert = current_var->content;
    //             }
    //             current_var = current_var->next;
    //         }
    //         //current_var = current_var->next;
    //     }
    //     printf("fin du while. to_insert = %s\n", to_insert);
    //     get_and_print_var(ht, export_hist, to_insert);
    //     sorted_history[j] = ft_strdup(to_insert);
    //     printf("j = %d, sorted_history[%d] = %s\n\n", j, j, sorted_history[j]);
    //     j++;
    // }
    // free_tab(sorted_history);
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

// var en plus dans export quand arrive dans un nouveau shell : OLDPWD 

// modif : mettre la t_list **export_history dans la structure de t_cmd 
int    ft_export(t_cmd *cmd)
{
    int     i;
    int     j;
    int     nb;
    int     exit_status;
    char    *var_name;
    char    *var_value;


    i = 1;
    //printf("coucou depuis ft_export(), %s\n", cmd->val.args[i]);
    exit_status = EXIT_OK;
    //printf("lol %s\n", cmd->val.args[i]);
    if (!cmd->val.args[i])
    {
        // dprintf(1, "pas d'arg\n");
        print_export_history(cmd->env, cmd->export_history);
        return (exit_status);
    }
    while (cmd->val.args[i])
    {
        j = 0;
        while (cmd->val.args[i][j] && cmd->val.args[i][j] != '=')
            j++;
        if (cmd->val.args[i][j]) // ie est tombé sur un '='
        {
            var_name = ft_substr(cmd->val.args[i], 0, j);
            if (valid_name(var_name))
            {
                if (is_in_export_history(cmd->export_history, var_name))
                    del_from_export_history(&cmd->export_history, var_name);
                var_value = ft_substr(cmd->val.args[i], j + 1, ft_strlen(cmd->val.args[i]));
                if (!strcmp(var_name, "SHLVL"))
                {
                    nb = ft_atoi(var_value);
                    if (nb > 999)
                    {
                        printf("minishell : warning : shell level too high (%d), resetting to 1\n", nb);
                        free(var_value);
                        var_value = ft_strdup("1");
                    }
                }
                // printf("var_value = %s\n", var_value);
                if (!ht_modify_value(cmd->env, var_name, var_value)) // ie si la variable n'est pas deja dans l'env
                    ht_insert_item(cmd->env, var_name, var_value);
                  
            }
            else
            {
                printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
                exit_status = INVALID_VAR_ID;
            }
        }
        else // ie est arrivé à la fin de cmd->val.args[i], sans avoir trouvé de 
        {
            var_name = cmd->val.args[i];
            if (valid_name(var_name))
            {
                
                
                if (!is_in_export_history(cmd->export_history, var_name))
                    ft_lstadd_back(&(cmd->export_history), ft_lstnew(var_name));
                
                // si est deja dans export history, ne fait rien (vu que ne modifie pas sa valeur)
            }
            else
            {
                printf("export : `%s' : not a valid identifier\n", cmd->val.args[i]);
                exit_status = INVALID_VAR_ID;
            }
        }
        i++;
    }
    return (exit_status);
}