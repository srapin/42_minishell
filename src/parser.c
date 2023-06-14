/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 02:12:27 by Helene            #+#    #+#             */
/*   Updated: 2023/06/15 01:26:48 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Parser : part of our Command Line Interpreter that calls the lexical scanner to retrieve tokens.
It then constructs an Abstract Syntax Tree, or AST, out of these tokens. 
This AST is what we’ll pass to the executor to be executed.
*/


// ((cat out) && ls | grep "c" | (wc -l)) || echo "hello world" 
// env | grep "a"$test | wc -l && ls


/*
Ensures the command abides to the following syntax rules :
    - No '|' at the very beginning or end of the command line, and of each command list (ie check between () as well)
    - No "&&" or "||" at the very beginning or end of the command line, and of each command list (ie check between () as well)
    - No more than two consecutives '<' or '>'
    - For '<' and '>' symbols : if what follows isn't a word, a dollar or an asterisk : syntax error
    
Check for unclosed parentheses
Check for unclosed quotes. 

((cat out) && ls | (wc -l)) || echo "hello world" 
*/


void    print_type(t_token_list *current)
{
        if (current->type == word)
            printf("current->type = word \n");
        else if (current->type == simple_quote)
            printf("current->type = simple quote\n");
        else if (current->type == double_quote)
            printf("current->type = double quote \n");
        else if (current->type == r_parenthesis)
            printf("current->type = )\n");
        else if (current->type == l_parenthesis)
            printf("current->type = (\n");
        else if (current->type == r_io_redirect)
            printf("current->type = >\n");
        else if (current->type == l_io_redirect)
            printf("current->type = <\n");
        else if (current->type == and)
            printf("current->type = &\n");
        else if (current->type == or)
            printf("current->type = |\n");
        else if (current->type == whitespace)
            printf("current->type = whitespace\n");
        current = current->next;
}

void print_tokens(t_token_list *first)
{
    t_token_list *current;

    current = first;
    while (current)
    {
        print_type(current);
        // print_merged_words(current);
        if (!*(current->content))
            printf("current->content = \'0\'\n\n");
        else
            printf("current->content = %s\n\n", current->content);
        current = current->next;
    }
    printf("----------------------\n\n");
}

void    print_ast(t_cmd *ast)
{
    t_cmd *current_pipeline;
    t_cmd *current_simple_cmd;

    current_pipeline = ast;
    while (current_pipeline)
    {
        current_simple_cmd = current_pipeline;
        while (current_simple_cmd)
        {
            printf("command : \n");
            printf("\tname : %s\n\targs : \n", current_simple_cmd->val.value);
            for (int i = 0; current_simple_cmd->val.args[i]; i++)
                printf("\t\targ %d : %s\n", i, current_simple_cmd->val.args[i]);
            printf("Pipe ? ");
            if (current_simple_cmd->red.next_cmd)
                printf("Yes\n");
            else 
                printf("No\n");
            
            current_simple_cmd = current_simple_cmd->red.next_cmd;
        }
        if (current_pipeline->ctrl == and)
            printf("control_op = '&&' \n");
        else if (current_pipeline->ctrl == or)
            printf("control_op = '||' \n");
        else
            printf("control_op = ';' \n");
        current_pipeline = current_pipeline->next;
    }
}

void    parse(t_ht_hash_table *ht, t_token_list *first, t_list *exp_hist)
{    
    int wstatus;
    
    wstatus = ft_syntax(&first);
    if (wstatus) // ie la syntaxe n'était pas bonne 
    {
        // met à jour le dernier exit status
        return ;
    }
    
    perform_variable_exp(ht, &first);
    
    // tej les quotes en regroupant les <word> qui se suivent et ne sont pas 
    // séparés par des whitespaces ou des operateurs de controle autres que des quotes.
    // Cela ne veut pas dire que cela enleve les possibles nouveaux whitespaces 
    // dans un <word>, apparus suite à l'expansion de variables.
    // On ne fait cela (traites les chaines de caractères  qu'après avoir géré les wildcards et avoir
    // séparé les tokens entre redirections et commandes
    delete_quotes(&first);
    group_words(&first);

    perform_wildcard_exp(ht, &first);

    set_here_docs(ht, &first);

    t_cmd *ast = get_ast(ht, &first, exp_hist);
    //print_ast(ast);

    exec_cmds(ast);
}