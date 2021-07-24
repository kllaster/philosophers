#include "philo.h"

int8_t	check_params(t_table *s_table)
{
	if (s_table->num_philo <= 0)
		return (-1);
	else if (s_table->time_life <= 0)
		return (-1);
	else if (s_table->time_eat <= 0)
		return (-1);
	else if (s_table->time_sleep <= 0)
		return (-1);
	else if (s_table->num_count_eat < 0)
		return (-1);
	return (0);
}

void parse_params(int argc, char *argv[], t_table *s_table)
{
	int i = 0;

	while (++i < argc)
	{
		if (i == 1)
			s_table->num_philo = ft_atoi(argv[i]);
		else if (i == 2)
			s_table->time_life = ft_atoi(argv[i]);
		else if (i == 3)
			s_table->time_eat = ft_atoi(argv[i]);
		else if (i == 4)
			s_table->time_sleep = ft_atoi(argv[i]);
		else if (i == 5)
			s_table->num_count_eat = ft_atoi(argv[i]);
	}
}

int	main(int argc, char *argv[])
{
	t_table	*s_table;

	if (!(argc == 5 || argc == 6))
		return (1);
	s_table = (t_table *)malloc(sizeof(t_table));
	if (s_table == NULL)
		return (-1);
	memset(s_table, 0, sizeof(t_table));
	printf("Parse params: %sOK%s\n", CGRN, CNRM);
	parse_params(argc, argv, s_table);
	if (check_params(s_table) == -1)
	{
		printf("Check params: %sX%s\n", CRED, CNRM);
		return (1);
	}
	printf("Check params: %sOK%s\n", CGRN, CNRM);
	if (create_table(s_table) == -1)
	{
		printf("Create table: %sX%s\n", CRED, CNRM);
		return (1);
	}
	free(s_table);
	return (0);
}