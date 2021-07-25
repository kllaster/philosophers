#include "philo.h"

void	init_philo(t_table *s_table, t_philo *s_arr_philo)
{
	int16_t	i;

	sem_unlink("philo_print");
	s_arr_philo[0].print = sem_open("philo_print", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("philo_forks");
	s_arr_philo[0].forks = sem_open("philo_forks", O_CREAT | O_EXCL,
			0644, s_table->num_philo);
	i = -1;
	while (++i < s_table->num_philo)
	{
		s_arr_philo[i].s_table = s_table;
		s_arr_philo[i].id = i + 1;
		s_arr_philo[i].print = s_arr_philo[0].print;
		s_arr_philo[i].forks = s_arr_philo[0].forks;
	}
}

int8_t	init_table(t_table *s_table, t_monitor *s_monitor)
{
	memset(s_monitor, 0, sizeof(t_monitor));
	s_monitor->s_table = s_table;
	s_monitor->s_processes = malloc(sizeof(t_processes));
	if (s_monitor->s_processes == NULL)
	{
		free(s_monitor);
		return (-1);
	}
	s_monitor->s_arr_philo = malloc(sizeof(t_philo) * s_table->num_philo);
	if (s_monitor->s_arr_philo == NULL)
	{
		free(s_monitor->s_processes);
		free(s_monitor);
		return (-1);
	}
	memset(s_monitor->s_arr_philo, 0, sizeof(t_philo) * s_table->num_philo);
	init_philo(s_table, s_monitor->s_arr_philo);
	s_monitor->s_table->start_table = time_unix_ms();
	return (0);
}

void	init_processes(t_monitor *s_monitor)
{
	int16_t		i;
	t_processes	*s_processes;

	i = -1;
	s_processes = s_monitor->s_processes;
	while (++i < s_monitor->s_table->num_philo)
	{
		if (s_monitor->s_table->num_philo == 1)
			s_monitor->s_arr_philo[i].time_death = s_monitor->s_table->start_table + s_monitor->s_table->time_life;
		s_monitor->s_arr_philo[i].last_eat = s_monitor->s_table->start_table;
		s_processes->pid = fork();
		if (!s_processes->pid)
			philo(&(s_monitor->s_arr_philo[i]));
		else if (s_processes->pid < 0)
			exit(3);
		if ((i + 1) < s_monitor->s_table->num_philo)
		{
			s_processes->next = malloc(sizeof(t_processes));
			if (s_processes->next == NULL)
				exit(4);
			s_processes = s_processes->next;
			s_processes->next = NULL;
		}
	}
}

int8_t	create_table(t_table *s_table)
{
	int32_t		sig;
	int32_t		i;
	t_monitor	*s_monitor;

	s_monitor = malloc(sizeof(t_monitor));
	if (s_monitor == NULL)
		return (-1);
	if (init_table(s_table, s_monitor) == -1)
	{
		free_struct(s_monitor);
		return (-1);
	}
	printf("Create table: %sOK%s\n", CGRN, CNRM);
	init_processes(s_monitor);
	i = -1;
	while (++i < s_monitor->s_table->num_philo)
	{
		if (waitpid(-1, &sig, 0) < 0)
			exit(5);
		if (sig == 256)
		{
			free_struct(s_monitor);
			return (0);
		}
	}
	free_struct(s_monitor);
	return (0);
}
