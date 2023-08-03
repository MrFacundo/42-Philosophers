/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:48 by facundo           #+#    #+#             */
/*   Updated: 2023/08/02 19:39:30 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	exit_routine(t_global_data *global_data, char *msg)
{
	if (global_data->philosophers)
		free(global_data->philosophers);
	if (msg)
		printf("%s\n", msg);
	exit(1);
}

void	init_philo(t_global_data *data, int i)
{
	data->philosophers[i].id = i + 1;
	data->philosophers[i].last_serving_t = get_time();
	data->philosophers[i].g_data = data;
	data->philosophers[i].lock = sem_open("lock", O_CREAT, S_IRWXU, 1);
	data->philosophers[i].is_eating = 0;
}

void	waiter(t_global_data *global_data)
{
	int	i;
	int	status;


	if (global_data->servings > 0)
	{
		i = -1;
		while (++i < global_data->phil_amount)
			sem_wait(global_data->total_servings);
		
		i = -1;
		while (++i < global_data->phil_amount)
			sem_post(global_data->shotdown);
	}

	i = -1;
	status = 0;

	while (++i < global_data->phil_amount)
		waitpid(-1, &status, 0);
	return ;
}

int	helper(t_global_data *data)
{
	int		i;
	t_philo	*ph;

	i = -1;
	while (++i < data->phil_amount)
	{
		ph = &data->philosophers[i];
		ph->pid = fork();
		if (!ph->pid)
		{
			init_philo(data, i);
			if (pthread_create(&ph->lifecycle, 0, routine, ph)
			|| pthread_create(&ph->monitoring, 0, monitor_starvation, ph)
			|| pthread_create(&ph->shotdown, 0, monitor_shotdown, data)
			|| pthread_join(ph->lifecycle, 0)
			|| pthread_join(ph->monitoring, 0)
			|| pthread_join(ph->shotdown, 0)
			)
				return (1);
			sem_close(ph->lock);
			sem_close(data->terminate);
			sem_close(data->forks);
			sem_close(data->shotdown);
			sem_close(data->printf_sem);
			sem_close(data->total_servings);

			
			free(data->philosophers);
			exit (0);
		}
		else if (ph->pid < 0)
			return (1);
	}
	waiter(data);
	return (0);	
}

int	main(int argc, char **argv)
{
	t_global_data	global_data;

	check_argc(argc);
	initialize_global_data(&global_data, argv);
	if (helper(&global_data))
		exit_routine(&global_data, E_THREAD);
	sem_close(global_data.terminate);
	sem_close(global_data.forks);
	sem_close(global_data.shotdown);
	sem_close(global_data.printf_sem);
	sem_close(global_data.total_servings);
	sem_unlink("printf_sem");
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("terminate");
	sem_unlink("shotdown");
	free(global_data.philosophers);
	return (0);
}
