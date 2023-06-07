/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:44:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/07 23:23:09 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Determine if the current token is a control operator, ie a "&&" or a "||" */
int     is_a_ctrl_op(t_token_list *current)
{
    if (current->type == andd)
        return (1);
    if (current->type == orr
        && current->length == 2)
        return (1);
    return (0);
}

/* Détermine si un opérateur de redirection a déja été parsed pour une même commande.
Si c'est le cas, free l'espace alloué.
Utilisation : si plusieurs opérateurs identiques se suivent pour une même commande.
Exemple : echo salut > out1 >out2 > out3. Dans ce cas, ne va retenir que les infos du dernier fichier,
ie celui d'où on va lire / sur lequel on va écrire.
*/
void    assess_and_free(t_file *content)
{
    if (!content)
        return ;
    if (content->name)
        free(content->name);
    // else if (content->sep)
    //     free(content->sep);
    free(content);
    content = NULL;
}

void    remove_char(t_token_list *current, size_t index)
{
    char    *before;
    char    *after;

    before = ft_substr(current->content, 0, index);
    after = ft_substr(current->content, index + 1, current->length);
    if (!before || !after)
    {
        perror("malloc ");
        return ;
    }
    free(current->content);
    current->content = ft_strjoin(before, after);
    current->length--;
    free(before);
    free(after);
}

/* Update the t_redirect struct of the t_cmd structure. */
void    update_redirect(t_cmd *cmd, t_token_list *current)
{
    t_file  *file;
    
    file = create_file_struct_with_filename(current->next->content);
    if (current->type == l_io_redirect) // '<'
    {
        file->flag = O_RDONLY;
        add_in_redir_with_file_struct(cmd, file);
    }
    else
    {
        if (current->length == 1) // '>'
            file->flag = O_WRONLY | O_CREAT | O_TRUNC;
        else // '>>'
            file->flag = O_WRONLY | O_CREAT | O_APPEND;
        add_out_redir_with_file_struct(cmd, file);
    }
    
    
    
    // t_list ne suffit pas : doit avoir deux contents : le nom du fichier et les flags ? 
    // doit aussi avoir 

    // file = ft_calloc(sizeof(t_file), 1);
    // if (!file)
    //     return ;
    
    // red->in_type = fd;
    // file->fd = -1;
    
    // file->name = ft_strdup(current->next->content);
    // if (current->type == l_io_redirect) // '<'
    // {
    //     assess_and_free(red->in_content); // ou &(red->in_content) ?
    //     red->in_type = fd;
    //     file->flag = O_RDONLY;
    //     red->in_content = file;
    // }
    // else  // '>' ou '>>'
    // {
    //     assess_and_free(red->out_content); // ou &(red->out_content) ?
    //     red->out_type = fd;
    //     if (current->type == r_io_redirect && current->length == 1) // '>'
    //         file->flag = O_WRONLY | O_CREAT | O_TRUNC;
    //     else // '>>'
    //         file->flag = O_WRONLY | O_CREAT | O_APPEND;
    //     red->out_content = file;
    // }
}


/* Returns the substring starting at the first encountered whitespace in the initial string.
When no whitespace is encountered, NULL is returned.
If the string is either empty or null, NULL is returned. */
char    *get_whtsp_pos(char *str)
{
    int i;
    
    if (!str || !(*str))
        return (NULL);
    i = 0;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            return (str + i);
        i++;
    }
    return (NULL);
}

/*
Dès que tombe sur une succession de whitespaces en parsant le token 
-> tout ce qui venait avant devient un mot (ie token) individuel 
*/
void    set_cmd_args(t_cmd **curr_cmd, t_token_list *curr_tk, int *i)
{
    int             j;
    int             k;
    int             get_word;
    size_t          whitespace_index;
    size_t          next_whitespace_index;
    char            *whitespace_pos;
    char            *next_whitespace_pos;
    t_word_data     *wd;
    
    // removes the whitespaces that aren't in quotes in the current token 
    //  (whitespaces that could for example be a result of a variable expansion)
    if (!curr_tk->merged_words)
    {
        (*curr_cmd)->val.args[*i] = ft_strdup(curr_tk->content);
        (*i)++;
        return ;
    }
    get_word = 0;
    wd = curr_tk->merged_words;
    while (wd)
    {
        if (!wd->quotes)
        {
            j = 0;
            k = 0;
            while (j < wd->length)
            {
                if (wd->content[j] != ' ' && wd->content[j] != '\t')
                {
                    k = j;
                    while (wd->content[j] && (wd->content[j] != ' ' && wd->content[j] != '\t'))
                        j++;
                    (*curr_cmd)->val.args[*i] = ft_substr(curr_tk->content, k, j - k + 1);
                    (*i)++;
                }
                else 
                {
                    while (wd->content[j] && (wd->content[j] == ' ' || wd->content[j] == '\t'))
                        j++;
                }
            }
        }
        else
        {
            (*curr_cmd)->val.args[*i] = ft_strdup(wd->content);
            (*i)++;
        }
        wd = wd->next;
    }
}

void    set_command_attributs(t_cmd **current, t_token_list **first_tk, t_token_list *current_tk, int args_count)
{
    int i;

    i = 0;
    (*current)->val.value = ft_strdup(current_tk->content);
    if (!(*current)->val.value)
    {
        perror("malloc ");
        return ;
    }
    current_tk = current_tk->next;
    tk_del_one(first_tk, current_tk->prev); // est-ce que current_tk pointe encore au bon endroit après ça ?
    
    (*current)->val.args = malloc(sizeof(char *) * args_count);
    if (!(*current)->val.args)
    {
        perror("malloc ");
        return ;
    }
    (*current)->val.args[i] = ft_strdup(current_tk->content);
    if (!(*current)->val.args[i])
    {
        perror("malloc ");
        return ;
    }
    i++;
    while (i < args_count) //while (current_tk && current_tk->type != and && current_tk->type != or)
    {
        set_cmd_args(current, current_tk, &i);
        tk_del_one(first_tk, current_tk); // est-ce que current_tk pointe encore au bon endroit après ça ?
        current_tk = current_tk->next;
    }
    (*current)->val.args[i] = NULL;
}

int get_words_count(t_token_list *current)
{
    int             i;
    int             count;
    t_word_data     *wd;

    i = 0;
    count = 1;
    wd = current->merged_words;
    if (!wd)
        return (current->length > 0); // pas la peine de check pour les whitespaces dans ce cas car cela a deja ete fait avant l'expansion des filenames
    while (wd)
    {
        if (!wd->quotes)
        {
            while (i < wd->length) // ou wd->content[i] ?
            {
                if (wd->content[i] == ' ' || wd->content[i] == '\t')
                {
                    while (wd->content[i] 
                        && (wd->content[i] == ' ' || wd->content[i] == '\t'))
                        i++;
                    if (wd->content[i])
                        count++;
                }
                else
                {
                    //count++;
                    while (wd->content[i] 
                        && (wd->content[i] != ' ' && wd->content[i] != '\t')) 
                        i++;
                }
            }
        }
        else
            count++;
        wd = wd->next;
    }
    return (count);
}

void    add_to_cmd_list(t_cmd **last, t_cmd *new)
{
    if (!last)
        return ;
    if (!(*last)) // si la liste est vide
        *last = new;
    else
    {
        (*last)->next = new;
        *last = new;
    }  
}

// t_cmd    *get_simple_command(t_ht_hash_table *ht, t_token_list **first_tk, t_token_list *tmp)
// {
//     t_cmd   *new_cmd;
    
//     new_cmd = ft_calloc(sizeof(t_cmd), 1);
//     if (!new_cmd)
//     {
//         perror ("malloc ");
//         return (NULL);
//     }
    
//     /* Remplit t_redirect en supprimant les tokens io_redirect au fur et a mesure*/
//     while (tmp && tmp->type != andd && tmp->type != orr)
//     {
//         if (tmp->type == r_io_redirect || tmp->type == l_io_redirect)
//         {
//             update_redirect(ht, &(new_cmd->red), tmp);
//             //tmp2 = tmp;
//             tmp = tmp->next->next; // current->next ne peut etre NULL, donc c'est ok d'écrire ça
//             tk_del_one(first_tk, tmp->prev->prev);
//             tk_del_one(first_tk, tmp->prev); 
//         }
//         // else if (tmp->type == l_parenthesis) // || tmp->type == l_parenthesis) // uniquement l_parenthesis : on parsera jusqu'à la deuxieme (ie ')' dans la fonction appelée dès qu'a lu '(' )
//         // {
//         //     // appel recursif a get_pipeline(), qui se termine quand lit une ')' ? 
//         //     // (ie if (== ')' return(cmd); et ensuite linke comme il faut (ie si est précédé et/ou suivi par un '|', '||', ou '&&'))
//         //     sub_cmd = get_pipeline(ht, first_tk, &tmp); // &tmp ou current_tk ? pointent au meme endroit ici ou pas ?
//         //     if (set_pipe)
//         //     {
//         //         new_cmd->red.out_content = sub_cmd;
//         //         new_cmd->red.out_type = cmds_cmd;
//         //         new_cmd->red.out_fd = -1;
//         //         // set_pipe = 0; ne pas mettre a 0 ici, car doit renseigner (plus bas, dans if(set_pipe)) le red->in_type
//         //     }
//         //     // si precédé par un pipe (ie set_pipe = 1), alors ajoute sub_cmd au void *out_content de t_redirect de la commande precedente, et remet set_pipe à 0
//         // }
//         // else if (tmp->type == r_parenthesis)
//         // {
//         //     break; 
//         //     // return (new_cmd); il faut mettre a jour le ctrl_op avant de return 
//         // }
//         else // incrémente le nombre d'agruments à passer à execve 
//             args_count += get_words_count(tmp); // two words are separated by successive whitespaces that aren't in quotes
//     }
//     return (new_cmd);
// }

char    *random_subshell_fname(void)
{
    static size_t   files_count;
    char            *count;
    char            *filename;

    count = ft_itoa(files_count);
    filename = ft_strjoin("/tmp/subshell_args_", count);
    free(count);
    return (filename);
}


/* 
//A modifier : les parenthèses ne sont pour l'instant pas gérées ! 
t_cmd    *get_pipeline(t_token_list **first_tk, t_token_list **current_tk, t_cmd **ast)
{
    int             set_pipe;
    int             args_count;
    t_cmd           *tmp_cmd;
    t_cmd           *new_cmd;
    t_cmd           *prev_cmd;
    t_cmd           *pipeline;
    t_token_list    *tmp;
    
    set_pipe = 0;
    
    tmp = *current_tk; // utile ?
    // allocates memory for the new t_cmd variable 
    pipeline = ft_calloc(sizeof(t_cmd), 1);
    if (!pipeline)
    {
        perror ("malloc ");
        return (NULL);
    }
    
    // parse the tokens simple command by simple command 
    // parse tant que ce n'est ni un && ni un || (ok ou a modifier ?)
    while (tmp && tmp->type != andd && (tmp->type != orr || tmp->length == 1))
    {
        args_count = 0;
        tmp = *current_tk;
        
        // new_cmd = get_simple_command(ht, first_tk, tmp, new_cmd);
        while (tmp && tmp->type != andd && tmp->type != orr)
        {
            if (tmp->type == r_io_redirect || tmp->type == l_io_redirect)
            {
                // ajoute la redirection à la liste de redirections pour la commande actuelle
                update_redirect(&(new_cmd->red), tmp);
                //tmp2 = tmp;
                tmp = tmp->next->next; // current->next ne peut etre NULL, donc c'est ok d'écrire ça
                tk_del_one(first_tk, tmp->prev->prev);
                tk_del_one(first_tk, tmp->prev); 
            }
            else if (tmp->type == l_parenthesis)
            {
                int fd = open(random_subshell_fname(), O_WRONLY);
                if (fd == -1)
                {
                    perror("open ");
                }
                t_token_list *
                
            }
            else // il s'agit alors d'un nom ou d'un argument de commande
            {
                // incrémente le nombre d'arguments à passer à execve
                args_count += get_words_count(tmp); // two words are separated by successive whitespaces that aren't in quotes
                tmp = tmp->next;
            }
        }
        
        // if (set_pipe) // ie si avait un pipe avant cette commande
        // {
        //     new_cmd->red.in_type = pipeu;
        //     // que faire pour red.in_content ?
        //     set_pipe = 0;    
        // }
        
        // chope nom + args de la commande
        set_command_attributs(&new_cmd, first_tk, *current_tk, args_count);
        
        // tmp et *current_tk pointent à nouveau normalement sur le meme token à cet endroit 
        if (tmp && tmp->type == orr && tmp->length == 1) // ie s'agit d'un pipe
        {
            set_pipe = 1;
            // renseigne t_redirect pour le pipe
            
               
            prev_cmd = new_cmd; // garde en mémoire la commande actuelle 
            // new_cmd->red.out_content : doit pointer vers la commande suivante : comment faire ?
            
            *current_tk = (*current_tk)->next;
        }
        else 
        {
            
        }
    }
    
    if (is_a_ctrl_op(tmp))
        new_cmd->ctrl = (tmp->type == andd) * andd + (tmp->type == orr) * orr;
    else // ie est arrivé à la fin de la liste de tokens -> ctrl_op = ';'
        new_cmd->ctrl = pointvirgule;
    if (new_cmd->ctrl )
    
    return (new_cmd);
}
*/




// -----------------------------------------------

t_cmd *init_new_cmd(void)
{
    t_cmd *cmd;
    cmd = ft_calloc(sizeof(t_cmd), 1);
    if (!cmd)
    {
        perror("malloc ");
        return (NULL);
    }
    //init_cmd(cmd, envp); envp est en fait ht
    return (cmd);
}

t_cmd   *get_commands_list(t_ht_hash_table *ht, t_token_list **first_tk)
{
    int             i;
    int             set_pipe;
    int             fd_subshell;
    int             args_count;
    char            *subshell_filename;
    t_token_list    *pipeline_start_tk;
    t_token_list    *cmd_start_tk;
    t_token_list    *current_tk;
    t_cmd           *ast;
    t_cmd           *pipeline_start_cmd;
    t_cmd           *current_cmd;
    t_cmd           *before_pipe_cmd;
    t_cmd           *before_ctrl_op_cmd;

    ast = init_new_cmd();
    if (!ast)
        return (NULL);
    set_here_docs(ht, first_tk);

    /* Ne modifie pas directement ast car il s'agit de la variable que renvoie à la fin,
    et veut ainsi que pointe encore sur la racine de l'arbre quand est récupéré par l'exec. */
    current_cmd = ast; 

    set_pipe = 0;
    before_pipe_cmd = NULL;
    before_ctrl_op_cmd = NULL;
    
    pipeline_start_tk = *first_tk; // le pointeur sur le premier token du premier pipeline est initialisé à pointeur que va renvoyer à l'exec à la fin
    pipeline_start_cmd = init_new_cmd();
    if (!pipeline_start_cmd)
    {
        perror("malloc ");
        // free en cascade
    }

    // parcours la liste de tokens un par un
    while (pipeline_start_tk)
    {
        current_tk = pipeline_start_tk;
        current_cmd = pipeline_start_cmd;
        
        // tant que n'est ni un '&&' ni un '||'
        // ie peut avoir des pipes, mais ne touche ici pas à la variable next de t_cmd
        while (pipeline_start_tk && pipeline_start_tk->type != and && (pipeline_start_tk->type != or || pipeline_start_tk->length == 1))
        {
            cmd_start_tk = current_tk;
            args_count = 0;
            // tant que n'est ni un '|', '||', ou '&&'
            while (current_tk && current_tk->type != and && current_tk->type != or)
            {
                // creer et updated une t_cmd
                

                if (current_tk->type == l_io_redirect || current_tk->type == r_io_redirect)
                {
                    update_redirect(current_cmd, current_tk);
                    current_tk = current_tk->next->next; 
                    
                    // ca free bien comme il faut ?
                    tk_del_one(first_tk, current_tk->next);
                    tk_del_one(first_tk, current_tk);
                }
                else if (current_tk->type == l_parenthesis)
                {
                    subshell_filename = random_subshell_fname();
                    fd_subshell = open(subshell_filename, O_CREAT | O_WRONLY, 00700);
                    if (fd_subshell == -1)
                        perror("open ");
                    i = 0;
                    //cmd_start_tk = current_tk;
                    while (current_tk && current_tk->type != r_parenthesis)
                    {
                        if (write(fd_subshell, current_tk->content, current_tk->length) == -1) // vérifier si current_tk->length est bien à jour !
                            perror ("write ");
                        current_tk = current_tk->next;
                    }
                    
                    add_val_to_cmd(current_cmd, ft_strjoin("minishell ", subshell_filename));
                    //current_cmd->val 
                    free(subshell_filename);
                    subshell_filename = NULL;
                }
                else // ie s'agit d'un nom ou option de commande 
                {
                    args_count += get_words_count(current_tk); // two words are separated by successive whitespaces that aren't in quotes
                    current_tk = current_tk->next;
                }
            }
            current_cmd->red.next_cmd = NULL;
            set_command_attributs(&current_cmd, first_tk, cmd_start_tk, args_count);

            // si current_cmd est après un pipe
            if (set_pipe) 
            {
                if (before_pipe_cmd) // simple verification mais normalement, si set_pipe = 1 alors ne peut pas etre NULL
                {
                    before_pipe_cmd->red.next_cmd = current_cmd;
                    before_pipe_cmd = NULL; // ne modifie pas la structure globale, si ?
                }
                set_pipe = 0;
            }
            
            // si current_cmd est avant un pipe
            if (current_tk && current_tk->type == or && current_tk->length == 1) // '|'
            {
                set_pipe = 1;
                
                // garde en mémoire current_cmd pour pouvoir la linker à la commande suivante une fois qu'aura déclaré et rempli la variable correspondante
                before_pipe_cmd = current_cmd;
                
                current_cmd = init_new_cmd();
                if (!current_cmd)
                {
                    // tout free en cascade
                }
                
                current_tk = current_tk->next; // faire ca ici ?
            }
        }
        if (is_a_ctrl_op(current_tk))
        {
            current_cmd->ctrl = (current_tk->type == andd) * andd + (current_tk->type == orr) * orr;
            before_ctrl_op_cmd = pipeline_start_cmd;
            
            pipeline_start_cmd = init_new_cmd();
            if (!pipeline_start_cmd)
            {
                perror("malloc ");
                // free en cascade
            }
        }
        else // ie est arrivé à la fin de la liste de tokens -> ctrl_op = ';'
            current_cmd->ctrl = pointvirgule;
    }
    return (ast);
        


        
        /*  get_pipeline() : 
            Parse the tokens simple command by simple command, until an entire pipeline has been parsed
            -> Parse the tokens' list until reading a control operator, ie a && or a || 
        set_pipeline(first_tk, &pipeline_start_tk, &ast);
        */
        
        // add_to_cmd_list(&ast, current_cmd); // passer le last plutot que ast (ie le first), car sinon doit entièrement reparcourir la liste a chaque fois que veut ajouter un élément à la fin 
        //pipeline_start_tk = pipeline_start_tk->next; // le fait ici ou sera deja fait a la fin de get_pipeline() ?
}


/*

N'utiliser que cette fonction pour écrire le nom + args d'une commande dans t_cmd

void init_cmd_and_add_val(t_cmd *cmd, t_ht_hash_table *ht, char *args)
{
	char *test = ft_strdup(args);
	// init_cmd(cmd, envp);
	add_cmdval_to_cmd(cmd, test);
}

*/




/* Retourne une liste chainée de pipelines, avec chaque {commande + redirections} 
regroupées dans un élément de type t_cmd.

Parse le flux de tokens :

Déclare et initialise une variable t_cmd.
Tant que ne tombe pas sur un opérateur de controle (ie un && ou un ||), 
met à jour cette commande avec les tokens que l'on lit :
    -> Dès que tombe sur un opérateur de redirection, met à jour l'attribut t_redirect de t_cmd
    -> Sinon, met à jour le nom et les arguments de la commande.
Dès que tombe sur un opérateur de controle :
    -> Met à jour l'attribut ctrl_op de la variable t_cmd actuelle.
    -> Ajoute la variable t_cmd actuelle à la liste de commandes.
    -> Déclare et initialise une nouvelle variable t_cmd.
    -> Répète les opérations ci-dessus jusqu'à tomber à nouveau sur un opérateur de 
       contrôle, ou jusqu'à être arrivé à la fin de la liste de tokens.


(echo un | (echo deux && echo trois))
*/

// t_cmd  *get_commands(t_ht_hash_table *ht, t_token_list **first)
// {
//     t_token_list    *current_tk;
//     t_cmd           *new_cmd;
//     t_cmd           *current_cmd;
//     t_cmd           *first_cmd;

//     current_tk = (*first);
    
//     // first_cmd = ft_calloc(sizeof(t_cmd), 1);
//     // if (!first_cmd)
//     //     return (NULL);
//     set_here_docs(ht, first);
//     first_cmd = get_simple_command(ht, first, &current_tk);
//     if (!first_cmd)
//     {
//         perror ("malloc ");
//         return (NULL);
//     }
//     current_cmd = first_cmd;
//     while (current_tk)
//     {
//         // parse la liste de tokens jusqu'à arriver à la fin d'un pipeline (ie un && ou un || i guess ?)
//         new_cmd = get_simple_command(ht, first, &current_tk);
//         if (!new_cmd)
//         {
//             perror ("malloc ");
//             return (NULL); // ?
//         }
//         add_to_cmd_list(&current_cmd, new_cmd);
//     }
//     return (first_cmd);
// }
