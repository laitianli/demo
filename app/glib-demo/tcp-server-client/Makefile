srv2_app=server2
srv_app=server
cli_app=client
LIB_GLIB=$(shell pkg-config --cflags --libs glib-2.0 gthread-2.0 gio-2.0)
all: $(srv_app) $(cli_app) $(srv2_app)

$(srv_app): server.c
	gcc -o $@ $< -g $(LIB_GLIB)

$(srv2_app): server2.c
	gcc -o $@ $< -g $(LIB_GLIB)

$(cli_app): client.c
	gcc -o $@ $< -g $(LIB_GLIB)

clean:
	@rm -rf $(srv_app) $(srv2_app) $(cli_app)
