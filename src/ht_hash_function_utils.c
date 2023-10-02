/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_hash_function_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:41:40 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search(int low, int high, int nbr)
{
	int long	mid;

	mid = (low + high) / 2;
	if (low > high)
		return (0);
	if ((mid * mid) == nbr)
		return (mid);
	if ((mid * mid) < nbr && ((mid + 1) * (mid + 1)) > nbr)
		return (mid + 1);
	if ((mid * mid) < nbr)
		return (search(mid + 1, high, nbr));
	else
		return (search(low, mid, nbr));
}

int	ft_sqrt(int nbr)
{
	return (search(0, nbr, nbr));
}

int	search_prime(int nb, int sqrt, int div)
{
	if (nb <= 2)
		return (nb == 2);
	if (nb == 3)
		return (1);
	if (nb % div == 0)
		return (0);
	if (div > sqrt)
		return (1);
	else
		return (search_prime(nb, sqrt, div + 1));
}

int	is_prime(int nb)
{
	int	sqrt;

	sqrt = ft_sqrt(nb);
	return (search_prime(nb, sqrt, 2));
}

/* Returns the next prime after x, or x if x is prime */
int	next_prime(int nb)
{
	if (nb < 2)
		return (2);
	if (is_prime(nb))
		return (nb);
	else
		return (next_prime(nb + 1));
}
