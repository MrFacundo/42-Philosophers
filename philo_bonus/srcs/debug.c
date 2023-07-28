/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 14:11:17 by facundo           #+#    #+#             */
/*   Updated: 2023/07/28 14:14:09 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_results(t_global_data *global_data)
{
	int	i;

	i = 0;
	printf("*********************************\n");
	printf("Results:\n");
	printf("amount\tDie time\tEat time\tSleep time\tServings\n");
	printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", global_data->phil_amount,
		global_data->die_t, global_data->eat_t, global_data->sleep_t,
		global_data->servings);
	printf("*********************************\n");
}
