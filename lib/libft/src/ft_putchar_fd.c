/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 20:15:21 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 20:15:42 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	ssize_t	bytes_written;

	bytes_written = write(fd, &c, 1);
	if (bytes_written == -1)
	{
		perror("write failed");
		exit(EXIT_FAILURE);
	}
}
