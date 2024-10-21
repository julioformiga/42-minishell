/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_itoa_base.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 03:54:13 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/11/14 03:54:13 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_reverse_string(char *tab)
{
	int	counter;
	int	buffer;
	int	size;

	size = ft_strlen(tab);
	counter = 0;
	while (counter < size / 2)
	{
		buffer = tab[counter];
		tab[counter] = tab[size - counter - 1];
		tab[size - counter - 1] = buffer;
		counter++;
	}
}

static int	count_numbers(unsigned long nbr, int basesize)
{
	int	i;

	i = 0;
	if (!nbr)
		return (1);
	while (nbr)
	{
		nbr = nbr / basesize;
		i++;
	}
	return (i);
}

char	*ft_uitoa_base(
	unsigned long nbr, char *base_to, unsigned long base_size)
{
	int		i;
	char	*res;

	res = ft_calloc(count_numbers(nbr, base_size) + 1, sizeof(char *));
	i = 0;
	if (nbr == 0)
		res[i++] = base_to[0];
	while (nbr)
	{
		res[i] = base_to[nbr % base_size];
		i++;
		nbr = nbr / base_size;
	}
	res[i] = '\0';
	ft_reverse_string(res);
	return (res);
}
