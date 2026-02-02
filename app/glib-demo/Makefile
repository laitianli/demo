
##pkg-config --cflags --libs glib-2.0
# gcc -o simple_loop mainloop0.c `pkg-config --cflags --libs glib-2.0`

app_name=mainloop0
LIB_GLIB=$(shell pkg-config --cflags --libs glib-2.0 gthread-2.0)

all:
	gcc  mainloop0.c -o $(app_name) -g $(LIB_GLIB)

clean:
	rm -rf $(app_name)



