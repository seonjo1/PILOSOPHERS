/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_library.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:51:43 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/26 12:31:11 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	philo_get_time(int sec, int usec)
{
	return ((long long)(sec * 1000) + (long long)(usec / 1000));
}

int	philo_print(t_philo *philo, long long time, char *str)
{
	char	*time_str;
	char	*name;
	int		flag;

	flag = 0;
	time_str = philo_itoa(time - philo->start_time);
	name = philo_itoa(philo->philo_num);
	if (time_str == NULL)
		flag = 2;
	else if (write(1, time_str, philo_strlen(time_str)) == -1)
		flag = 2;
	else if (write(1, " ms ", 4) == -1)
		flag = 2;
	else if (write(1, name, philo_strlen(name)) == -1)
		flag = 2;
	else if (write(1, " ", 1) == -1)
		flag = 2;
	else if (write(1, str, philo_strlen(str)) == -1)
		flag = 2;
	return (flag);
}

int	philo_error(void)
{
	write(1, "error\n", 6);
	return (1);
}

void	philo_free_mutex(pthread_mutex_t *mutex)
{
	pthread_mutex_destroy(mutex);
	free(mutex);
}

void	*philo_free(t_philo *philos, t_arg *arg, int n, int flag)
{
	int	i;

	free(arg);
	if (n == -1)
		free(philos[1].print_mutex);
	else if (n < 0)
		philo_free_mutex(philos[1].print_mutex);
	i = 1;
	while (i <= n)
		philo_free_mutex(philos[i++].left_fork);
	if (flag == 3)
		philo_free_mutex(philos[i].left_fork);
	else if (flag == 1 || flag == 2)
		free(philos[i].left_fork);
	i = 1;
	while (i <= n)
		philo_free_mutex(philos[i++].dead_mutex);
	if (flag > 1)
		free(philos[i].dead_mutex);
	free(philos);
	return (NULL);
}

int	philo_is_dead_n(t_philo *philo, int n)
{
	int	flag;

	pthread_mutex_lock(philo->dead_mutex);
	if (philo->dead == n)
		flag = 1;
	else
		flag = 0;
	pthread_mutex_unlock(philo->dead_mutex);
	return (flag);
}

void	philo_change_dead(t_philo *philo, int n)
{
	pthread_mutex_lock(philo->dead_mutex);
	philo->dead = n;
	pthread_mutex_unlock(philo->dead_mutex);
}

void	philo_join(t_philo *philos)
{
	int	i;

	i = 1;
	while (i <= philos->arg->number_of_philo)
		pthread_join(philos[i++].thread_id, NULL);
}

void	philo_print_mutex(t_philo *philo, long long time, char *str)
{
	pthread_mutex_lock(philo->print_mutex);
	if (philo_is_dead_n(philo, 0))
		philo_change_dead(philo, philo_print(philo, time, str));
	pthread_mutex_unlock(philo->print_mutex);
}
