
import socket
import threading
import os
import stat
import re
import sys

down_file='/home/haizhi/demo/python/tcp/file/messon_ninja.tar.bz2'
SERVER_PORT=5678
SERVER_IP='192.168.42.1'
def tcp_client_main():
   f = open(down_file, "rb")
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   sock.connect((SERVER_IP, SERVER_PORT))
   print('begin send file...size: {}'.format(os.path.getsize(down_file)))
   os.sendfile(sock.fileno(), f.fileno(), 0, os.path.getsize(down_file))
   print('begin send end')
   f.close()
   sock.close()
   print("send file success")

if __name__ == "__main__":
   tcp_client_main()
