/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 11:59:26 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 16:13:51 by facu             ###   ########.fr       */
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
		ts = get_time();
		if (check_is_dead(ts, philo))
		{
			if (!check_term(philo))
			{
				print_status(philo, S_DIED);
				sem_post(philo->g_data->shotdown);
				i = 0;
				while (i++ <= philo->g_data->phil_amount)
					sem_post(philo->g_data->total_servings);
			}
			return (0);
		}
	}
	sem_post(philo->g_data->total_servings);
	return (0);
}

// void	*routine(void *arg)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)arg;
// 	while (philo->g_data->servings)
// 	{
// 		print_status(philo, S_THINK);
// 		if (sem_wait(philo->g_data->forks) == 0)
// 		{
// 			if (check_term(philo))
// 			{
// 				sem_post(philo->g_data->forks);
// 				return (0);
// 			}
// 			if (philo->g_data->phil_amount == 1)
// 			{
// 				print_status(philo, S_FORK);
// 				return (0);
// 			}
// 			if (sem_wait(philo->g_data->forks) == 0)
// 			{
// 				eat(philo);
// 				leave_forks(philo);
// 				nap(philo);
// 			}
// 			else
// 			{
// 				sem_post(philo->g_data->forks);
// 				ft_usleep(10);
// 			}
// 		}
// 	}
// 	return (0);
// }

int try_pickup_forks(t_philo *philo) {
    if (sem_wait(philo->g_data->forks) != 0) {
        return 0;
    }
    
    t_global_data *g_data = philo->g_data;

    if (sem_wait(g_data->term_lock) != 0) {
        sem_post(g_data->forks);
        return 0;
    }

    if (g_data->term == 1) {
        sem_post(g_data->term_lock);
        sem_post(g_data->forks);
        return 0;
    }

    sem_post(g_data->term_lock);

    if (g_data->phil_amount == 1) {
        print_status(philo, S_FORK);
        sem_post(g_data->forks);
        return 0;
    }

    if (sem_wait(g_data->forks) != 0) {
        sem_post(g_data->forks);
        return 0;
    }

    return 1;
}

void *routine(void *arg) {
    t_philo *philo = (t_philo *)arg;

    while (philo->g_data->servings) {
        print_status(philo, S_THINK);
        if (try_pickup_forks(philo)) {
            eat(philo);
            leave_forks(philo);
            nap(philo);
        } else {
            ft_usleep(10);
        }
    }

    return (NULL);
}