import spwd
import crypt
import socket
import threading
import re
from ftpfun import*

FTP_SERVER_PORT = 5566
FTP_SERVER_IP = '0.0.0.0'
FTP_VERSION = '1.0'

CMD_HANDLE = {
    'USER': do_username,
    'PASS': do_passwd,
    'SYST': do_syst,
    'OPTS': do_opts,
    'PWD':  do_pwd,
    'XPWD':  do_pwd,
    'CWD':  do_cwd,
    'CDUP': do_cdup,
    'LIST': do_list,
    'NLST': do_list,
    'MKD' : do_mkd,
    'RMD' : do_rmd,
    'DELE' : do_dele,
    'PASV': do_pasv, ##--pasv
    'REST': do_rest, ##--pasv
    'SIZE': do_size, ##--pasv
    'TVFS': do_tvfs, ##--pasv
    'UTF8': do_utf8, ##--pasv
    'TYPE': do_type,
    'STOR': do_stor,
    'MDTM': do_mdtm,
    'RETR': do_retr,
    'FEAT': do_feat,
    'EPRT': do_eprt, ##--eprt
    'EPSV': do_epsv, ##--eprt
    'PORT': do_port,
    'QUIT': do_quit,
}
def do_unknow(sock, cmd, arg = None):
    if arg == None:
        print('do_unknow cmd: ' + cmd)
    else:
        print('do_unknow cmd: ' + cmd + ' arg: ' + arg)
    str_info = '200 {} unknow handle ok.\r\n'.format(cmd)
    sock.send(str_info.encode())

def parse_cmd(data):
    index = data.find(' ')
    return data[0 : index], data[index + 1: ].rstrip()

def ftp_client_thread(clientsocket, address):
    print("new ftp client...ip addr: " + repr(address))
    clientsocket.send(b'220 (python FTP 1.0)\r\n')
    while True:
        data = clientsocket.recv(1024)
        if not data:
            break
        print(repr(data))  ##'USER haizhi\r\n'
        str_cmd_arg = data.decode()
        if " " in str_cmd_arg.rstrip():
            cmd, arg = parse_cmd(str_cmd_arg)
            #print('cmd: ' + cmd + ', arg: ' + arg)
            if CMD_HANDLE.get(cmd):
                CMD_HANDLE.get(cmd)(clientsocket, cmd, arg)
            else:
                do_unknow(clientsocket, cmd, arg)
        else:
            cmd = str_cmd_arg.rstrip()
            if CMD_HANDLE.get(cmd):
                CMD_HANDLE.get(cmd)(clientsocket, cmd)
            else:
                do_unknow(clientsocket, cmd)

    print("ftp clent exit...")
    clientsocket.close()

def ftp_main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((FTP_SERVER_IP, FTP_SERVER_PORT))
    server.listen(5)
    while True:
        clientsocket, address = server.accept()
        client_th = threading.Thread(target = ftp_client_thread, name = "ftp client thread", args=(clientsocket, address))
        client_th.start()
    server.close()

if __name__ == "__main__":
    ftp_main()
