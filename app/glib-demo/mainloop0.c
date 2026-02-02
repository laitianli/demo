// //mainloop0.c
#include<glib.h> 
#include <stdio.h>
GMainLoop* loop;
int test1(int argc, char* argv[])
{
    //g_thread_init是必需的，GMainLoop需要gthread库的支持。
    if(g_thread_supported() == 0)
        g_thread_init(NULL);
    //创建一个循环体，先不管参数的意思。
    g_print("g_main_loop_new/n");
    loop = g_main_loop_new(NULL, FALSE);
    //让这个循环体跑起来
    g_print("g_main_loop_run/n");
    g_main_loop_run(loop);
    //循环运行完成后，计数器减一
    //glib的很多结构类型和c++的智能指针相似，拥有一个计数器
    //当计数器为0时，自动释放资源。
    g_print("g_main_loop_unref/n");
    g_main_loop_unref(loop);
    return 0;
 }


// GMainLoop *loop; // 声明主事件循环对象

// 超时回调函数
gboolean timeout_callback(gpointer data) {
    g_print("time out, begin call func: g_main_loop_quit() ...\n");
    g_main_loop_quit(loop); // 退出事件循环
    return FALSE; // 返回FALSE表示此超时事件源不再执行
}

int test2(int argc, char *argv[]) {
    // 创建一个主事件循环，使用默认的上下文(Context)
    loop = g_main_loop_new(NULL, FALSE);
    
    // 添加一个超时事件源：3000毫秒后触发回调函数
    g_timeout_add(3000, timeout_callback, NULL);
    
    g_print("begin run g_main_loop_run()...\n");
    // 启动事件循环，它将持续运行直到被g_main_loop_quit()退出
    g_main_loop_run(loop);
    g_print("g_main_loop_run() run over.\n");
    
    // 释放主事件循环占用的资源
    g_main_loop_unref(loop);
    
    return 0;
}

int main(int argc, char *argv[]) {
    test2(argc, argv);
}