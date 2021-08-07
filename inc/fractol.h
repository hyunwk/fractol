/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunwkim <hyunwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:00:19 by hyunwkim          #+#    #+#             */
/*   Updated: 2021/08/07 16:28:51 by hyunwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include	"../mlx/mlx.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<math.h>

# define		KEY_EXIT		17
# define		KEY_ESC			53
# define		KEY_PRESS		2
# define		SCROLL_UP		4
# define		SCROLL_DOWN		5

# define		LEFT			123
# define		RIGHT			124
# define		DOWN			125
# define		UP				126

# define		WIDTH			800
# define		HEIGHT			600

# define		C				8

typedef struct s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef struct s_img
{
	void		*ptr;
	char		*data;
	int			size_l;
	int			bpp;
	int			endian;
}				t_img;

typedef struct s_f
{
	void		*ptr;
	void		*win;
	int			color_style;
	int			max_iter;
	int			(*func)(struct s_f*);
	t_img		img;
	t_complex	c;
	t_complex	min;
	t_complex	max;
	t_complex	factor;
}				t_f;

// init.c
void		init_fractal(t_f *f);
int			init_window(t_f *f);
int			check_argv(t_f *f, int argc, char **argv);

//draw.c
void		draw(t_f *f);
int			julia(t_f *f);
int			tricorn(t_f *f);
int			mandelbrot(t_f *f);

// hook.c
void		hooks_loop(t_f *f);
int			close_window(t_f *f);
int			key_press(int key, t_f *f);
void		move(int key, t_f *f);
int			zoom(int key, int x, int y, t_f *f);

//util.c
int			color_set(int iter, t_f *f);
void		interpolate(double zoom_rate, t_complex pos, t_f *f);
void		put_pixel(int x, int y, int color, t_f *f);
int			ft_strcmp(const char *s1, const char *s2);
t_complex	init_complex(double re, double im);
#endif
