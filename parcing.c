#include <unistd.h>

typedef struct philo
{
    int must_eat_count;
} t_philo;

void ft_putstr(char *s)
{
    while (*s)
    {
        write(1, s, 1);
        s++;
    }
}

int ft_atol(char *str)
{
    int count = 0;
    long result = 0;
    int sign = 1;

    while (str[count] == '\r' || str[count] == '\t' || str[count] == ' ' ||
           str[count] == '\f' || str[count] == '\v' || str[count] == '\n')
        count++;

    if (str[count] == '-' || str[count] == '+')
    {
        if (str[count] == '-')
            sign = -1;
        count++;
    }

    if (!(str[count] >= '0' && str[count] <= '9'))
        return 0;

    while (str[count] >= '0' && str[count] <= '9')
        result = result * 10 + (str[count++] - '0');

    return result * sign;
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int check_is_digit(char *av[], int ac)
{
    int i = 1;
    int j;

    while (i < ac)
    {
        j = 0;
        while (av[i][j])
        {
            if (!ft_isdigit(av[i][j++]))
                return 1;
        }
        i++;
    }
    return 0;
}

int check_num(int ac, char **av)
{
    long num;
    int i = 1;

    if (ac == 6 && ft_atol(av[5]) <= 0)
        return 1;

    while (i < ac)
    {
        num = ft_atol(av[i]);
        if (num <= 0 || (i == 1 && num > 200))
            return 1;
        i++;
    }
    return 0;
}

int main(int ac, char *av[])
{
    if (ac != 5 && ac != 6)
    {
        ft_putstr("invalid_argument\n");
        return 1;
    }

    if (check_is_digit(av, ac) != 0 || check_num(ac, av) != 0)
    {
        ft_putstr("invalid_argument\n");
        return 1;
    }

    ft_putstr("Parsing successful!\n");
    return 0;
}
