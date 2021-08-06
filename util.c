/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 03:38:18 by hyunwkim          #+#    #+#             */
/*   Updated: 2021/08/06 03:38:27 by hyunwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	interpolate(double zoom_rate, t_complex pos, t_f *f)
{
	f->max.re = pos.re + (f->max.re - pos.re) * zoom_rate;
	f->min.re = pos.re + (f->min.re - pos.re) * zoom_rate;
	f->max.im = pos.im + (f->max.im - pos.im) * zoom_rate;
	f->min.im = pos.im + (f->min.im - pos.im) * zoom_rate;
}

void	put_pixel(int x, int y, int color, t_f *f)
{
	char	*dst;

	dst = f->img.data + (y * f->img.size_l) + (x * f->img.bpp / 8);
	*(unsigned int *)dst = color;
}

t_complex	init_complex(double re, double im)
{
	t_complex	complex;

	complex.re = re;
	complex.im = im;
	return (complex);
}

int	color_set(int iter, t_f *f)
{
	double	t;
	int		red;
	int		green;
	int		blue;

	if (iter == f->max_iter)
		return (0x00000000);
	if (f->color_style == 0)
	{
		t = (double)iter / (double)f->max_iter;
		red = (int)(9 * (1 - t) * pow (t, 3) * 255);
		green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
		blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
	}
	else if (f->color_style == 1)
	{
		red = sin(0.3 * (double)iter);
		green = sin(0.3 * (double)iter + 3) * 127 + 128;
		blue = sin(0.3 * (double)iter + 3) * 127 + 128;
	}
	else if (f->color_style == 2)
	{
		red = (((double)iter + 1) * 25) / 2;
		green = 0;
		blue = (((double)iter + 1) * 25) / 2;
	}
	return ((red << 16) + (green << 8) + blue);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2 && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
