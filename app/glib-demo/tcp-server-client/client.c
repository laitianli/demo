#include <glib.h>
#include <gio/gio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

GMainLoop *loop;
GSocket *client_socket = NULL;

// 处理服务器数据的回调函数
gboolean handle_server_data(GSocket *socket, GIOCondition condition, gpointer user_data) {
    gchar buffer[1024];
    gssize len;
    
    if (condition & G_IO_IN) {
        len = g_socket_receive(socket, buffer, sizeof(buffer) - 1, NULL, NULL);
        if (len > 0) {
            buffer[len] = '\0';
            g_print("Reply from server: %s", buffer);
        } else if (len <= 0) {
            g_print("Server closed connection\n");
            g_main_loop_quit(loop);
            return FALSE;
        }
    }
    return TRUE;
}

// 处理标准输入的回调函数
gboolean handle_stdin_input(GIOChannel *channel, GIOCondition condition, gpointer user_data) {
    gchar *line = NULL;
    gsize len = 0;
    GError *error = NULL;
    
    if (condition & G_IO_IN) {
        if (g_io_channel_read_line(channel, &line, &len, NULL, &error) == G_IO_STATUS_NORMAL) {
            if (client_socket) {
                // 发送数据到服务器
                g_socket_send(client_socket, line, len, NULL, NULL);
                
                // 检查退出条件
                if (g_strrstr(line, "quit")) {
                    g_main_loop_quit(loop);
                }
            }
            g_free(line);
        }
    }
    return TRUE;
}

int main(int argc, char *argv[]) {
    GError *error = NULL;
    
    // 创建主事件循环
    loop = g_main_loop_new(NULL, FALSE);
    
    // 创建客户端socket
    client_socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, 
                               G_SOCKET_PROTOCOL_TCP, &error);
    if (!client_socket) {
        g_error("Failed to create socket: %s", error->message);
        return 1;
    }
    
    // 创建服务器地址
    GInetAddress *inet_addr = g_inet_address_new_from_string("127.0.0.1");
    GSocketAddress *addr = g_inet_socket_address_new(inet_addr, 8080);
    
    // 连接服务器
    if (!g_socket_connect(client_socket, addr, NULL, &error)) {
        g_error("Connection failed: %s", error->message);
        g_object_unref(client_socket);
        g_object_unref(addr);
        g_object_unref(inet_addr);
        return 1;
    }
    
    g_print("Connected to server. Type messages (type 'quit' to exit):\n");
    
    // 创建服务器数据接收的事件源
    GSource *socket_source = g_socket_create_source(client_socket, G_IO_IN, NULL);
    g_source_set_callback(socket_source, (GSourceFunc)handle_server_data, NULL, NULL);
    g_source_attach(socket_source, NULL);
    g_source_unref(socket_source);
    
    // 创建标准输入的事件源
    GIOChannel *stdin_channel = g_io_channel_unix_new(fileno(stdin));
    GSource *stdin_source = g_io_create_watch(stdin_channel, G_IO_IN);
    g_source_set_callback(stdin_source, (GSourceFunc)handle_stdin_input, NULL, NULL);
    g_source_attach(stdin_source, NULL);
    g_source_unref(stdin_source);
    g_io_channel_unref(stdin_channel);
    
    // 启动主事件循环
    g_main_loop_run(loop);
    
    // 清理资源
    g_main_loop_unref(loop);
    if (client_socket) {
        g_object_unref(client_socket);
    }
    g_object_unref(addr);
    g_object_unref(inet_addr);
    
    return 0;
}