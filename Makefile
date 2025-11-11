LIBS = -lGLESv2 -lEGL -lm -lX11  -lcairo -lwayland-client -lwayland-server -lwayland-cursor -lwayland-egl
CFLAGS =-g -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libdrm -I/usr/include/libpng12  -I/usr/include

all: triangle_animation triangle triangle_simple simple_texture rotate_texture triangle_color mvp_triangle cube cube_headless render \

triangle_animation : ./2.triangle_animation/main.o  ./common/common.o ./common/window.o ./common/xdg-shell-protocol.o
	gcc ./2.triangle_animation/main.c ./common/common.c ./common/window.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}
triangle : ./1.triangle/main.o  ./common/common.o ./common/window.o ./common/xdg-shell-protocol.o
	gcc ./1.triangle/main.c ./common/common.c ./common/window.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

simple_texture : ./4.simple_texture/main.o  ./common/common.o ./common/window.o ./common/xdg-shell-protocol.o
	gcc ./4.simple_texture/main.c ./common/common.c ./common/window.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

rotate_texture : ./5.rotate_texture/main.o  ./common/common.o ./common/window.o ./common/xdg-shell-protocol.o
	gcc ./5.rotate_texture/main.c ./common/common.c ./common/window.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

triangle_color : ./6.triangle_color/main.o  ./common/common.o ./common/window.o ./common/transform.o ./common/xdg-shell-protocol.o
	gcc ./6.triangle_color/main.c ./common/common.c ./common/window.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

mvp_triangle : ./7.mvp_triangle/main.o  ./common/common.o ./common/window.o ./common/transform.o ./common/xdg-shell-protocol.o
	gcc ./7.mvp_triangle/main.c ./common/common.c ./common/window.c ./common/transform.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

cube : ./8.cube/main.o  ./common/common.o ./common/window.o ./common/transform.o ./common/xdg-shell-protocol.o
	gcc ./8.cube/main.c ./common/common.c ./common/window.c ./common/transform.c ./common/xdg-shell-protocol.c ${CFLAGS} -o $@ ${LIBS}

cube_headless : ./8.cube/main_headless.o ./common/transform.o
	gcc ./8.cube/main_headless.c ./common/transform.c ${CFLAGS} -o $@ -lGLESv2 -lEGL -lm

render_objs : ./render/render.o ./render/renderer.o ./render/shaders.o ./render/geometry.o

# Render module object files
./render/render.o: render/render.c render/render.h
	gcc ${CFLAGS} -c -o $@ $<

./render/renderer.o: render/renderer.c render/renderer.h render/shaders.h render/geometry.h
	gcc ${CFLAGS} -c -o $@ $<

./render/shaders.o: render/shaders.c render/shaders.h
	gcc ${CFLAGS} -c -o $@ $<

./render/geometry.o: render/geometry.c render/geometry.h
	gcc ${CFLAGS} -c -o $@ $<

renderb : ./render/render.o ./render/renderer.o ./render/shaders.o ./render/geometry.o ./common/common.o ./common/window.o ./common/transform.o ./common/xdg-shell-protocol.o
	gcc ./render/render.c ./render/renderer.c ./render/shaders.c ./render/geometry.c ./common/common.c ./common/window.c ./common/transform.c ./common/xdg-shell-protocol.c ${CFLAGS} -o renderb ${LIBS}

clean:
	rm -f 1.triangle/*.o *~ 
	rm -f triangle
	rm -f 2.triangle_animation/*.o *~ 
	rm -f triangle_animation
	rm -f 3.triangle_simple/*.o *~ 
	rm -f triangle_simple
	rm -f 4.simple_texture/*.o *~ 
	rm -f simple_texture
	rm -f 5.rotate_texture/*.o *~ 
	rm -f rotate_texture
	rm -f 6.triangle_color/*.o *~ 
	rm -f triangle_color
	rm -f 7.mvp_triangle/*.o *~ 
	rm -f mvp_triangle
	rm -f 8.cube/*.o *~ 
	rm -f cube
	rm -f cube_headless
	rm -f cube_x11
	rm -f render/render
	rm -f render/*.o
	rm renderb
