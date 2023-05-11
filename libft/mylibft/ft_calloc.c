/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 00:06:22 by srapin            #+#    #+#             */
/*   Updated: 2022/11/19 00:06:32 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*tab;

	if (size && SIZE_MAX / size < nmemb)
		return (0);
	tab = malloc(nmemb * size);
	if (!tab)
		return (0);
	ft_memset(tab, 0, nmemb * size);
	return (tab);
}
