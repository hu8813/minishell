/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:41 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
/*
#include <ctype.h>
#include <stdio.h>
int	main (void)
{
	char	c1;
	char	c2;

	c1 = 'a';
	c2 = '5';
	printf("char: %c is alphanumeric or not: %d\n", c1, ft_isalnum(c1));
	printf("char: %c is alphanumeric or not: %d\n", c2, ft_isalnum(c2));
}
*/
