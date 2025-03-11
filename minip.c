/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minip.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: keanders <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/03/07 15:27:30 by keanders		  #+#	#+#			 */
/*   Updated: 2025/03/07 15:27:36 by keanders		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minip.h"

// Function to draw a line between two points on the back buffer
void draw_line_to_buffer(t_data *data, t_point start, t_point end, int color)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);
	int sx = (start.x < end.x) ? 1 : -1;
	int sy = (start.y < end.y) ? 1 : -1;
	int err = dx - dy;

	while (1)
	{
		// Draw pixel to the back buffer
		int pixel = start.y * data->size_line + start.x * (data->bpp / 8);
		*(unsigned int *)(data->back_data + pixel) = color;

		if (start.x == end.x && start.y == end.y)
			break;
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			start.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			start.y += sy;
		}
	}
}

// Function to draw a square using lines on the back buffer
void draw_square_to_buffer(t_data *data, t_square square, int color)
{
	// Draw lines between the vertices
	draw_line_to_buffer(data, square.top_left, square.top_right, color);	// Top line
	draw_line_to_buffer(data, square.top_right, square.bottom_right, color); // Right line
	draw_line_to_buffer(data, square.bottom_right, square.bottom_left, color); // Bottom line
	draw_line_to_buffer(data, square.bottom_left, square.top_left, color);   // Left line
}

void fill_random_blob(void *img, int width, int height, int bpp, int size_line)
{
	char *img_data = mlx_get_data_addr(img, &bpp, &size_line, NULL);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int pixel = y * size_line + x * (bpp / 8);
			*(unsigned int *)(img_data + pixel) = rand() % 0xFFFFFF; // Random color
		}
	}
}

void clear_buffer(t_data *data, void *buffer, int color)
{
	char *buffer_data = mlx_get_data_addr(buffer, &data->bpp, &data->size_line, &data->endian);
	for (int y = 0; y < WINDOW_HEIGHT; y++)
	{
		for (int x = 0; x < WINDOW_WIDTH; x++)
		{
			int pixel = y * data->size_line + x * (data->bpp / 8);
			*(unsigned int *)(buffer_data + pixel) = color;
		}
	}
}

void render_frame(t_data *data)
{
	// Clear the back buffer
	clear_buffer(data, data->back_buffer, 0x000000); // Clear with black

	// Define a square (for example)
	draw_square_to_buffer(data, data->dynamic_square, WHITE_PIXEL);
	// Debug: Print square coordinates
	/*printf("Drawing square at: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n",
		   my_square.top_left.x, my_square.top_left.y,
		   my_square.top_right.x, my_square.top_right.y,
		   my_square.bottom_right.x, my_square.bottom_right.y,
		   my_square.bottom_left.x, my_square.bottom_left.y);*/
		// Draw the square to the back buffer
	draw_square_to_buffer(data, data->dynamic_square, WHITE_PIXEL);

	// Render sprites to the back buffer
	mlx_put_image_to_window(data->mlx, data->win, data->player.img,
	data->player.x, data->player.y);
	mlx_put_image_to_window(data->mlx, data->win, data->enemy.img,
	data->enemy.x, data->enemy.y);
	mlx_put_image_to_window(data->mlx, data->win, data->obstacle.img,
	data->obstacle.x, data->obstacle.y);
	// Copy the back buffer to the window
	mlx_put_image_to_window(data->mlx, data->win, data->back_buffer, 0, 0);
}

void apply_gravity(t_sprite *sprite)
{
	if (!sprite->grounded)
		sprite->vy += sprite->gravity;
}

void update_position(t_sprite *sprite)
{
	sprite->x += sprite->vx;
	sprite->y += sprite->vy;
}

int check_collision(t_sprite *a, t_sprite *b)
{
	return (a->x < b->x + b->width &&
			a->x + a->width > b->x &&
			a->y < b->y + b->height &&
			a->y + a->height > b->y);
}

void handle_collision(t_sprite *sprite, t_sprite *obstacle)
{
	if (check_collision(sprite, obstacle))
	{
		if (sprite->vy > 0) // Collision from above
		{
			sprite->y = obstacle->y - sprite->height;
			sprite->vy = 0;
			sprite->grounded = 1;
		}
		else if (sprite->vy < 0) // Collision from below
		{
			sprite->y = obstacle->y + obstacle->height;
			sprite->vy = 0;
		}
		if (sprite->vx > 0) // Collision from the left
		{
			sprite->x = obstacle->x - sprite->width;
			sprite->vx = 0;
		}
		else if (sprite->vx < 0) // Collision from the right
		{
			sprite->x = obstacle->x + obstacle->width;
			sprite->vx = 0;
		}
	}
}

/*void handle_input(t_data *data, int keycode) // Corrected function signature
{
	if (keycode == KEY_LEFT)
		move_square(&data->dynamic_square, -MOVEMENT_STEP, 0);
	else if (keycode == KEY_RIGHT)
		move_square(&data->dynamic_square, MOVEMENT_STEP, 0);
	else if (keycode == KEY_UP)
		move_square(&data->dynamic_square, 0, -MOVEMENT_STEP);
	else if (keycode == KEY_DOWN)
		move_square(&data->dynamic_square, 0, MOVEMENT_STEP);
	else if (keycode == KEY_SPACE && data->player.grounded)
	{
		data->player.vy = -15;
		data->player.grounded = 0;
	}
}*/

int key_hook(int keycode, t_data *data) // Ensure correct parameter type
{
	if (keycode == KEY_ESC)
		close_window(data); // Close the window and exit
	else
		handle_input(data, keycode); // Pass data
	return (0);
}

int close_window(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}

int game_loop(t_data *data)
{
	apply_gravity(&data->player);
	update_position(&data->player);
	handle_collision(&data->player, &data->obstacle);
	render_frame(data);
	return (0);
}

int main(void)
{
	t_data data;

	//printf("Initializing MLX...\n");
	data.mlx = mlx_init();
	if (!data.mlx)
	{
		fprintf(stderr, "Error: mlx_init failed\n");
		return (1);
	}
	//printf("MLX initialized successfully.\n");

	//printf("Creating window...\n");
	data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "minip Shapes");
	if (!data.win)
	{
		fprintf(stderr, "Error: mlx_new_window failed\n");
		return (1);
	}
	//printf("Window created successfully.\n");

	//printf("Creating front buffer...\n");
	data.front_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!data.front_buffer)
	{
		fprintf(stderr, "Error: mlx_new_image failed for front_buffer\n");
		return (1);
	}
	//printf("Front buffer created successfully.\n");

	//printf("Creating back buffer...\n");
	data.back_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!data.back_buffer)
	{
		fprintf(stderr, "Error: mlx_new_image failed for back_buffer\n");
		return (1);
	}
	data.back_data = mlx_get_data_addr(data.back_buffer, &data.bpp, &data.size_line, &data.endian);
	//printf("Back buffer created successfully.\n");

	//printf("Initializing player sprite...\n");
	data.player.img = mlx_new_image(data.mlx, 50, 50);
	if (!data.player.img)
	{
		fprintf(stderr, "Error: mlx_new_image failed for player\n");
		return (1);
	}
	//printf("Player sprite initialized successfully.\n");

	//printf("Initializing enemy sprite...\n");
	data.enemy.img = mlx_new_image(data.mlx, 50, 50);
	if (!data.enemy.img)
	{
		fprintf(stderr, "Error: mlx_new_image failed for enemy\n");
		return (1);
	}
	//printf("Enemy sprite initialized successfully.\n");

	//printf("Initializing obstacle sprite...\n");
	data.obstacle.img = mlx_new_image(data.mlx, 100, 20);
	if (!data.obstacle.img)
	{
		fprintf(stderr, "Error: mlx_new_image failed for obstacle\n");
		return (1);
	}
	
	// Init dynamic square
	data.dynamic_square = create_square(
	(t_point){10, 10}, // TL
	(t_point){20, 10}, // TR
	(t_point){20, 20}, // BR
	(t_point){10, 20}  // BL
);
		
	/*void move_square(t_square *square, int dx, int dy)
	{
		square->top_left.x += dx;
		square->top_left.y += dy;
		square->top_right.x += dx;
		square->top_right.y += dy;
		square->bottom_right.x += dx;
		square->bottom_right.y += dy;
		square->bottom_left.x += dx;
		square->bottom_left.y += dy;
	}*/
	
	
	//printf("Obstacle sprite initialized successfully.\n");

	//printf("Setting up hooks...\n");
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data); // Key press
	mlx_hook(data.win, 17, 0, close_window, &data);  // Window close button
	mlx_loop_hook(data.mlx, game_loop, &data);	   // Game loop
	//printf("Hooks set up successfully.\n");

	//printf("Starting main loop...\n");
	mlx_loop(data.mlx);
	return (0);
}
