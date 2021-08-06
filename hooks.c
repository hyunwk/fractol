/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 03:34:32 by hyunwkim          #+#    #+#             */
/*   Updated: 2021/08/06 12:59:15 by hyunwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	hooks_loop(t_f *f)
{
	mlx_hook(f->win, KEY_PRESS, 0, key_press, f);
	mlx_hook(f->win, SCROLL_UP, 0, zoom, f);
	mlx_hook(f->win, SCROLL_DOWN, 0, zoom, f);
	mlx_hook(f->win, KEY_EXIT, 0, close_window, 0);
	mlx_loop(f->ptr);
}

int	key_press(int key, t_f *f)
{
	if (key == KEY_ESC)
		exit(0);
	else if (key == C)
	{
		f->color_style = (f->color_style + 1) % 3;
		draw(f);
	}
	else if (key == LEFT || key == RIGHT || key == DOWN || key == UP)
		move(key, f);
	return (0);
}

int	close_window(int key)
{
	exit(0);
}

void	move(int key, t_f *f)
{
	t_complex	factor;

	factor = init_complex(f->max.re - f->min.re, f->max.im - f->min.im);
	if (key == LEFT)
	{
		f->max.re -= factor.re * 0.05;
		f->min.re -= factor.re * 0.05;
	}
	else if (key == RIGHT)
	{
		f->max.re += factor.re * 0.05;
		f->min.re += factor.re * 0.05;
	}
	else if (key == DOWN)
	{
		f->max.im += factor.im * 0.05;
		f->min.im += factor.im * 0.05;
	}
	else if (key == UP)
	{
		f->max.im -= factor.im * 0.05;
		f->min.im -= factor.im * 0.05;
	}
	draw(f);
}

int	zoom(int key, int x, int y, t_f *f)
{
	t_complex	pos;
	double		zoom_rate;

	if (key == SCROLL_UP || key == SCROLL_DOWN)
	{
		pos = init_complex(
				f->min.re + (double)x * f->factor.re,
				f->min.im + (double)y * f->factor.im);
		if (key == SCROLL_UP)
			zoom_rate = 1.2;
		else
		{
			zoom_rate = 0.8;
			f->max_iter += 20;
		}
		interpolate(zoom_rate, pos, f);
		mlx_clear_window(f->ptr, f->win);
		printf("max_iter :%d\n", f->max_iter);
		draw(f);
	}
	return (0);
}
