/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:13:59 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 15:43:31 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	if (check_term(philo))
		return ;
	timestamp = get_time() - philo->g_data->start_time;
	sem_wait(philo->g_data->printf_lock);
	if (!ft_strncmp(S_DIED, status, ft_strlen(status)) && !check_term(philo))
	{
		printf("%ld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->g_data->printf_lock);
		return ;
	}
	else
	{
		printf("%ld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->g_data->printf_lock);
	}
}

long	get_time(void)
{
	long			timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, 0);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}

void	exit_routine(t_global_data *global_data, char *msg)
{
	if (global_data->philosophers)
		free(global_data->philosophers);
	if (msg)
		printf("%s\n", msg);
	exit(1);
}

void	close_semaphores(t_global_data *data)
{
	sem_close(data->term_lock);
	sem_close(data->forks);
	sem_close(data->shotdown);
	sem_close(data->printf_lock);
	sem_close(data->total_servings);
	sem_unlink("printf_lock");
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("term_lock");
	sem_unlink("shotdown");
}

void	close_child_process(t_philo *ph)
{
	sem_close(ph->lock);
	sem_close(ph->g_data->forks);
	sem_close(ph->g_data->term_lock);
	sem_close(ph->g_data->shotdown);
	sem_close(ph->g_data->printf_lock);
	sem_close(ph->g_data->total_servings);
	free(ph->g_data->philosophers);
	exit(0);
}
