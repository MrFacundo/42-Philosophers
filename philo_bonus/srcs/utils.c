/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:13:59 by facundo           #+#    #+#             */
/*   Updated: 2023/08/02 20:04:19 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	timestamp = get_time() - philo->g_data->start_time;
	// if (!philo->g_data->servings)
	// 	return ;
	sem_wait(philo->g_data->printf_sem);
	// printf("philo %d term %d\n", philo->id, philo->g_data->term);
	if (!ft_strncmp(S_DIED, status, ft_strlen(status)) && check_term(philo))
	{
		printf("%ld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->g_data->printf_sem);
		return ;
	}

	//check if someone died so that we don't print anything and shut down
    sem_wait(philo->g_data->terminate);
    if (philo->g_data->term == 1)
    {
        sem_post(philo->g_data->terminate);
        sem_post(philo->g_data->printf_sem);
        return ;
    }
    sem_post(philo->g_data->terminate);

	printf("%ld %d %s\n", timestamp, philo->id, status);
	sem_post(philo->g_data->printf_sem);
}

long	get_time(void)
{
	long			timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, 0);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}

void	*monitor_shotdown(void *arg)
{
	t_global_data	*data;

	data = (t_global_data *)arg;
	sem_wait(data->shotdown);
	sem_wait(data->terminate);
	data->term = 1;
	sem_post(data->terminate);
	sem_post(data->shotdown);
	printf("returning from monitor shotdown\n");
	return (0);
}

void	*monitor_starvation(void *arg)
{
	long	ts;
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	while (check_servings(philo))
	{
		ft_usleep(5);
		ts = get_time();
		if (check_is_dead(ts, philo))
		{
			if (!check_term(philo))
            {
				// sem_wait(philo->g_data->terminate);
				printf("philo %d died\n", philo->id);
				print_status(philo, S_DIED);
				sem_post(philo->g_data->shotdown);
				philo->g_data->term = 1;
				i = 0;
				while (i++ <= philo->g_data->phil_amount)
					sem_post(philo->g_data->total_servings);
				// sem_post(philo->g_data->terminate);
			}
			printf("ph %d returning from monitor starvation\n", philo->id);
			return (0);
		}
	}
	sem_post(philo->g_data->total_servings);
	printf("ph %d returning from monitor starvation because servings are enough\n", philo->id);
	return (0);
}

void	*routine(void *arg)	
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->g_data->servings)
	{
		print_status(philo, S_THINK);

		if (sem_wait(philo->g_data->forks) == 0)
		{
            sem_wait(philo->g_data->terminate);
            if (philo->g_data->term == 1)
            {
                sem_post(philo->g_data->terminate);
				printf("philo %d returning from routine 1\n", philo->id);
                return (0);
            }
            sem_post(philo->g_data->terminate);

			if (philo->g_data->phil_amount == 1)
			{
				print_status(philo, S_FORK);
				printf("philo %d returning from routine 2\n", philo->id);

				return (0);
			}
			if (sem_wait(philo->g_data->forks) == 0)
			{
				eat(philo);
				leave_forks(philo);
				nap(philo);
			}
			else
			{
				sem_post(philo->g_data->forks);
				ft_usleep(10);
			}
		}
	}
	printf("ph %d returning from routine because servings are enough\n", philo->id);

	return (0);
}
