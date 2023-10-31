/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_atoi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:34:40 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/30 11:26:21 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*sign_check(char *str, long long *minus)
{
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*minus = *minus * -1;
		str++;
	}
	return (str);
}

long long	overflow_check(long long num, long long i)
{
	if (i > 10)
		return (-1);
	if (num < 0 || num > 2147483647)
		return (-1);
	return (num);
}

long long	philo_atoi(char *str)
{
	long long	minus;
	long long	i;
	long long	num;

	i = 0;
	num = 0;
	minus = 1;
	while (*str >= 9 && *str <= 13)
		str++;
	str = sign_check(str, &minus);
	if (!(*str >= '0' && *str <= '9'))
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		str++;
		if (num > 0)
			i++;
	}
	if (*str != '\0')
		return (-1);
	return (overflow_check(num * minus, i));
}
