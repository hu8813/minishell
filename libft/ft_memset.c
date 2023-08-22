/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:42 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
	{
		ptr[n] = (unsigned char)c;
	}
	return (s);
}
/*
int	main(void)
{
   int len;
	const char	str[];
	const char	ch;
   char *ret;

   str[] = "http://www.tutorialspoint.com";
   ch = '.';
   ret = ft_memset(str, ch, 3);
   printf("String after |%c| is - |%s|\n", ch, ret);
   
	return(0);
}
*/
