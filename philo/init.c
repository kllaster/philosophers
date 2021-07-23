#include "philo.h"

void free_struct(pthread_t *threads, t_monitor *s_monitor)
{
	int i;

	i = -1;
	while (++i < s_monitor->s_table->num_philo)
	{
		pthread_mutex_destroy(s_monitor->s_arr_philo[i].left_fork);
		free(s_monitor->s_arr_philo[i].left_fork);
	}
	free(threads);
	free(s_monitor->s_arr_philo);
	free(s_monitor);
}

int8_t init_philo(pthread_t *threads, t_table *s_table, t_philo *s_arr_philo)
{
	int i;

	if (!(s_arr_philo[0].mutex_death = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t))))
		return (-1);
	if (pthread_mutex_init(s_arr_philo[0].mutex_death, NULL) != 0)
		return (-1);
	i = 0;
	while (++i <= s_table->num_philo)
	{
		s_arr_philo[i - 1].id = i;
		s_arr_philo[i - 1].s_table = s_table;
		if (pthread_create(&(threads[i]), NULL, philo, &(s_arr_philo[i - 1])) != 0)
			return (-1);
		if (pthread_detach(threads[i]) != 0)
			return (-1);
		s_arr_philo[i - 1].mutex_death = s_arr_philo[0].mutex_death;
		if (!(s_arr_philo[i - 1].left_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t))))
			return (-1);
		if (pthread_mutex_init(s_arr_philo[i - 1].left_fork, NULL) != 0)
			return (-1);
		if ((i - 1) != 0)
			s_arr_philo[i - 1].right_fork = s_arr_philo[i - 2].left_fork;
		if (i == s_table->num_philo)
			s_arr_philo[0].right_fork = s_arr_philo[i - 1].left_fork;
	}
	return (0);
}

int8_t	init_monitor(pthread_t *threads, t_table *s_table, t_monitor *s_monitor)
{
	s_monitor->s_table = s_table;
	if (pthread_create(&(threads[0]), NULL, monitor, s_monitor) != 0)
	{
		free(threads);
		free(s_monitor);
		return (-1);
	}
	return (0);
}

int8_t	init_table(pthread_t *threads, t_table *s_table, t_monitor *s_monitor)
{
	memset(s_monitor, 0, sizeof(t_monitor));
	if (!(s_monitor->s_arr_philo = (t_philo *)malloc(sizeof(t_philo) * s_table->num_philo)))
	{
		free(threads);
		free(s_monitor);
		return (-1);
	}
	memset(s_monitor->s_arr_philo, 0, sizeof(t_philo) * s_table->num_philo);
	return (0);
}

int8_t	create_table(t_table *s_table)
{
	pthread_t	*threads;
	t_monitor	*s_monitor;

	if (!(threads = (pthread_t *)malloc(sizeof(pthread_t) * (s_table->num_philo + 1))))
		return (-1);
	if (!(s_monitor = (t_monitor *)malloc(sizeof(t_monitor))))
	{
		free(threads);
		return (-1);
	}
	if (init_table(threads, s_table, s_monitor) == -1)
		return (-1);
	if (init_philo(threads, s_table, s_monitor->s_arr_philo) == -1)
		return (-1);
	if (init_monitor(threads, s_table, s_monitor) == -1)
		return (-1);
	printf("Create table: %sOK%s\n", CGRN, CNRM);
	pthread_join(threads[0], NULL);
	free_struct(threads, s_monitor);
	return (0);
}
