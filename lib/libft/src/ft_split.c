/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 21:24:35 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/19 21:24:35 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	token_count(char const *str, char c)
{
	size_t	index;
	size_t	count;
	size_t	length;

	count = 0;
	length = 0;
	index = -1;
	while (index++, str[index])
	{
		if (str[index] == c)
		{
			if (length != 0)
				count++;
			length = 0;
		}
		else
			length++;
	}
	if (length != 0)
		return (count + 1);
	return (count);
}

char	**token_fill(char **str, char const *s, char c)
{
	int	i;
	int	str_i;
	int	minus;

	i = 0;
	str_i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		minus = i;
		while (s[i] && s[i] != c)
			i++;
		str[str_i] = (char *)malloc(i - minus + 1);
		if (!str[str_i])
			return (NULL);
		ft_strlcpy(str[str_i++], s + minus, i - minus + 1);
	}
	str[str_i] = NULL;
	return (str);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

char	**ft_split(char const *s, char c)
{
	int		tokens;
	char	**array;

	if (s == NULL)
		return (NULL);
	tokens = token_count(s, c);
	array = (char **)malloc((tokens + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	if (!token_fill(array, s, c))
	{
		free_str(array);
		return (NULL);
	}
	return (array);
}
