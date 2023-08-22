/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:41 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
/*
#include <stdio.h>
int	main (void)
{
	int	c1;
	int	c2;

	c1 = 'a';
	c2 = '5';
	printf("char: %c is digit or not: %d\n", c1, ft_isdigit(c1));
	printf("char: %c is digit or not: %d\n", c2, ft_isdigit(c2));
}
*/
