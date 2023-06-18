/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/18 23:38:43 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Quelles erreurs peut-on avoir a gérer ? 
Est-ce que cd ~ est à gérer ? Non car il s'agit d'un token, et qu'il n'est pas à gérer dans minishell
*/

/* Obtient le path complet (chemin absolu) depuis la racine
Néanmoins, des './' ou '../' peuvent encore être présents
*/
char    *get_full_path(t_ht_hash_table *env, char *arg_path)
{
    char    *pwd;

    pwd = *get_pwd(env); // que faire si pwd = NULL ?
    return (ft_strjoin(pwd, arg_path));
}

/* Parses the path, and replaces any occurence of 
'./', or '../'
*/
char *replace_prev_or_actual_dir(char *path)
{
    int     i;
    int     j;
    char    *before;
    char    *after;

    i = 0;
    if (path[0] && path[0] == '/' && path[1] && path[1] == '/' && !path[2]) // ie a juste '//'
    {
        //printf("ok. returning, path = %s\n", path);
        return (path);
    }
    while (path[i])
    {
        // ne peux pas avoir path[0] == '.' car a récupéré pwd, donc path[0] = '/' obligatoirement
        if (path[i] == '.' && path[i - 1] == '/')
        {
            // simply deletes the '.' or './'
            if ((!path[i + 1] || path[i + 1] == '/')) // '.' ou './'
            {
                before = ft_substr(path, 0, i);
                after = ft_substr(path, i + 2, ft_strlen(path));
                free(path);
                path = ft_strjoin(before, after);
                free(before);
                free(after);
                before = NULL;
                after = NULL;
            }
            else if (path[i + 1] && path[i + 1] == '.')
            {
                /* Deletes the '..' or '../', along with the <dir_name>/ that 
                comes previously, ONLY if there's something before the 
                previous '/'
                */
                if ((!path[i + 2] || path[i + 2] == '/')) // '..' ou '../'
                {
                    
                    if (i - 2 >= 0) // ie n'est pas à la racine
                    {
                        // détermine l'index du précédent '/' (par ex le premier '/' de "cd ./dir1/..")
                        j = i - 2; // ie l'index de l'élément précédant le '/' précédant le premier '.'
                        while (j >= 0 && path[j] != '/')
                            j--;
                        before = ft_substr(path, 0, j + 1);
                    }
                    else // ie est à la racine -> supprime alors juste '..' (ou '../')
                        before = ft_strdup("/");
                    after = ft_substr(path, i + 3, ft_strlen(path));
                    if (i - 2 >= 0)
                        i = j + 1; // ou juste i = j, change rien 
                    else 
                        i = 0; // ouais ?
                    free(path);
                    path = ft_strjoin(before, after);
                    free(before);
                    free(after);
                    before = NULL;
                    after = NULL;
                }
            }
        }
        else
            i++;
        
    }

    // tej les '/' successifs
    i = 0;
    j = 0;
    char *tmp;        
    while (path[i])
    {
        if (path[i] == '/')
        {
            j = i;
            //i++;
            while (path[j] && path[j] == '/')
                j++;
            before = ft_substr(path, 0, i + 1);
            after = ft_substr(path, j, ft_strlen(path));
            free(path);
            path = ft_strjoin(before, after);
            free(before);
            free(after);
        }
        i++;
    }
    //printf("returning, path = %s\n", path);
    return (path);
}

/* 
Implements the cd builtin with only a relative or absolute path.
Returns 0 IF directory is successfully changed

*/
int     ft_cd(t_cmd *cmd)
{
    char    *full_path;
    char    *tmp;
    char    *pwd;
    char    *after_rel;
    char    *new_pwd;

    //dprintf(1, "coucou depuis ft_cd()\n");
    
    
    pwd = NULL;
    new_pwd = NULL;
    tmp = NULL;
    after_rel = NULL;
    full_path = NULL;
    if (!cmd->val.args[1]) // si n'a aucun argument
    {
        if (!ht_search(cmd->env, "HOME"))
        {
            printf("minishell : cd : HOME not set\n");
            return (HOME_NOT_SET);
        }
        // pourquoi le code qui suit fait tout couiller ?? ca unset le HOME, d'ou ??
        full_path = ft_strdup(ht_search(cmd->env, "HOME"));
        if (chdir(full_path) == -1)
        {
            perror("chdir ");
            free(full_path);
            return (CANNOT_ACCESS_DIR);
        }
        //print_ht(cmd->env);
        //printf("after chdir and before modifying PWD value in ht, HOME = %s\n", full_path);
        if (!ht_modify_value(cmd->env, "PWD", full_path)) // ie si PWD n'est dans l'env
            ht_insert_item(cmd->env, ft_strdup("PWD"), full_path);
        //print_ht(cmd->env);
        //printf("after chdir and after modifying PWD value in ht, HOME = %s\n", full_path);
        update_pwd(cmd->env, full_path);
        //printf("after modifying PWD value in singleton, HOME = %s\n", full_path);
        return (EXIT_OK);
    }
    if (cmd->val.args[2]) // si il y a plus d'un argument. val.args est null-terminated
    {
        printf("minishell : cd : too many arguments\n");
        return (CD_TOO_MANY_ARGS);
    }

    if (cmd->val.args[1][0] == '/') // chemin absolu)
        full_path = ft_strdup(cmd->val.args[1]);
    else // chemin relatif
    {
        tmp = ft_strdup(cmd->val.args[1]);
        if (ft_strcmp(*get_pwd(cmd->env), "/")) // ie $PWD != "/"
            tmp = ft_strjoin("/", cmd->val.args[1]);
        full_path = get_full_path(cmd->env, tmp);
        free(tmp);
        tmp = NULL;
    }
        
    if (chdir(full_path) == -1)
    {
        perror("chdir ");
        return (CANNOT_ACCESS_DIR); // Quel code erreur ?
    }
    
    // si le dossier est accessible, et que s'y est bien déplacé :

    tmp = ft_strdup(full_path);
    free(full_path);
    full_path = replace_prev_or_actual_dir(tmp);
    // free(tmp); pourquoi ce free() fait tout couiller ??
    //printf("full path = %s\n", full_path);
    if (ft_strlen(full_path) > 1 && full_path[ft_strlen(full_path) - 1] == '/'
        && full_path[1] != '/')
        {
            tmp = ft_substr(full_path, 0, ft_strlen(full_path) - 1);
            free(full_path);
            full_path = tmp;
        }
    printf("after chdir and before modifying PWD value in ht, HOME = %s\n", ht_search(cmd->env, "HOME"));
    if (!ht_modify_value(cmd->env, "PWD", full_path)) // ie si PWD n'est dans l'env
        ht_insert_item(cmd->env, ft_strdup("PWD"), full_path);
    printf("after chdir and after modifying PWD value in ht, HOME = %s\n", ht_search(cmd->env, "HOME"));
    update_pwd(cmd->env, full_path);
    printf("after modifying PWD value in singleton, HOME = %s\n", ht_search(cmd->env, "HOME"));
        
    /* if (!ht_modify_value(cmd->env, "PWD", full_path)) // ie si PWD n'est dans l'env
        ht_insert_item(cmd->env, ft_strdup("PWD"), full_path);
    update_pwd(cmd->env, full_path); */
    //printf("dans export() : PWD apres ht_modify() = %s, %s\n", ht_search(cmd->env, "PWD"), *get_pwd(cmd->env));
    
    return (EXIT_OK);
}

/*
getcwd() returns a null-terminated string containing an
absolute pathname that is the current working directory of the
calling process.
*/
// char *getpwd()
// {
//     size_t  size;
//     char    *path;

//     size = GETCWD_INITIAL_BUF_SIZE;
//     getcwd(path, size);
//     while (!path && errno == ERANGE)
//     {
//         size *= 2;
//         getcwd(path, size);
//     }
//     return (path);
// }

// int     ft_cd(t_cmd *cmd)
// {
//     char    *path;
//     char    *new_pwd;

//     if (!cmd->val.args[1]) // si n'a aucun argument
//         return (EXIT_OK);
//     if (cmd->val.args[2]) // si il y a plus d'un argument. val.args est null-terminated
//         return (CD_TOO_MANY_ARGS);
//     path = cmd->val.args[1];
//     printf("path = %s\n", path);
//     if (chdir(path) == -1)
//     {
//         perror("chdir ");
//         return(errno); // Quel code erreur ?
//     }
//     new_pwd = getpwd();
//     printf("new pwd = %s\n", new_pwd);
//     ht_modify_value(cmd->env, "PWD", new_pwd);
//     return (EXIT_OK);
// }