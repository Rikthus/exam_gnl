#include "get_next_line.h"

int	ft_strlen_n(char *str, int mod)
{
	int	i;

	i = 0;
	if (mod == 0)
	{
		while (str[i])
			i++;
	}
	else
	{
		while (str[i] && str[i] != '\n')
			i++;
		if (str[i] == '\n')
			i++;
	}
	return (i);
}

char	*strjoin_n(char *line, char *buf)
{
	int	i;
	int	j;
	int	k;
	char	*join;

	i = 0;
	j = 0;
	join = malloc(sizeof(*join) * (ft_strlen_n(line, 0) + ft_strlen_n(buf, 1) + 1));
	if (!join)
		return (NULL);
	while (line[i])
	{
		join[i] = line[i];
		i++;
	}
	free(line);
	while (buf[k] == '\0' && k < BUFFER_SIZE)
		k++;
	while (buf[k] && buf[k] != '\n')
	{
		join[i + j] = buf[k];
		buf[k] = '\0';
		j++;
		k++;
	}
	if (buf[k] == '\n')
	{
		join[i + j] = buf[k];
		buf[k] = '\0';
		j++;
	}
	join[i + j] = '\0';
	return (join);
}

int	checkbuf(char *buf)
{
	int	i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		if (buf[i] != '\0')
			return (1);
		i++;
	}
	return (0);
}

int	is_line_done(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*reader(int fd, char *line, char *buf)
{
	int	bytes;

	while (!is_line_done(line))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(line);
			return (NULL);
		}
		line = strjoin_n(line, buf);
		if (!line)
		{
			free(line);
			return (NULL);
		}
		if (line[0] == '\0' && bytes == 0)
			return (NULL);
		else if (bytes == 0)
			return (line);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static	char	buf[BUFFER_SIZE + 1] = {0};

	if (BUFFER_SIZE < 1 || BUFFER_SIZE > 100000 || fd < 0 || fd > OPEN_MAX)
		return (NULL);
	line = malloc(1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	if (checkbuf(buf))
		line = strjoin_n(line, buf);
	line = reader(fd, line, buf);
	return (line);
}

int	main(int argc, char **argv)
{
	(void)argc;
	int	i;
	int	fd;

	i = 0;
	fd = open(argv[1], O_RDONLY);
	while (i < 10)
	{
		printf("%s", get_next_line(fd));
		i++;
	}
	close(fd);
}
