/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 03:35:27 by hyunwkim          #+#    #+#             */
/*   Updated: 2021/08/06 12:58:34 by hyunwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	init_fractal(t_f *f)
{
	f->max_iter = 100;
	f->color_style = 0;
	f->min = init_complex(-2.0, -1.0);
	f->max = init_complex(1.0, 1.0);
}

int	check_argv(t_f *f, int argc, char **argv)
{
	if (argc == 2)
	{
		if (!ft_strcmp(argv[1], "mandelbrot"))
		{
			f->func = &mandelbrot;
			return (1);
		}
		else if (!ft_strcmp(argv[1], "julia"))
		{
			f->func = &julia;
			return (1);
		}
		else if (!ft_strcmp(argv[1], "tricorn"))
		{
			f->func = &tricorn;
			return (1);
		}
	}
	printf("Choose fractal \n* mandelbrot \n* julia \n* tricorn\n");
	return (0);
}

int	init_window(t_f *f)
{
	f->ptr = mlx_init();
	if (!f->ptr)
		return (0);
	f->win = mlx_new_window(f->ptr, WIDTH, HEIGHT, "fractol");
	if (!f->win)
		return (0);
	f->img.ptr = mlx_new_image(f->ptr, WIDTH, HEIGHT);
	if (!f->img.ptr)
		return (0);
	f->img.data = mlx_get_data_addr(f->img.ptr,
			&f->img.bpp, &f->img.size_l, &f->img.endian);
	if (!f->img.data)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_f	f;

	if (!init_window(&f) || !check_argv(&f, argc, argv))
		return (0);
	init_fractal(&f);
	draw(&f);
	hooks_loop(&f);
}
