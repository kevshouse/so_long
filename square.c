/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
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
	square->top_left.x += dx;
	square->top_left.y += dy;
	square->top_right.x += dx;
	square->top_right.y += dy;
	square->bottom_right.x += dx;
	square->bottom_right.y += dy;
	square->bottom_left.x += dx;
	square->bottom_left.y += dy;
}

void handle_input(t_data *data, int keycode) // Ensure this is in square.c
{
	if (keycode == KEY_LEFT)
		move_square(&data->dynamic_square, -MOVEMENT_STEP, 0); // Use MOVEMENT_STEP
	else if (keycode == KEY_RIGHT)
		move_square(&data->dynamic_square, MOVEMENT_STEP, 0);
	else if (keycode == KEY_UP)
		move_square(&data->dynamic_square, 0, -MOVEMENT_STEP);
	else if (keycode == KEY_DOWN)
		move_square(&data->dynamic_square, 0, MOVEMENT_STEP);
	else if (keycode == KEY_SPACE && data->player.grounded)
	{
		data->player.vy = -15;
		data->player.grounded = 0; // Corrected from rounded to grounded
	}
}

