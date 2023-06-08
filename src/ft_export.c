/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 23:25:02 by Helene            #+#    #+#             */
/*   Updated: 2023/06/08 15:38:19 by Helene           ###   ########.fr       */
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

/*
bash-3.2$ export hola=bon(jour
bash: syntax error near unexpected token `('
*/




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

// print toutes les variables ayant été exportées, triées par ordre alphabétique
void    print_export_history(t_ht_hash_table *ht, t_list *export_hist)
{
    int     i;
    int     j;
    char    **sorted_history;
    t_list  *current_var;
    char    *to_insert;
    
    j = 0;
    sorted_history = ft_calloc(sizeof(char *), ht->count + ft_lstsize(export_hist));
    if (!sorted_history)
    {
        perror("malloc ");
        return ;
    }
    to_insert = ht->items[0]->key; // choix arbitraire
    while (j < (ht->count + ft_lstsize(export_hist)))
    {
        i = 0;
        current_var = export_hist;
        while (i < ht->count)
        {
            while (i < ht->count && ft_strcmp(to_insert, ht->items[i]->key) < 0)
                i++;
            if (i < ht->count) // ie ft_strcmp(to_insert, current) > 0 (== 0 est impossible)
            {
                if (!is_in_tab(sorted_history, to_insert))
                    to_insert = ht->items[i]->key;
            }
            i++;
        }
        while (current_var)
        {
            while (current_var && ft_strcmp(to_insert, current_var->content) < 0)
                current_var = current_var->next;
            if (current_var) // ie ft_strcmp(to_insert, current) > 0 (== 0 est impossible)
            {
                if (!is_in_tab(sorted_history, to_insert))
                    to_insert = current_var->content;
            }
            current_var = current_var->next;
        }
        get_and_print_var(ht, export_hist, to_insert);
        sorted_history[j] = ft_strdup(to_insert);
        j++;
    }
    free_tab(sorted_history);
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
int    ft_export(t_list **export_history, t_ht_hash_table *ht, char **args)
{
    int     i = 0;
    int     j = 0;
    char    *var_name;
    char    *var_value;

    if (!args)
    {
        print_export_history(ht, *export_history);
        return (EXIT_OK);
    }
    while (args[i])
    {
        j = 0;
        while (args[i][j] && args[i][j] != '=')
            j++;
        if (args[i][j]) // ie est tombé sur un '='
        {
            var_name = ft_substr(args[i], 0, j);
            if (valid_name(var_name))
            {
                if (is_in_export_history(*export_history, var_name))
                    del_from_export_history(export_history, var_name);
                var_value = ft_substr(args[i], j + 1, ft_strlen(args[i]));
                if (!ht_modify_value(ht, var_name, var_value)) // ie si la variable n'est pas deja dans l'env
                    ht_insert_item(ht, var_name, var_value);
                  
            }
            else
            {
                printf("export : `%s' : not a valid identifier\n", args[i]);
                return (INVALID_VAR_ID);
            }
        }
        else // ie est arrivé à la fin de args[i], sans avoir trouvé de 
        {
            var_name = args[i];
            if (valid_name(var_name))
            {
                if (!is_in_export_history(*export_history, var_name))
                    ft_lstadd_back(export_history, ft_lstnew(var_name));
                // si est deja dans export history, ne fait rien (vu que ne modifie pas sa valeur)
            }
            else
            {
                printf("export : `%s' : not a valid identifier\n", args[i]);
                return (INVALID_VAR_ID);
            }
        }
        i++;
    }
    return (EXIT_OK);
}