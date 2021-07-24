#include "philo.h"

int8_t	init_philo_fields(int32_t i, t_philo *s_arr_philo)
{
	s_arr_philo[i].id = i + 1;
	s_arr_philo[i].timeout = 1;
	if (s_arr_philo[i].id % 2 == 0)
		s_arr_philo[i].timeout = 30;
	s_arr_philo[i].print = s_arr_philo[0].print;
	s_arr_philo[i].left_fork = malloc(sizeof(pthread_mutex_t));
	if (s_arr_philo[i].left_fork == NULL ||
		pthread_mutex_init(s_arr_philo[i].left_fork, NULL) != 0)
		return (-1);
	return (0);
}

int8_t	init_philo(pthread_t *threads, t_table *s_table, t_philo *s_arr_philo)
{
	int i;

	s_arr_philo[0].print = malloc(sizeof(pthread_mutex_t));
	if (s_arr_philo[0].print == NULL ||
		pthread_mutex_init(s_arr_philo[0].print, NULL) != 0)
		return (-1);
	i = -1;
	while (++i < s_table->num_philo)
	{
		s_arr_philo[i].s_table = s_table;
		if (pthread_create(&(threads[i + 1]), NULL, philo, &(s_arr_philo[i])) != 0 ||
			pthread_detach(threads[i + 1]) != 0)
			return (-1);
		if (init_philo_fields(i, s_arr_philo) != 0)
			return (-1);
		if (i != 0)
			s_arr_philo[i].right_fork = s_arr_philo[i - 1].left_fork;
		if (i + 1 == s_table->num_philo)
			s_arr_philo[0].right_fork = s_arr_philo[i].left_fork;
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
	s_monitor->s_arr_philo = malloc(sizeof(t_philo) * s_table->num_philo);
	if (s_monitor->s_arr_philo == NULL)
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

	threads = malloc(sizeof(pthread_t) * (s_table->num_philo + 1));
	if (threads == NULL)
		return (-1);
	s_monitor = malloc(sizeof(t_monitor));
	if (s_monitor == NULL)
	{
		free(threads);
		return (-1);
	}
	if (init_table(threads, s_table, s_monitor) == -1 ||
		init_philo(threads, s_table, s_monitor->s_arr_philo) == -1 ||
		init_monitor(threads, s_table, s_monitor) == -1)
	{
		free_struct(threads, s_monitor);
		return (-1);
	}
	printf("Create table: %sOK%s\n", CGRN, CNRM);
	pthread_join(threads[0], NULL);
	free_struct(threads, s_monitor);
	return (0);
}
