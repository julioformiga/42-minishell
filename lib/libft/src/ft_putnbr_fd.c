/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 20:43:45 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 20:43:48 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	*nbrs;

	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else
	{
		nbrs = ft_itoa(n);
		if (nbrs == NULL)
			return ;
		ft_putstr_fd(nbrs, fd);
		free(nbrs);
	}
}
