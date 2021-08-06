/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 03:36:24 by hyunwkim          #+#    #+#             */
/*   Updated: 2021/08/06 03:36:31 by hyunwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	draw(t_f *f)
{
	int		iter;
	int		color;
	int		x;
	int		y;

	f->factor = init_complex(
			(f->max.re - f->min.re) / WIDTH,
			(f->max.im - f->min.im) / HEIGHT);
	y = 0;
	while (y <= HEIGHT)
	{
		f->c.im = f->min.im + y * f->factor.im;
		x = 0;
		while (x <= WIDTH)
		{
			f->c.re = f->min.re + x * f->factor.re;
			put_pixel(x, y, color_set(f->func(f), f), f);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(f->ptr, f->win, f->img.ptr, 0, 0);
}
int	mandelbrot(t_f *f)
{
	int			iter;
	double		x;
	double		x2;
	double		y;
	double		y2;

	iter = 0;
	x = 0;
	y = 0;
	x2 = 0;
	y2 = 0;
	while (x2 + y2 <= 4 && (iter < f->max_iter))
	{
		y = 2 * x * y + f->c.im;
		x = x2 - y2 + f->c.re;
		x2 = x * x;
		y2 = y * y;
		iter++;
	}
	return (iter);
}

int	tricorn(t_f *f)
{
	int			iter;
	t_complex	z;
	t_complex	z2;

	iter = 0;
	z = init_complex(f->c.re, f->c.im);
	z2 = init_complex(z.re * z.re, z.im * z.im);
	while (z2.re + z2.im <= 4 && iter < f->max_iter)
	{
		z.im = -2.0 * z.re * z.im + f->c.im;
		z.re = z2.re - z2.im + f->c.re;
		z2.re = z.re * z.re;
		z2.im = z.im * z.im;
		iter++;
	}
	return (iter);
}

int	julia(t_f *f)
{
	int			iter;
	t_complex	z;
	t_complex	z2;
	t_complex	k;

	iter = 0;
	z = init_complex(f->c.re, f->c.im);
	z2 = init_complex(z.re * z.re, z.im * z.im);
	k = init_complex(-0.701756, -0.3842);
	while (z2.re + z2.im <= 4 && iter < f->max_iter)
	{
		z.im = 2.0 * z.re * z.im + k.im;
		z.re = z2.re - z2.im + k.re;
		z2.re = z.re * z.re;
		z2.im = z.im * z.im;
		iter++;
	}
	return (iter);
}
