/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_pub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:11:02 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:11:02 by srapin            #+#    #+#             */
/*   Updated: 2023/06/10 21:41:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	t_list	*lst;

	lst = ft_lstnew(file_struct);
	ft_lstadd_back(&(cmd->red.files), lst);
}

void	init_redirections(t_redirect *red)
{
	red->files = NULL;
	red->in_fd = -1;
	red->out_fd = -1;
	red->next_cmd = NULL;
}

t_redirect	*create_redir(void)
{
	t_redirect	*red;

	red = malloc(sizeof(t_redirect));
	if (!red)
		return (NULL);
	init_redirections(red);
	return (red);
}
