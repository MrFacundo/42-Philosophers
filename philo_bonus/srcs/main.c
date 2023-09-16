/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:48 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 15:42:09 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	waiter(t_global_data *global_data)
{
	int	i;
	int	status;

	if (global_data->servings > 0)
	{
		i = 0;
		while (i++ < global_data->phil_amount)
			sem_wait(global_data->total_servings);
		i = 0;
		sem_post(global_data->shotdown);
	}
	i = 0;
	status = 0;
	while (i++ < global_data->phil_amount)
		waitpid(-1, &status, 0);
	return ;
}

int	fork_philosopher(t_global_data *data, int i)
{
	t_philo	*ph;

	ph = &data->philosophers[i];
	ph->pid = fork();
	if (!ph->pid)
	{
		init_philo(data, i);
		if (pthread_create(&ph->routine, 0, routine, ph)
			|| pthread_create(&ph->monitor_feeding, 0, monitor_starvation, ph)
			|| pthread_create(&ph->monitor_shotdown, 0, monitor_shotdown, data)
			|| pthread_join(ph->routine, 0)
			|| pthread_join(ph->monitor_feeding, 0)
			|| pthread_join(ph->monitor_shotdown, 0))
			return (1);
		close_child_process(ph);
	}
	else if (ph->pid < 0)
		return (1);
	return (0);
}

int	helper(t_global_data *data)
{
	int	i;

	i = -1;
	while (++i < data->phil_amount)
	{
		if (fork_philosopher(data, i))
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
	close_semaphores(&global_data);
	free(global_data.philosophers);
	return (0);
}
