/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 03:40:31 by srapin            #+#    #+#             */
/*   Updated: 2022/11/13 00:41:45 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_filltab(unsigned int nb, int lenght, int sign)
{
	char	*number;

	number = ft_calloc(sizeof(char), lenght + 1);
	if (!number)
		return (NULL);
	if (sign < 0)
		number[0] = '-';
	while (nb)
	{
		number[--lenght] = nb % 10 + 48;
		nb /= 10;
	}
	return (number);
}

char	*ft_itoa(int n)
{
	long int	nb;
	int			s;
	int			l;

	s = 1;
	l = 1;
	nb = n;
	if (nb == 0)
		return (ft_strdup("0"));
	if (nb < 0)
	{
		s = -1;
		nb *= -1;
	}
	while (nb > 9)
	{
		nb /= 10;
		l++;
	}
	return (ft_filltab(n * s, l + (s < 0), s));
}
