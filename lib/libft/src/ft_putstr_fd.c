/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:26 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 19:53:32 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	ssize_t	bytes_written;

	if (!s || !fd)
		return ;
	while (*s)
	{
		bytes_written = write(fd, s, 1);
		if (bytes_written == -1) {
			perror("write failed");
			exit(EXIT_FAILURE);
		}
	}
}
