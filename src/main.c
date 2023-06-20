/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:32:17 by Helene            #+#    #+#             */
/*   Updated: 2023/06/20 02:23:32 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// peut avoir un shell level maximal de 999 (ie export SHLVL=1000)
// si excede : bash: warning: shell level (1099) too high, resetting to 1 (avec export SHLVL=1100 ici)
void    set_shell_level(t_ht_hash_table *ht)
{
    int     i;
    int     nb;
    char    *shlvl;
    char    *new_shlvl;

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
                return;
            }
            i++;
        }
        nb = ft_atoi(shlvl);
        new_shlvl = ft_itoa(nb + 1);
        ht_modify_value(ht,"SHLVL", new_shlvl);
    }
}

t_ht_hash_table *ht_get_env(char **envp)
{
    int i;
    int j;
    t_ht_hash_table *ht;

    i = 0;
    ht = ht_new(HT_INITIAL_SIZE);
    if (!ht)
        return (NULL);
    while (envp[i])
    {
        j = 0;
        while (envp[i][j] && envp[i][j] != '=')
            j++;
        //printf("ok, env[%d] = %s\n", i, envp[i]);
        //printf("key = %s\n", ft_substr(envp[i], 0, j));
        //printf("value = %s\n\n", ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
        ht_insert_item(ht, ft_substr(envp[i], 0, j), ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
        i++;
    }
    set_shell_level(ht);
    return (ht);
}

char *ft_strcpy(char *str)
{
    int i;
    char *cpy;
    
    if (!str)
        return (NULL);
    i = -1;
    cpy = ft_calloc(sizeof(char), ft_strlen(str));
    if (!cpy)
        return (NULL);
    while (str[++i])
        cpy[i] = str[i];
    cpy[i] = '\0';
    return (cpy);
}

// seulement dans le cas env -i, ou aussi lorsque l'env passé au main est nul ? 
t_ht_hash_table *get_minimal_env() // comment indiquer a quel niveau du shell se situe ? (pour la SHLVL variable de l'env)
{
    size_t          size;
    char            *pwd;
    t_ht_hash_table *ht;
    
    ht = ht_new(HT_INITIAL_SIZE);
    size = GETCWD_INITIAL_BUF_SIZE;
    pwd = ft_calloc(sizeof(char), size);
    if (!pwd)
        return (NULL); // ?
    getcwd(pwd, size);
    while (!pwd)
    {
        free(pwd);
        size *= 2;
        pwd = ft_calloc(sizeof(char), size);
        if (!pwd)
            return (NULL); // ?
        getcwd(pwd, size);
    }
    ht_insert_item(ht, ft_strcpy("SHLVL"),ft_strdup("1"));
    ht_insert_item(ht, ft_strcpy("PWD"), pwd);
    ht_insert_item(ht, ft_strcpy("_"), ft_strcpy("/usr/bin/env"));
    return (ht);
}

/* Prints the hash map */
void    print_ht(t_ht_hash_table *ht)
{
    printf("ht->count = %ld\n\n", ht->count);
    for (int i = 0; i < ht->size; i++)
    {
        if (ht->items[i])
        {
            printf("hash->items[%d] :\nkey = %s \nvalue = %s\n\n", \
            i, ht->items[i]->key, ht->items[i]->value);
        }
    }
}



void    exec_script(t_data *data, char *path)
{
    int     fd;
    char    *line;
    t_token_list    *tk_list;

    fd = open(path, O_RDONLY);
    if (!fd)
    {
        perror("open ");
        // return ; ?
    }
    line = get_next_line(fd);
    ////dprintf(1, "in exec_script(), line = %s\n", line);
    data->first = tokenise(assign_type(line, ft_strlen(line)), ft_strlen(line), line);
    parse(data);
    
    // normalement, n'a qu'une seule ligne !! car ne doit pas gerer les ';' dans minishell    
    while (line)
    {
        free(line);
        line = get_next_line(fd);
    }
    unlink(path);
    close(fd);
}

int main (int argc, char **argv, char **envp)
{
    char            *pwd;
    t_data          data;
    t_ht_hash_table *hash_map;

    // initialise t_data
    data.env = NULL;
    data.env = NULL;
    data.first = NULL;

    // rajouter une condition avec isatty pour gerer le cas ./minishell | ./minishell
    
    signal(SIGINT, sigint_next_prompt);
    
    if (argc > 2)
    {
        printf("error : too many arguments\n"); //idk
        return (0);
    }
    
    if (!envp || !envp[0])
        hash_map = get_minimal_env();
    else
        hash_map = ht_get_env(envp);
    if (!hash_map)
        return (1); // do we return ? which exit status ?
    
    pwd = *get_pwd(hash_map);
    t_list *exp_hist = init_export_history(hash_map);
    data.env = hash_map;
    data.exp_history = exp_hist;
    

    if (argc == 1)
        read_lines(&data);
    else // peut avoir plus qu'un fichier ? part du principe que non ici ?
        exec_script(&data, argv[1]);
    
    
    //parse(hash_map, tokenise(hash_map, assign_type(argv[1], ft_strlen(argv[1])), ft_strlen(argv[1]), argv[1]), exp_hist);

    /* 
            ------------ Tests ------------
    */
    // if (argc != 2)
    //     return (printf("Mauvais nombre d'args : argc = %d\n", argc), 0);
    

    // t_ht_hash_table *ht = ht_new(HT_INITIAL_SIZE);
    // if (!envp)
    //     return (printf("pas d'env\n"));
    // int j;
    // for (int i = 0; envp[i]; i++)
    // {
    //     j = 0;
    //     while (envp[i][j] != '=')
    //         j++;
    //     ht_insert_item(ht, ft_substr(envp[i], 0, j), ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
    // }
    
    
    // Print the envp variable */
    // printf("\n---------------------------\n");

   
    

    // printf("key = PATH, value = %s\n", ht_search(ht, "PATH"));
}