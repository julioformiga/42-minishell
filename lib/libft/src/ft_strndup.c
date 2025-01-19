/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 02:38:56 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/19 02:38:56 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;

	if (!s1 || n == 0)
		return (NULL);
	str = malloc(sizeof(char) * (n + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	while (++i < n)
		str[i] = s1[i];
	str[i] = '\0';
	return (str);
}
