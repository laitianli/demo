import spwd
import pwd
import crypt
import socket
import threading
import os
import crypt
import getpass
import stat
from hmac import compare_digest as compare_hash
from datetime import datetime
import time
import re
import sys

def do_username(sock, cmd, arg = None):
    global username
    if arg == None:
        sock.send(b'530 Permission denied.\r\n')
        sock.close()
        return
    print('do_username cmd: ' + cmd + ', arg: ' + arg)
    username = arg
    print('username: ' + username)
    sock.send(b'331 Please specify the passwd.\r\n')

def do_passwd(sock, cmd, arg = None):
    global username
    print('do_passwd cmd: ' + cmd + ', arg: ' + arg)
    str_incorrect = b'530 Login incorrect.\r\n'
    str_succ = b'230 Login successful.Have fun.\r\n'
    if username == 'anonymous':
        p = pwd.getpwnam("ftp")
        os.setegid(p.pw_gid)
        os.seteuid(p.pw_uid)
        os.chdir(str(p.pw_dir))
    else :
        p = pwd.getpwnam(username)
        print('username: ' + username)
        sp = spwd.getspnam(username)
        if not sp[1] and p :
            sock.send(str_incorrect)
            return
        if compare_hash(crypt.crypt(arg, sp[1]), sp[1]) == False:
            sock.send(str_incorrect)
            return

        os.setegid(p.pw_gid)
        os.seteuid(p.pw_uid)
        os.chdir(str(p.pw_dir))

    sock.send(str_succ)

def do_syst(sock, cmd, arg = None):
    str = b'215 Unix type.\r\n'
    sock.send(str)

def do_feat(sock, cmd, arg = None):
    str = b'211-Features:\r\n'
    sock.send(str)
    str_eprt = b' EPRT\r\n EPSV\r\n MDTM\r\n'
    sock.send(str_eprt)
    str_pasv = b' PASV\r\n REST STREAM\r\n SIZE\r\n TVFS\r\n UTF8\r\n211 End\r\n'
    sock.send(str_pasv)

def do_opts(sock, cmd, arg):
    print('do_opts cmd: ' + cmd + ', arg: ' + arg)
    if "UTF8 ON" in arg:
        str = b'200 Always in UTF8 mode.\r\n'
        sock.send(str)
    else:
        str = b'Option not understood.\r\n'
        sock.send(str)

def do_pwd(sock, cmd, arg = None):
    print("cur_dir: " + os.getcwd())
    str = '257 \"{}\" is the current directory\r\n'.format(os.getcwd())
    sock.send(str.encode())

def do_cwd(sock, cmd, arg = None):
    os.chdir(arg)
    str = '250 Directory successfully changed.\r\n'
    sock.send(str.encode())

def do_cdup(sock, cmd, arg = None):
    if os.getcwd() != '/':
        cur_dir = os.getcwd() + '/..'
        os.chdir(cur_dir)
    str = '250 Directory successfully changed.\r\n'
    sock.send(str.encode())

def get_file_time(file):
    mtime = os.path.getmtime(file)
    cur_time = time.time()
    date_format = "%b %d %H:%M"
    if mtime > cur_time or cur_time - mtime > 60*60*24*182:
        date_format = "%b %d  %Y"
    return datetime.fromtimestamp(mtime).strftime(date_format)

def list_detail(sock):
    for name in os.listdir(os.getcwd()):
        name_path = os.path.join(os.getcwd(), name)
        mode = os.stat(name_path).st_mode
        str_info = ''
        private = oct(mode)[-3:]
        #print('mode: ' + repr(oct(mode)) + ' private: ' + str(private))
        if stat.S_ISSOCK(mode):
            str_info += 's'
        elif stat.S_ISDIR(mode):
            str_info += 'd'
        elif stat.S_ISREG(mode):
            str_info += '-'
        elif stat.S_ISCHR(mode):
            str_info += 'c'
        elif stat.S_ISLNK(mode):
            str_info += 'l'
        elif stat.S_ISFIFO(mode):
            str_info += 'p'
        else:
            str_info += 'u'

        if stat.S_IRUSR &  int(private, 8) :
            str_info += 'r'
        else:
            str_info += '-'
        if  stat.S_IWUSR & int(private, 8) :
            str_info += 'w'
        else:
            str_info += '-'
        if stat.S_IXUSR & int(private, 8) :
            str_info += 'x'
        else:
            str_info += '-'
        if stat.S_IRGRP &  int(private, 8) :
            str_info += 'r'
        else:
            str_info += '-'
        if  stat.S_IWGRP & int(private, 8) :
            str_info += 'w'
        else:
            str_info += '-'
        if stat.S_IXGRP & int(private, 8) :
            str_info += 'x'
        else:
            str_info += '-'
        if stat.S_IROTH &  int(private, 8) :
            str_info += 'r'
        else:
            str_info += '-'
        if  stat.S_IWOTH & int(private, 8) :
            str_info += 'w'
        else:
            str_info += '-'
        if stat.S_IXOTH & int(private, 8) :
            str_info += 'x'
        else:
            str_info += '-'
        str_info += '{0:>4d}'.format(os.stat(name_path).st_nlink)
        str_info += ' {0:<5d}'.format(os.stat(name_path).st_uid)
        str_info += '{0:<5d}'.format(os.stat(name_path).st_gid)
        str_info += '{0:>12d}'.format(os.stat(name_path).st_size)
        str_info += ' {0:>13s}'.format(get_file_time(name_path))
        str_info += ' {0:<20s}'.format(name)
        str_info += '\r\n'
        #print(str_info)
        sock.send(str_info.encode())

def do_list(sock, cmd, argarg = None):
    global type_port_pasv
    global pasv_sock
    global port_sock
    print('do_list cmd: ' + cmd)
    if type_port_pasv == 1:
        print('port mode')
        str_resp = b'150 Here comes the directory list.\r\n'
        sock.send(str_resp)
        list_detail(port_sock)
        str_resp_ok = b'226 Directory send ok.\r\n'
        sock.send(str_resp_ok)
        port_sock.close()
    elif type_port_pasv == 2:
        print('pasv mode')
        str_resp = b'150 Here comes the directory list.\r\n'
        sock.send(str_resp)
        clientsocket, address = pasv_sock.accept()
        list_detail(clientsocket)
        clientsocket.close()
        str_resp_ok = b'226 Directory send ok.\r\n'
        sock.send(str_resp_ok)
    else:
        print('unknow mode')

def do_mkd(sock, cmd, arg = None):
    new_dir = os.getcwd() + '/' + arg
    os.mkdir(new_dir)
    str_info = '257 \"' + new_dir + '\" created successful.\r\n'
    sock.send(str_info.encode())

def do_rmd(sock, cmd, arg = None):
    new_dir = os.getcwd() + '/' + arg
    if os.path.exists(new_dir):
        os.rmdir(new_dir)
        sock.send(b'250 Remove directory operation successful.\r\n')
    else:
        sock.send(b'550 Delete operation failed, directory does exist.\r\n')

def do_dele(sock, cmd, arg = None):
    new_file = os.getcwd() + '/' + arg
    if os.path.exists(new_file):
        os.unlink(new_file)
        sock.send(b'250 Delete operation successful.\r\n')
    else:
        sock.send(b'550 Delete operation failed, file does exist.\r\n')

def do_pasv(sock, cmd, arg = None):
    global type_port_pasv
    global pasv_sock
    print('do_pasv cmd: ' + cmd)

    pasv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    pasv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    pasv_sock.bind(('0.0.0.0', 0))
    pasv_sock.listen(1)
    print(repr(sock.getsockname()))
    print(repr(pasv_sock.getsockname()))
    pasv_ip = sock.getsockname()[0];
    pasv_port = pasv_sock.getsockname()[1];
    print('ip: ' + pasv_ip + ', port: ' + str(pasv_port))
    pasv_str = '227 Entering Passive Mode({},{},{}).\r\n'.format(pasv_ip.replace('.', ','), (pasv_port >> 8) & 0xFF, pasv_port &0xFF)
    sock.send(pasv_str.encode())
    type_port_pasv = 2

def do_rest(sock, cmd, arg = None):
    print('do_rest cmd: ' + cmd)

def do_size(sock, cmd, arg = None):
    print('do_size cmd: ' + cmd)

def do_tvfs(sock, cmd, arg = None):
    print('do_tvfs cmd: ' + cmd)

def do_utf8(sock, cmd, arg = None):
    print('do_utf8 cmd: ' + cmd)

def do_type(sock, cmd, arg = None):
    print('do_type cmd: ' + cmd)
    sock.send(b'200 Switching to Binary mode.\r\n')

def do_size(sock, cmd, arg = None):
    print('do_size cmd: ' + cmd)
    new_file = os.getcwd() + '/' + arg
    if os.path.exists(new_file):
        str_cmd = '213 {}\r\n'.format(os.path.getsize(new_file))
        sock.send(str_cmd.encode())
    else:
        sock.send(b'550 Could not get file size.\r\n')

def ftp_upload_thread(ctrl_sock, data_sock, file_path):
    f= open(file_path, "wb")
    while True:
       data = data_sock.recv(2048)
       if not data:
           break
       f.write(data)
    f.close()
    data_sock.close()
    ctrl_sock.send(b'226 Transfer complete.\r\n')

def do_stor(sock, cmd, arg = None):
    global type_port_pasv
    global pasv_sock
    global port_sock
    print('do_stor cmd: ' + cmd)
    file_path = os.getcwd() + '/' + arg
    print('upload file: ' + file_path)
    sock.send(b'150 Ok to send data.\r\n')
    if type_port_pasv == 1:
        data_sock = port_sock
    elif type_port_pasv == 2:
        data_sock, address = pasv_sock.accept()
    stor_th = threading.Thread(target=ftp_upload_thread, name="ftp_upload_thread", args=(sock, data_sock, file_path))
    stor_th.start()


def do_mdtm(sock, cmd, arg = None):
    print('do_mdtm cmd: ' + cmd + ' arg:' + arg)
    index = arg.find(' ')
    if index == 8 or index == 14 or index > 15:
        file_time = arg[0 : index]
        file = arg[index + 1: ]
        print('file_time: '+ file_time + ' file: ' + file)
        if os.path.exists(file):
            timestamp = time.mktime(time.strptime(file_time, "%Y%m%d%H%M%S")) + float(8 * 60 * 60)
            os.utime(file, (timestamp, timestamp ))
        sock.send(b'213 File modification time set.\r\n')
    else:
        file = arg
        if os.path.exists(file):
            mtime = os.path.getmtime(file) - float(8 * 60 * 60)
            str_mtime = datetime.fromtimestamp(mtime).strftime('%Y%m%d%H%M%S')
            str_info = '213 {}\r\n'.format(str_mtime)
            sock.send(str_info.encode())

def ftp_download_thread(ctrl_sock, data_sock, file_path):
    f = open(file_path, "rb")
    os.sendfile(data_sock.fileno(), f.fileno(), 0, os.path.getsize(file_path))
    f.close()
    data_sock.close()
    ctrl_sock.send(b'226 Transfer complete.\r\n')

def do_retr(sock, cmd, arg = None):
    global type_port_pasv
    global pasv_sock
    global port_sock
    global data_sock
    print('do_retr cmd: ' + cmd)
    file_name = os.getcwd() + '/' + arg
    if os.path.exists(file_name):
        size = os.path.getsize(file_name)
        str_150 = '150 Opening BINARY mode data connection for ' + arg + ' ({} bytes).\r\n'.format(size)
        sock.send(str_150.encode())
        if type_port_pasv == 1:
            data_sock = port_sock
        elif type_port_pasv == 2:
            data_sock, address = pasv_sock.accept()

        retr_th = threading.Thread(target=ftp_download_thread, name="ftp_download_thread",
                                   args=(sock, data_sock, file_name))
        retr_th.start()
    else:
        sock.send(b'550 download file failed, file does exist.\r\n')

def do_eprt(sock, cmd, arg = None):
    print('do_eprt cmd: ' + cmd)

def do_epsv(sock, cmd, arg = None):
    print('do_epsv cmd: ' + cmd)

##192,168,42,1,123,432
def get_ip_port(arg):
    ip, port_hi, port_low = re.findall("^(\d+.\d+.\d+.\d+).(\d+).(\d+$)", arg.replace(',', '.'))[0]
    return ip, int((int(port_hi) << 8) + int(port_low))

def do_port(sock, cmd, arg = None):
    global port_sock
    print('do_port cmd: ' + cmd)
    global type_port_pasv
    type_port_pasv = 1
    ip, port = get_ip_port(arg)
    print('ip: ' + ip + ', port: ' + str(port))
    port_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    port_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    port_sock.connect((ip, port));
    sock.send(b'200 PORT command successful.Consider using PASV.\r\n')

def do_quit(sock, cmd, arg = None):
    sock.send(b'221 Goodbye!\r\n')
    #sock.close()
