/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hben-laz <hben-laz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:11:22 by hben-laz          #+#    #+#             */
/*   Updated: 2024/05/20 21:59:12 by hben-laz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	substr_len;
	size_t	i;
	char	*substr;

	i = 0;
	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (NULL);
	substr_len = len;
	if (start + substr_len > s_len)
		substr_len = s_len - start;
	substr = (char *)malloc((substr_len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	while (i < substr_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[substr_len] = '\0';
	return (substr);
}

static int	chek_new_line(char *buf, int *n)
{
	int	i;

	i = 0;
	if (!buf)
		return (0);
	while (buf[i])
	{
		if (buf[i] == '\n')
		{
			*n = i;
			return (1);
		}
		i++;
		*n = i;
	}
	return (0);
}

static char	*read_function(char **buf, char *buf_save, int n, int fd)
{
	int			nb_read;
	char		*tmp;

	nb_read = 1;
	while (nb_read > 0)
	{
		nb_read = read(fd, *buf, BUFFER_SIZE);
		if (nb_read == 0 && buf_save == 0)
			return (free(*buf), *buf = NULL, NULL);
		if (nb_read == 0)
			break ;
		if (nb_read < 0)
			return (free(buf_save), free(*buf), buf_save = NULL,
				*buf = NULL, NULL);
		(*buf)[nb_read] = '\0';
		tmp = buf_save;
		buf_save = ft_strjoin(buf_save, *buf);
		if (buf_save == NULL)
			return (free(tmp), NULL);
		free (tmp);
		tmp = NULL;
		if (chek_new_line(buf_save, &n))
			break ;
	}
	return (buf_save);
}

static char	*get_next_line(int fd)
{
	char		*buf;
	char static	*buf_save;
	char		*tmp;
	char		*line;
	int			n;

	n = 0;
	buf = malloc(BUFFER_SIZE * sizeof(char) + 1);
	if (buf == 0)
		return (free(buf_save), buf_save = NULL, NULL);
	buf_save = read_function(&buf, buf_save, n, fd);
	free (buf);
	chek_new_line(buf_save, &n);
	line = ft_substr(buf_save, 0, n + 1);
	if (line == NULL)
		return (free(buf_save), buf_save = NULL, NULL);
	tmp = buf_save;
	if (buf_save != NULL)
		buf_save = ft_substr(buf_save, n + 1, ft_strlen(buf_save));
	free (tmp);
	tmp = NULL;
	return (line);
}

void	here_doc(char *limiter, int ac, int *fd)
{
	char	*line;

	if (ac < 6)
		ft_error("min 6 arg : \n", "fail", 0, 0);
	fd[1] = open("herd.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	fd[0] = open("herd.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd[1] == -1 || fd[0] == -1)
		ft_error("open fail : \n", "fail", 0, -1);
	unlink("herd.txt");
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& ft_strlen(limiter) == ft_strlen(line) - 1)
			break ;
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	free(line);
}
