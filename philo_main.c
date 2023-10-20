/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:43:58 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/20 22:31:36 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_error(t_philo *philo)
{
	philo->dead = 2;
	philo_die(philo, 0);
}

void	philo_die(t_philo *philo, long long time)
{
	char	*time_str;

	if (philo->dead == 2)
		write(2, "error\n", 6) == -1;
	else
	{
		time_str = philo_itoa(time);
		if (time_str == NULL)
			write_error(philo);
		else if (write(1, "timestamp_in_ms ", 16) == -1)
			write_error(philo);
		else if (write(1, time_str, philo_strlen(time_str)) == -1)
			write_error(philo);
		else if (write(1, "died", 4) == -1)
			write_error(philo);
	}
}

void	philo_sleeping(t_philo *philo)
{
	
}

void	philo_eating(t_philo *philo)
{
	struct timeval	now;
	long long		remaining_time;
	long long		sleep_time;

	if (gettimeofday(&now, NULL) != 0)
		philo->dead = 2;
	if (philo->dead != 0)
	{
		if (philo->dead == 1)
			philo->last_eating_time = (long long)now.tv_sec * 1000;
		return ;
	}
	remaining_time = philo->arg->time_to_die - \
				((long long)now.tv_sec * 1000 - philo->last_eating_time);
	if (philo->arg->time_to_eat > remaining_time)
	{
		sleep_time = philo->arg->time_to_eat - remaining_time;
		usleep(sleep_time);
		philo->dead = 1;
		philo->last_eating_time = (long long)now.tv_sec * 1000 + sleep_time;
		return ;
	}
	usleep(philo->arg->time_to_eat);
	philo->last_eating_time = (long long)now.tv_sec * 1000;
	philo->eat_num++;
}

void	philo_thinking(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	philo_eating(philo);
	if (philo->dead != 0)
	{
		philo_die(philo, philo->last_eating_time - philo->start_time);
		return ;
	}
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	philo_sleeping(philo);
}

void	*arg_check(t_arg *arg)
{
	if (arg->time_to_die <= 0 || arg->time_to_sleep <= 0)
	{
		free(arg);
		return (NULL);
	}
	if (arg->time_to_eat <= 0 || arg->eat_num_limit <= 0)
	{
		free(arg);
		return (NULL);
	}
	return (arg);
}

void	*arg_setting(int argc, char **argv)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (arg == NULL)
		return (NULL);
	if (philo_atoi(argv[2]) <= 0)
	{
		free(arg);
		return (NULL);
	}
	arg->time_to_die = philo_atoi(argv[3]);
	arg->time_to_eat = philo_atoi(argv[4]);
	arg->time_to_sleep = philo_atoi(argv[5]);
	if (argc == 6)
	{
		arg->eat_num_limit = pilo_atoi(argv[6]);
		arg->is_have_eat_num_limit = 1;
	}
	else
	{
		arg->eat_num_limit = 1;
		arg->is_have_eat_num_limit = 0;
	}
	return (arg_check(arg));
}

void	*make_philos(t_philo *philos, t_arg *arg, int philo_num)
{
	int	i;

	i = 1;
	while (i <= philo_num)
	{
		philos[i].arg = arg;
		philos[i].eat_num = 0;
		philos[i].dead = 0;
		if (pthread_mutex_init(philos[i].left_fork, NULL) != 0)
			return (philo_error(philos, arg, i - 1));
		i++;
	}
	i = 1;
	while (i <= philo_num)
	{
		philos[i].right_fork = philos[(i % philo_num) + 1].left_fork;
		i++;
	}
	return (philos);
}

void	*start_philos(t_philo *philos, int philo_num)
{
	struct timeval	tv;
	int				i;

	if (gettimeofday(&tv, NULL) != 0)
		return (philo_error(philos, philos[1].arg, philo_num));
	i = 1;
	while (i <= philo_num)
	{
		philos[i].start_time = (long long)tv.tv_sec * 1000;
		philos[i].last_eating_time = (long long)tv.tv_sec * 1000;
		if (pthread_create(philos[i].thread_id, NULL, philo_thinking, \
		&philos[i]) != 0)
			return (philo_error(philos, philos[1].arg, philo_num));
		i++;
		philos[i].start_time = (long long)tv.tv_sec * 1000;
		philos[i].last_eating_time = (long long)tv.tv_sec * 1000;
		if (i > philo_num)
			break ;
		if (pthread_create(philos[i].thread_id, NULL, philo_sleeping, \
		&philos[i]) != 0)
			return (philo_error(philos, philos[1].arg, philo_num));
		i++;
	}
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_arg	*arg;
	int		philo_num;

	if (!(argc == 5 || argc == 6))
		return (1);
	arg = arg_setting(argc, argv);
	if (arg == NULL)
		return (1);
	philo_num = philo_atoi(argv[2]);
	philos = malloc(sizeof(t_philo) * (philo_num + 1));
	if (make_philos(philos, arg, philo_num) == NULL)
		return (1);
	if (start_philos(philos, philo_num) == NULL)
		return (1);
}
