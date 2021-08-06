all:
	make all -C mlx
	#arch -x86_64 gcc *.c -Imlx -Lmlx -lmlx -framework OpenGL -framework Appkit
	#gcc *.c -Imlx -Lmlx -lmlx -framework OpenGL -framework Appkit
	gcc *.c -g -Imlx -Lmlx -lmlx -framework OpenGL -framework Appkit
	./a.out mandelbrot

clean :
	rm mlx/*.o mlx/libmlx.a
	rm ./a.out
