#include "minip.h"
#include <stdio.h>
#include <stdlib.h>

void draw_line_to_buffer(t_data *data, t_point start, t_point end, int color)
{
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = (start.x < end.x) ? 1 : -1;
    int sy = (start.y < end.y) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        if (start.x >= 0 && start.x < WINDOW_WIDTH && start.y >= 0
            && start.y < WINDOW_HEIGHT)
        {
            int pixel = start.y * data->size_line + start.x * (data->bpp / 8);
            *(unsigned int *)(data->back_data + pixel) = color;
        }
        if (start.x == end.x && start.y == end.y)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
            { err -= dy; start.x += sx; }
        if (e2 < dx)
            { err += dx; start.y += sy; }
    }
}

void draw_square_to_buffer(t_data *data, t_square square, int color)
{
    draw_line_to_buffer(data, square.top_left, square.top_right, color);
    draw_line_to_buffer(data, square.top_right, square.bottom_right, color);
    draw_line_to_buffer(data, square.bottom_right, square.bottom_left, color);
    draw_line_to_buffer(data, square.bottom_left, square.top_left, color);
}

void clear_buffer(t_data *data, void *buffer, int color)
{
    char *buffer_data = mlx_get_data_addr(buffer, &data->bpp,
        &data->size_line, &data->endian);
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            int pixel = y * data->size_line + x * (data->bpp / 8);
            *(unsigned int *)(buffer_data + pixel) = color;
        }
    }
}

void draw_dolphin_to_buffer(t_data *data)
{
    if (!data->dolphin_img) return;

    int scaled_width = data->dolphin_width * SCALE_FACTOR;
    int scaled_height = data->dolphin_height * SCALE_FACTOR;

    int x = (WINDOW_WIDTH - scaled_width) / 2;
    int y = (WINDOW_HEIGHT - scaled_height) / 2;

    for (int i = 0; i < data->dolphin_height; i++)
    {
        for (int j = 0; j < data->dolphin_width; j++)
        {
            int src_pixel = i * data->dolphin_size_line + j * (data->bpp / 8);
            unsigned int color = *(unsigned int *)(data->dolphin_data + src_pixel);
            
            //if (color != 0x000000)
            //if (color != 0xFF000000)
            if (color != 0x0E1A39)
            {
                int dst_x = x + (int)(j * SCALE_FACTOR);
                int dst_y = y + (int)(i * SCALE_FACTOR);
                if (dst_x >= 0 && dst_x < WINDOW_WIDTH && dst_y >= 0 && dst_y < WINDOW_HEIGHT)
                {
                    int dst_pixel = dst_y * data->size_line + dst_x * (data->bpp / 8);
                    *(unsigned int *)(data->back_data + dst_pixel) = color;
                }
            }
        }
    }
}

void render_frame(t_data *data)
{
    clear_buffer(data, data->back_buffer, 0x000000);
    draw_dolphin_to_buffer(data);
    draw_square_to_buffer(data, data->dynamic_square, WHITE_PIXEL);
    draw_square_to_buffer(data, data->second_dynamic_square, RED_PIXEL);
    mlx_put_image_to_window(data->mlx, data->win, data->back_buffer, 0, 0);
}

int close_window(t_data *data)
{
    if (data->front_buffer) mlx_destroy_image(data->mlx, data->front_buffer);
    if (data->back_buffer) mlx_destroy_image(data->mlx, data->back_buffer);
    if (data->dolphin_img) mlx_destroy_image(data->mlx, data->dolphin_img);
    mlx_destroy_window(data->mlx, data->win);
    exit(0);
    return (0);
}

int game_loop(t_data *data)
{
    render_frame(data);
    return (0);
}

int main(void)
{
    t_data data = {0};

    data.mlx = mlx_init();
    if (!data.mlx) return (1);
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "minip");
    data.front_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    data.back_buffer = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    data.back_data = mlx_get_data_addr(data.back_buffer, &data.bpp,
        &data.size_line, &data.endian);

    // Initialize squares
    t_point top_left = {10, 10};
    t_point top_right = {15, 10};
    t_point bottom_right = {15, 15};
    t_point bottom_left = {10, 15};
    data.dynamic_square = create_square(top_left, top_right,
        bottom_right, bottom_left);

    t_point second_top_left = {100, 100};
    t_point second_top_right = {105, 100};
    t_point second_bottom_right = {105, 105};
    t_point second_bottom_left = {100, 105};
    data.second_dynamic_square = 
        create_square(second_top_left, second_top_right, 
        second_bottom_right, second_bottom_left);

    // Load dolphin image
    data.dolphin_img = mlx_xpm_file_to_image(data.mlx, "dolphin.xpm",
        &data.dolphin_width, &data.dolphin_height);
    if (data.dolphin_img)
    {
        data.dolphin_data = mlx_get_data_addr(data.dolphin_img, &data.bpp,
            &data.dolphin_size_line, &data.endian);
    }
    else
    {
        fprintf(stderr, "Warning: Failed to load dolphin.xpm\n");
    }
printf("Scaled image size: %dx%d\n", data.dolphin_width, data.dolphin_height);
    mlx_hook(data.win, 2, 1L<<0, key_hook, &data);
    mlx_hook(data.win, 17, 0, close_window, &data);
    mlx_loop_hook(data.mlx, game_loop, &data);
    mlx_loop(data.mlx);
    return (0);
}
