#include "../inc/ft_ping.h"

void	print_usage(void)
{
	printf("Usage: ft_ping [-v] [-h] <hostname>\n");
	printf("Options:\n");
	printf("  -v    Verbose output\n");
	printf("  -h    Show this help message\n");
}

void	parse_arguments(int argc, char **argv, t_ping *ping)
{
	int	opt;

	if (argc < 2)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "vh")) != -1)
	{
		switch (opt)
		{
		case 'v':
			ping->options.verbose = true;
			break ;
		case 'h':
			ping->options.help = true;
			break ;
		case '?':
			print_usage();
			exit(EXIT_FAILURE);
		}
	}
	if (optind < argc)
		ping->hostname = argv[optind];
}
