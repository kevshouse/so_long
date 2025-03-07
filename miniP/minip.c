/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minip.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keanders <keanders@student.42london.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:06:35 by keanders          #+#    #+#             */
/*   Updated: 2025/03/06 19:09:39 by keanders         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minip.h"

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

	// Render sprites to the back buffer
	mlx_put_image_to_window(data->mlx, data->win, data->player.img, data->player.x, data->player.y);
	mlx_put_image_to_window(data->mlx, data->win, data->enemy.img, data->enemy.x, data->enemy.y);
	mlx_put_image_to_window(data->mlx, data->win, data->obstacle.img, data->obstacle.x, data->obstacle.y);

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

int		check_collision(t_sprite *a, t_sprite *b)
{
	return (a->x < b->x + b->width
		&& a->x + a->width > b->x
		&& a->y < b->y + b->height
		&& a->y + a->height > b->y);
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

void handle_input(t_sprite *sprite, int keycode)
{
    if (keycode == KEY_LEFT)
        sprite->vx = -5;
    else if (keycode == KEY_RIGHT)
        sprite->vx = 5;
    else if (keycode == KEY_SPACE && sprite->grounded)
    {
        sprite->vy = -15;
        sprite->grounded = 0;
    }
}

int key_hook(int keycode, t_data *data)
{
    handle_input(&data->player, keycode);
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

	printf("Initializing MLX...\n");
	data.mlx = mlx_init();
	if (!data.mlx)
    {
        fprintf(stderr, "Error: mlx_init failed\n");
        return (1);
    }
    printf("MLX initialized successfully.\n");

    printf("Creating window...\n");
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Random Blobs Example");
    if (!data.win)
    {
        fprintf(stderr, "Error: mlx_new_window failed\n");
        return (1);
    }
    printf("Window created successfully.\n");

    printf("Creating front buffer...\n");
    data.front_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!data.front_buffer)
    {
        fprintf(stderr, "Error: mlx_new_image failed for front_buffer\n");
        return (1);
    }
    printf("Front buffer created successfully.\n");

    printf("Creating back buffer...\n");
    data.back_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!data.back_buffer)
    {
        fprintf(stderr, "Error: mlx_new_image failed for back_buffer\n");
        return (1);
    }
    printf("Back buffer created successfully.\n");

    printf("Initializing player sprite...\n");
    data.player.img = mlx_new_image(data.mlx, 50, 50);
    if (!data.player.img)
    {
        fprintf(stderr, "Error: mlx_new_image failed for player\n");
        return (1);
    }
    printf("Player sprite initialized successfully.\n");

    printf("Initializing enemy sprite...\n");
    data.enemy.img = mlx_new_image(data.mlx, 50, 50);
    if (!data.enemy.img)
    {
        fprintf(stderr, "Error: mlx_new_image failed for enemy\n");
        return (1);
    }
    printf("Enemy sprite initialized successfully.\n");

    printf("Initializing obstacle sprite...\n");
    data.obstacle.img = mlx_new_image(data.mlx, 100, 20);
    if (!data.obstacle.img)
    {
        fprintf(stderr, "Error: mlx_new_image failed for obstacle\n");
        return (1);
    }
    printf("Obstacle sprite initialized successfully.\n");

    printf("Setting up hooks...\n");
    mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
    mlx_loop_hook(data.mlx, game_loop, &data);
    printf("Hooks set up successfully.\n");

    printf("Starting main loop...\n");
    mlx_loop(data.mlx);
    return (0);
}