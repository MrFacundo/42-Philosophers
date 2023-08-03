/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:08:05 by facundo           #+#    #+#             */
/*   Updated: 2023/08/02 19:15:45 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	initialize_global_data(t_global_data *data, char **argv)
{
	data->phil_amount = ft_atoi(argv[1]);
	data->die_t = ft_atoi(argv[2]);
	data->eat_t = ft_atoi(argv[3]);
	data->sleep_t = ft_atoi(argv[4]);
	data->servings = -1;
	if (argv[5])
		data->servings = ft_atoi(argv[5]);
	data->philosophers = malloc(sizeof(t_philo) * data->phil_amount);
	sem_unlink("printf_sem");
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("terminate");
	sem_unlink("shotdown");
	sem_unlink("total_servings");
	data->term = 0;
	data->terminate = sem_open("terminate", O_CREAT, S_IRWXU, 1);
	data->forks = sem_open("forks", O_CREAT, S_IRWXU, data->phil_amount);
	data->shotdown = sem_open("shotdown", O_CREAT, S_IRWXU, 0);
	data->printf_sem = sem_open("printf_sem", O_CREAT, S_IRWXU, 1);
	data->total_servings = sem_open("total_servings", O_CREAT, S_IRWXU, 0);
	data->start_time = get_time();
	if (!data->philosophers)
		exit_routine(data, E_MALLOC);
	if (data->forks == SEM_FAILED || data->printf_sem == SEM_FAILED)
		exit_routine(data, E_SEM);
	if (check_limits(data))
		exit_routine(data, E_ARGS);
}
