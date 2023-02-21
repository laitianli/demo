import socket
import threading
import os
import stat
import re
import sys
SERVER_PORT=5678
#tmp_file='/home/haizhi/demo/python/tcp/file/aa.txt'
tmp_file='E:\\ftp\\aa.txt'
def tcp_server_main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(('0.0.0.0', SERVER_PORT))
    sock.listen(5)
    data_sock, address = sock.accept()
    count = 0
    f = open(tmp_file, 'wb+')
    while True:
        data = data_sock.recv(1024)
        if not data:
            break
        f.write(data)
        count += len(data)

    f.close()
    print('save data size: {} success'.format(count))

if __name__ == "__main__":
   tcp_server_main()