/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_start_mlx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keanders <keanders@student.42london.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:39:46 by keanders          #+#    #+#             */
/*   Updated: 2025/03/05 13:48:26 by keanders         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	random_color()
{
	int		red;
	int		green;
	int		blue;

	red = rand() % 256;
	green = rand() % 256;
	blue = rand() % 256;
	return ((0x000 << 24) | (red << 16) | (green << 8) | blue);
}

void	ft_put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	ft_draw_line(t_data *data, t_vertx vertex1, t_vertx vertex2, int color)
{
	t_linep2p	line;

	line.step = 1;
	line.i = 0;
	line.x = 0;
	line.y = 0;

	line.dx = abs(vertex2.x - vertex1.x);
	line.dy = abs(vertex2.y - vertex1.y);
	if (vertex1.x < vertex2.x)
		line.step = 1;
	else
		line.step = -1;
	if (vertex1.y < vertex2.y)
		line.step_y = 1;
	else
		line.step_y = -1;
	line.err = line.dx + line.dy;
	line.x = vertex1.x;
	line.y = vertex1.y;
	while (1)
	{
		ft_put_pixel(data, line.x, line.y, color);
		if (line.x == vertex2.x && line.y == vertex2.y)
			break ;
		line.e2 = 2 * line.err;
		if (line.e2 >= line.dy)
		{
			if (line.x == vertex2.x)
				break ;
			line.err += line.dy;
			line.x += line.step;
		}
		if (line.e2 <= line.dx)
		{
			if (line.y == vertex2.y)
				break ;
			line.err += line.dx;
			line.y += line.step_y;
		}
	}
}

int	handle_key(int keycode, void *param)
{
	(void)param;
	if (keycode == 53)
		exit(0);
	return (0);
}

void generate_point_clusters(t_data *data, int num_clusters, int points_per_cluster, int step_size, int steps_per_point)
{
	int		cluster_centres[num_clusters][2];
	int		i;
	int		j;
	int		x;
	int		y;
	int		s;

	i = 0;
	while (i < num_clusters)
	{
		cluster_centres[i][0] = rand() % 1024;
		cluster_centres[i][1] = rand() % 768;
		j = 0;
		while (j < points_per_cluster)
		{
			x = cluster_centres[i][0];
			y = cluster_centres[i][1];
			s = 0;
			while (s < steps_per_point)
			{
				x += (rand() % (2 * step_size + 1) - step_size);
				y += (rand() % (2 * step_size + 1) - step_size);
				x = fmax(0, fmin(1023, x));
				y = fmax(0, fmin(767, y));
				s++;
			}
			ft_put_pixel(data, x, y, random_color());
			j++;
		}
		i++;
	}
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_data	img;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1024, 768, "Clustered Random Dots");
	img.img = mlx_new_image(mlx, 1024, 768);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	// Seed the random number generator
	srand(time(NULL));
	// Generate clustered points
	generate_point_clusters(&img, 142, 2000, 25, 1); // 5 clusters, 2000 points per cluster
	// Display the image in the window
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	// Handle window closing and key press
	mlx_hook(mlx_win, 17, 0, (int (*)())exit, 0);
	mlx_key_hook(mlx_win, handle_key, NULL);
	// Start the MLX loop
	mlx_loop(mlx);
	return (0);
}
