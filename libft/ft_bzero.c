/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:41 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
/*
int	main(void)
{
	char	*test;

	test = malloc(256);
	test[0] = 'a';
	printf("String after is - |%s|\n", test);	
	ft_bzero(test, 256);
	printf("String after is - |%s|\n", test);
	return (0);
}
*/
