/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:17:43 by facundo           #+#    #+#             */
/*   Updated: 2023/06/12 14:17:53 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void print_results(t_global_data *global_data)
{
	int i;

	i = 0;
	printf("*********************************\n");
	printf("Results:\n");
	printf("Phil amount\tDie time\tEat time\tSleep time\tServings\n");
	printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", global_data->phil_amount, global_data->starvation_t, global_data->eat_t, global_data->sleep_t, global_data->servings);
	while (i < global_data->phil_amount)
	{
		printf("Philosopher %d remaining servings: %d\n", global_data->philosophers[i].id, global_data->philosophers[i].servings);
		i++;
	}
	printf("*********************************\n");
}