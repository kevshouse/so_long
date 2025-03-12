/* ************************************************************************** */
/*									      */
/*				  	  :::	  ::::::::   */
/*   square.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: keanders <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/03/07 15:28:15 by keanders		  #+#	#+#			 */
/*   Updated: 2025/03/07 15:28:19 by keanders		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minip.h"

t_square create_square(
		t_point top_left,
		t_point top_right,
		t_point bottom_right,
		t_point bottom_left)
{
	t_square	square;

	square.top_left = top_left;
	square.top_right = top_right;
	square.bottom_right = bottom_right;
	square.bottom_left = bottom_left;
	return (square);
}

void move_square(t_square *square, int dx, int dy)
{
    // Calculate new positions for the square's corners
    int new_top_left_x = square->top_left.x + dx;
    int new_top_left_y = square->top_left.y + dy;

    // Calculate the width and height of the square
    int width = square->top_right.x - square->top_left.x;
    int height = square->bottom_left.y - square->top_left.y;

    // Check if the new position is within the window bounds
    if (new_top_left_x >= 0 && new_top_left_x + width <= WINDOW_WIDTH)
    {
        square->top_left.x = new_top_left_x;
        square->top_right.x = new_top_left_x + width;
        square->bottom_left.x = new_top_left_x;
        square->bottom_right.x = new_top_left_x + width;
    }

    if (new_top_left_y >= 0 && new_top_left_y + height <= WINDOW_HEIGHT)
    {
        square->top_left.y = new_top_left_y;
        square->top_right.y = new_top_left_y;
        square->bottom_left.y = new_top_left_y + height;
        square->bottom_right.y = new_top_left_y + height;
    }
}

int key_hook(int keycode, t_data *data)
{
	handle_input(data, keycode);
	return (0);
}

void handle_input(t_data *data, int keycode)
{
    if (keycode == KEY_LEFT)
        move_square(&data->dynamic_square, -MOVEMENT_STEP, 0);
    else if (keycode == KEY_RIGHT)
        move_square(&data->dynamic_square, MOVEMENT_STEP, 0);
    else if (keycode == KEY_UP)
        move_square(&data->dynamic_square, 0, -MOVEMENT_STEP);
    else if (keycode == KEY_DOWN)
        move_square(&data->dynamic_square, 0, MOVEMENT_STEP);
    else if (keycode == KEY_W)
        move_square(&data->second_dynamic_square, 0, -MOVEMENT_STEP);
    else if (keycode == KEY_A)
        move_square(&data->second_dynamic_square, -MOVEMENT_STEP, 0);
    else if (keycode == KEY_S)
        move_square(&data->second_dynamic_square, 0, MOVEMENT_STEP);
    else if (keycode == KEY_D)
        move_square(&data->second_dynamic_square, MOVEMENT_STEP, 0);
    else if (keycode == KEY_ESC)
        close_window(data);
}


