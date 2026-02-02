#include <glib.h>
#include <gio/gio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

GMainLoop *loop;
int server_fd;

// 处理客户端数据的回调函数
gboolean handle_client_data(GSocket *socket, GIOCondition condition, gpointer user_data) {
    gchar buffer[1024];
    gssize len;
    
    if (condition & G_IO_IN) {
        len = g_socket_receive(socket, buffer, sizeof(buffer) - 1, NULL, NULL);
        if (len > 0) {
            buffer[len] = '\0';
            g_print("Received from client: %s", buffer);
            
            // 回显数据给客户端
            g_socket_send(socket, buffer, len, NULL, NULL);
        } else if (len == 0) {
            g_print("Client disconnected\n");
            g_object_unref(socket);
            return FALSE; // 移除事件源
        }
    }
    return TRUE; // 保持事件源活跃
}

// 接受新连接的回调函数
gboolean accept_connection(GSocket *socket, GIOCondition condition, gpointer user_data) {
    GSocket *client_socket;
    GError *error = NULL;
    
    if (condition & G_IO_IN) {
        client_socket = g_socket_accept(socket, NULL, &error);
        if (client_socket) {
            g_print("New client connected\n");
            
            // 创建客户端socket的IO监视源
            GSource *client_source = g_socket_create_source(client_socket, G_IO_IN, NULL);
            g_source_set_callback(client_source, (GSourceFunc)handle_client_data, 
                               client_socket, NULL);
            g_source_attach(client_source, NULL);
            g_source_unref(client_source);
        }
    }
    return TRUE;
}

int main(int argc, char *argv[]) {
    GError *error = NULL;
    
    // 创建主事件循环
    loop = g_main_loop_new(NULL, FALSE);
    
    // 创建服务器socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        g_error("Failed to create socket");
        return 1;
    }
    
    // 设置socket选项
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 绑定地址和端口
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        g_error("Bind failed");
        close(server_fd);
        return 1;
    }
    
    // 开始监听
    if (listen(server_fd, 10) < 0) {
        g_error("Listen failed");
        close(server_fd);
        return 1;
    }
    
    g_print("TCP Server listening on port 8080...\n");
    
    // 创建服务器socket的GSocket对象
    GSocket *server_socket = g_socket_new_from_fd(server_fd, &error);
    if (!server_socket) {
        g_error("Failed to create GSocket: %s", error->message);
        close(server_fd);
        return 1;
    }
    
    // 创建接受连接的事件源
    GSource *accept_source = g_socket_create_source(server_socket, G_IO_IN, NULL);
    g_source_set_callback(accept_source, (GSourceFunc)accept_connection, 
                        server_socket, NULL);
    g_source_attach(accept_source, NULL);
    g_source_unref(accept_source);
    
    // 启动主事件循环
    g_main_loop_run(loop);
    
    // 清理资源
    g_main_loop_unref(loop);
    g_object_unref(server_socket);
    close(server_fd);
    
    return 0;
}