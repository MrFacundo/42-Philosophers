/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:13:59 by facundo           #+#    #+#             */
/*   Updated: 2023/08/02 09:33:09 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	timestamp = get_time() - philo->g_data->start_time;
	if (!philo->g_data->servings)
		return ;
	sem_wait(philo->g_data->printf_sem);
	if (!ft_strncmp(S_DIED, status, ft_strlen(status)) && check_is_dead(timestamp, philo))
		printf("%ld %d %s\n", timestamp, philo->id, status);
	printf("philo->g_data->terminate: %d\n", philo->g_data->term);
	//check if someone died so that we don't print anything and shut down
    sem_wait(philo->g_data->terminate);
    if (philo->g_data->term == 1)
    {
        sem_post(philo->g_data->terminate);
        sem_post(philo->g_data->printf_sem);
        return ;
    }
    sem_post(philo->g_data->terminate);

	if (!ft_strncmp(S_EAT, status, ft_strlen(status)))
	{
		sem_wait(philo->lock);
		philo->last_serving_t = get_time();
		sem_post(philo->lock);
	}
	printf("%ld %d %s\n", timestamp, philo->id, status);
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
	while (check_servings(philo))
	{
		ft_usleep(5);
		ts = get_time();
		if (check_is_dead(ts, philo))
		{
			 //tell everybody to shut down because of death
            sem_wait(philo->g_data->terminate);
            philo->g_data->term = 1;
            sem_post(philo->g_data->terminate);

			print_status(philo, S_DIED);
			return (0);
		}
	}
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
			            //check if we need to shut down
            sem_wait(philo->g_data->terminate);
            if (philo->g_data->term == 1)
            {
                sem_post(philo->g_data->terminate);
                return (NULL);
            }
            sem_post(philo->g_data->terminate);

			if (philo->g_data->phil_amount == 1)
			{
				print_status(philo, S_FORK);
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
				printf("putting fork back\n");
				sem_post(philo->g_data->forks);
				ft_usleep(10);
			}
		}
	}
	return (0);
}
