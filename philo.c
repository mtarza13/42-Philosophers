
#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	
	if (argc != 5 && argc != 6)
	{
		ft_putstr("Error: Invalid number of arguments\n");
		return (FAILURE);
	}

	if (check_is_digit(argv, argc) != 0 || check_num(argc, argv) != 0)
	{
		ft_putstr("Error: Invalid argument values\n");
		return (FAILURE);
	}
	
	if (init_program(&data, argc, argv) != 0)
	{
		ft_putstr("Error: Failed to initialize program\n");
		return (FAILURE);
	}
	
	start_simulation(&data);
	clean_up(&data);
	
	return (SUCCESS);
}