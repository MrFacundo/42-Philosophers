/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:13:59 by facundo           #+#    #+#             */
/*   Updated: 2023/07/27 17:28:15 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo_bonus.h"

static void	print_status(t_philo *philo, char *status)
{
	long	timestamp;
	
	timestamp = get_time() - philo->g_data->start_time;
	if (!philo->g_data->servings)
		return ;
	sem_wait(philo->printf_sem);
	if (!ft_strncmp(S_EAT, status, ft_strlen(status)))
		philo->last_serving_t = get_time();
	printf("%ld %d %s\n", timestamp, philo->id, status);
	if (ft_strncmp(S_DIED, status, ft_strlen(status)))
		sem_post(philo->printf_sem);
}

long	get_time(void)
{
	long			timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, 0);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}
void	*monitor_starvation(void *arg)
{
	long	ts;
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->g_data->servings != 0)
	{
		ft_usleep(5);
		ts = get_time();
		if (ts - philo->last_serving_t > philo->g_data->die_t)
		{
			print_status(philo, S_DIED);
			exit(philo->id);
		}
		ft_usleep(500);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_

	philo = (t_philo *)arg;
	while (philo->g_data->servings != 0)
	{
		print_status(philo, S_THINK);
		sem_wait(philo->forks);
		print_status(philo, S_FORK);
		sem_wait(philo->forks);
		print_status(philo, S_FORK);
		print_status(philo, S_EAT);
		ft_usleep(philo->g_data->eat_t * 1000);
		philo->last_serving_t = get_time();
		print_status(philo, S_SLEEP);
		sem_post(philo->forks);
		sem_post(philo->forks);
		ft_usleep(philo->g_data->sleep_t * 1000);
		philo->g_data->servings--;
	}
	return (0);
}
