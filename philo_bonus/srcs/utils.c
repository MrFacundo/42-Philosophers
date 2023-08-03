/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:13:59 by facundo           #+#    #+#             */
/*   Updated: 2023/08/03 11:59:53 by facundo          ###   ########.fr       */
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
	// printf("philo %d term %d\n", philo->id, philo->g_data->term);
	if (!ft_strncmp(S_DIED, status, ft_strlen(status)) && !check_term(philo))
	{
		printf("%ld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->g_data->printf_lock);
		return ;
	}
	else {
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
