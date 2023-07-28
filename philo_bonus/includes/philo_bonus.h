/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:01 by facundo           #+#    #+#             */
/*   Updated: 2023/07/28 14:12:02 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H

# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/time.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>

// limits
# define MIN_PHIL_AMOUNT 1
# define MAX_PHIL_AMOUNT 200
# define MIN_T 60
# define MAX_T 60000

// messages
# define S_FORK "has taken a fork"
# define S_EAT "is eating"
# define S_SLEEP "is sleeping"
# define S_THINK "is thinking"
# define S_DIED "died"
# define E_MALLOC "Error: malloc failed\n"
# define E_THREAD "Error: pthread function failed\n"
# define E_ARGS "Error: bad arguments\n"
# define E_SEM "Error: semaphore function failed\n"

typedef struct s_philo
{
	int						id;
	int						pid;
	long					last_serving_t;
	pthread_t				monitoring;
	pthread_t				lifecycle;
	sem_t					*lock;
	struct s_global_data	*g_data;
}	t_philo;

typedef struct s_global_data
{
	int						phil_amount;
	int						die_t;
	int						eat_t;
	int						sleep_t;
	int						servings;
	sem_t					*printf_sem;
	sem_t					*forks;
	t_philo					*philosophers;
	long					start_time;
}	t_global_data;

/* checks.c */
int		check_limits(t_global_data *data);
void	check_argc(int argc);
int		check_is_dead(long timestamp, t_philo *philo);
int		check_servings(t_philo *philo);

/* debug.c */
void	print_results(t_global_data *global_data);

/* init.c */
void	initialize_global_data(t_global_data *data, char **argv);

/* libft_utils.c */
int		ft_atoi(char *str);
int		ft_strlen(char *s);
int		ft_strncmp(char *str_1, char *str_2, int n);
void	ft_usleep(unsigned int n);

/* main.c */
void	exit_routine(t_global_data *global_data, char *msg);

/* subroutines.c */
void	eat(t_philo *philo);
void	leave_forks(t_philo *philo);
void	nap(t_philo *philo);

/* utils.c */
void	print_status(t_philo *philo, char *status);
long	get_time(void);
void	*monitor_starvation(void *arg);
void	*routine(void *arg);

#endif
