/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_public.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 02:09:28 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:11:35 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "priv.h"

void init_node(t_node *node)
{
	node->content = NULL;
	node->next = NULL;
	node->type = none;
	node->ctrl =none;
	node->ret = -1;
	node->already_exec = false;
}

t_node *create_empty_node()
{
	t_node *node;

	node = malloc(sizeof(node));
	return node;
}