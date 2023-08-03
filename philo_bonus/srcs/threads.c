/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 11:59:26 by facundo           #+#    #+#             */
/*   Updated: 2023/08/03 12:05:04 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*monitor_shotdown(void *arg)
{
	t_global_data	*data;

	data = (t_global_data *)arg;
	sem_wait(data->shotdown);
	sem_wait(data->term_lock);
	data->term = 1;
	sem_post(data->term_lock);
	sem_post(data->shotdown);
	//printf("returning from monitor shotdown\n");
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
				//printf("philo %d died\n", philo->id);
				print_status(philo, S_DIED);
				sem_post(philo->g_data->shotdown);
				i = 0;
				while (i++ <= philo->g_data->phil_amount)
					sem_post(philo->g_data->total_servings);
			}
			//printf("ph %d returning from monitor starvation\n", philo->id);
			return (0);
		}
	}
	sem_post(philo->g_data->total_servings);
	//printf("ph %d returning from monitor starvation because servings are enough\n", philo->id);
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
            sem_wait(philo->g_data->term_lock);
            if (philo->g_data->term == 1)
            {
                sem_post(philo->g_data->term_lock);
				//printf("philo %d returning from routine 1\n", philo->id);
                return (0);
            }
            sem_post(philo->g_data->term_lock);

			if (philo->g_data->phil_amount == 1)
			{
				print_status(philo, S_FORK);
				//printf("philo %d returning from routine 2\n", philo->id);
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
	//printf("ph %d returning from routine because servings are enough\n", philo->id);
	return (0);
}
