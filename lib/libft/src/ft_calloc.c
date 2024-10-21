/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:12:12 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 20:13:19 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pos;
	size_t	total;

	total = nmemb * size;
	if (size && (total / size) != nmemb)
		return (NULL);
	pos = malloc(total);
	if (!pos)
		return (NULL);
	ft_bzero(pos, total);
	return (pos);
}
