/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 22:23:36 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 19:50:19 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*pdst;
	unsigned char	*psrc;

	if (dst == NULL && src == NULL)
		return (dst);
	pdst = (unsigned char *)dst;
	psrc = (unsigned char *)src;
	while (n--)
		*pdst++ = *psrc++;
	return (dst);
}
