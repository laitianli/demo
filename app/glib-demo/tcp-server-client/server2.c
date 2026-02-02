#include <glib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    GSource source;        // 必须作为第一个成员
    GPollFD server_fd;     // 服务器socket的PollFD
    GPollFD *client_fds;   // 客户端socket的PollFD数组
    int max_clients;       // 最大客户端数量
    int client_count;      // 当前客户端数量
} TcpServerSource;

// 准备函数：检查是否有事件立即就绪
gboolean server_prepare(GSource *source, gint *timeout) {
    // printf("server_prepare called\n");
    TcpServerSource *server_source = (TcpServerSource *)source;
    *timeout = -1; // 一直阻塞，直到有事件发生
    return FALSE;  // 返回FALSE表示需要poll来检查具体事件
}

// 检查函数：检查poll的结果，判断哪些事件已就绪
gboolean server_check(GSource *source) {
    TcpServerSource *server_source = (TcpServerSource *)source;
    // printf("server_check called\n");
    // 检查服务器socket是否有新连接
    if (server_source->server_fd.revents & G_IO_IN) {
        return TRUE;
    }
    
    // 检查所有客户端socket是否有数据可读
    for (int i = 0; i < server_source->client_count; i++) {
        if (server_source->client_fds[i].fd != -1 && 
            (server_source->client_fds[i].revents & G_IO_IN)) {
            return TRUE;
        }
    }
    
    return FALSE;
}

// 分发函数：处理就绪的事件
gboolean server_dispatch(GSource *source, GSourceFunc callback, gpointer user_data) {
    TcpServerSource *server_source = (TcpServerSource *)source;
    
    // 处理新连接
    if (server_source->server_fd.revents & G_IO_IN) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_source->server_fd.fd, 
                             (struct sockaddr*)&client_addr, &addr_len);
        
        if (client_fd != -1) {
            g_print("new client: %s:%d, socket: %d\n", 
                   inet_ntoa(client_addr.sin_addr), 
                   ntohs(client_addr.sin_port), client_fd);
            
            // 将新客户端socket添加到poll集合中
            if (server_source->client_count < server_source->max_clients) {
                server_source->client_fds[server_source->client_count].fd = client_fd;
                server_source->client_fds[server_source->client_count].events = G_IO_IN;
                server_source->client_count++;
                
                // 关键步骤：将新的客户端fd添加到事件源的poll集合中
                g_source_add_poll(source, &server_source->client_fds[server_source->client_count - 1]);
            } else {
                g_print("over Max client, refuse!\n");
                close(client_fd);
            }
        }
    }
    
    // 处理客户端数据
    for (int i = 0; i < server_source->client_count; i++) {
        if (server_source->client_fds[i].fd != -1 && 
            (server_source->client_fds[i].revents & G_IO_IN)) {
            
            char buffer[1024];
            ssize_t len = recv(server_source->client_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
            
            if (len > 0) {
                buffer[len] = '\0';
                g_print("recv from client %d : %s", server_source->client_fds[i].fd, buffer);
                
                // 回显数据
                send(server_source->client_fds[i].fd, buffer, len, 0);
            } else if (len == 0) {
                // 客户端断开连接
                g_print("client %d disconnected\n", server_source->client_fds[i].fd);
                close(server_source->client_fds[i].fd);
                
                // 从poll集合中移除
                g_source_remove_poll(source, &server_source->client_fds[i]);
                server_source->client_fds[i].fd = -1;
            }
        }
    }
    
    return TRUE;
}

// 资源清理函数
void server_finalize(GSource *source) {
    TcpServerSource *server_source = (TcpServerSource *)source;
    printf("server_finalize called\n");
    // 关闭所有客户端socket
    for (int i = 0; i < server_source->client_count; i++) {
        if (server_source->client_fds[i].fd != -1) {
            close(server_source->client_fds[i].fd);
        }
    }
    
    // 关闭服务器socket
    if (server_source->server_fd.fd != -1) {
        close(server_source->server_fd.fd);
    }
    
    g_free(server_source->client_fds);
}

// 创建TCP服务器
int create_tcp_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        g_error("create socket failed.");
        return -1;
    }
    
    // 设置socket选项
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 绑定地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        g_error("bind address failed.");
        close(server_fd);
        return -1;
    }
    
    // 开始监听
    if (listen(server_fd, 10) == -1) {
        g_error("listen() failed.");
        close(server_fd);
        return -1;
    }
    
    return server_fd;
}

int main(int argc, char *argv[]) {
    GMainLoop *loop;
    GMainContext *context;
    
    // 创建主事件循环
    loop = g_main_loop_new(NULL, FALSE);
    context = g_main_loop_get_context(loop);
    
    // 定义事件源函数表
    GSourceFuncs source_funcs = {
        .prepare = server_prepare,
        .check = server_check,
        .dispatch = server_dispatch,
        .finalize = server_finalize
    };
    
    // 创建TCP服务器
    int server_fd = create_tcp_server(8080);
    if (server_fd == -1) {
        return 1;
    }
    
    g_print("TCP Server listening on port 8080...\n");
    
    // 创建自定义事件源
    TcpServerSource *server_source = (TcpServerSource *)g_source_new(&source_funcs, 
                                                                   sizeof(TcpServerSource));
    
    // 初始化服务器socket的PollFD
    server_source->server_fd.fd = server_fd;
    server_source->server_fd.events = G_IO_IN;
    
    // 初始化客户端socket数组
    server_source->max_clients = 100;
    server_source->client_count = 0;
    server_source->client_fds = g_new0(GPollFD, server_source->max_clients);
    for (int i = 0; i < server_source->max_clients; i++) {
        server_source->client_fds[i].fd = -1;
    }
    
    // 关键步骤：将服务器socket添加到事件源的poll集合中
    g_source_add_poll((GSource *)server_source, &server_source->server_fd);
    
    // 设置回调函数（这里使用NULL，因为我们在dispatch函数中直接处理了逻辑）
    g_source_set_callback((GSource *)server_source, NULL, NULL, NULL);
    
    // 将事件源附加到主上下文
    g_source_attach((GSource *)server_source, context);
    
    // 减少引用计数（事件源已被上下文引用）
    g_source_unref((GSource *)server_source);
    
    // 启动主事件循环
    g_main_loop_run(loop);
    
    // 清理资源
    g_main_loop_unref(loop);
    
    return 0;
}