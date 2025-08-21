#!/usr/bin/env python
# _*_ coding:utf-8 _*_

import libvirt
import socket
import threading
import time
from libvirt import libvirtError

CONN_SOCKET = 4
CONN_TLS = 3
CONN_SSH = 2
CONN_TCP = 1
TLS_PORT = 16514
SSH_PORT = 22
TCP_PORT = 16509


class wvmEventLoop(threading.Thread):
    def __init__(self, group=None, target=None, name=None, args=(), kwargs={}):
        # register the default event implementation
        # of libvirt, as we do not have an existing
        # event loop.
        libvirt.virEventRegisterDefaultImpl()

        if name is None:
            name = "libvirt event loop"

        super(wvmEventLoop, self).__init__(group, target, name, args, kwargs)

        # we run this thread in deamon mode, so it does
        # not block shutdown of the server
        self.daemon = True

    def run(self):
        while True:
            # if this method will fail it raises libvirtError
            # we do not catch the exception here so it will show up
            # in the logs. Not sure when this call will ever fail
            libvirt.virEventRunDefaultImpl()


class ConnectLibvirtd:
    def __init__(self, type=CONN_SOCKET, host=None, login_username=None):
        self.type = type
        self.conn = None
        self.host = host
        self.login_username = login_username
        self.last_error = None


    def __libvirt_auth_credentials_callback(self, credentials, user_data):
        for credential in credentials:
            if credential[0] == libvirt.VIR_CRED_AUTHNAME:
                credential[4] = self.login
                if len(credential[4]) == 0:
                    credential[4] = credential[3]
            elif credential[0] == libvirt.VIR_CRED_PASSPHRASE:
                credential[4] = self.passwd
            else:
                return -1
        return 0


    def __connection_close_callback(self, connection, reason, opaque=None):
        try:
            if libvirt is not None:
                if reason == libvirt.VIR_CONNECT_CLOSE_REASON_ERROR:
                    self.last_error = "connection closed: Misc I/O error"
                elif reason == libvirt.VIR_CONNECT_CLOSE_REASON_EOF:
                    self.last_error = "connection closed: End-of-file from server"
                elif reason == libvirt.VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
                    self.last_error = "connection closed: Keepalive timer triggered"
                elif reason == libvirt.VIR_CONNECT_CLOSE_REASON_CLIENT:
                    self.last_error = "connection closed: Client requested it"
                else:
                    self.last_error = "connection closed: Unknown error"
        finally:
            pass


    def __connect_by_tcp(self):
        flags = [libvirt.VIR_CRED_AUTHNAME, libvirt.VIR_CRED_PASSPHRASE]
        auth = [flags, self.__libvirt_auth_credentials_callback, None]
        uri = "qemu+tcp://%s/system" % self.host
        try:
            self.conn = libvirt.openAuth(uri, auth, 0)
            self.last_error = None
        except libvirtError as e:
            self.last_error = "Connect %s Failed: " % uri
            self.last_error += str(e)
        if self.last_error:
            print(self.last_error)
            return False
        else:
            print("connect %s success!" % (uri))
        return True


    def __connect_by_tls(self):
        flags = [libvirt.VIR_CRED_AUTHNAME, libvirt.VIR_CRED_PASSPHRASE]
        auth = [flags, self.__libvirt_auth_credentials_callback, None]
        uri = "qemu+tls://%s@%s/system" % (self.login, self.host)
        try:
            self.conn = libvirt.openAuth(uri, auth, None)
            self.last_error = None
        except libvirtError as e:
            self.last_error = "Connect %s Failed: " % uri
            self.last_error += str(e)
            self.conn = None
        if self.last_error:
            print(self.last_error)
            return False
        else:
            print("connect %s success!" % (uri))
        return True


    def __connect_by_unix_socket(self):
        uri = "qemu:///system"
        try:
            self.conn = libvirt.open(uri)
            self.last_error = None
        except libvirtError as e:
            self.last_error = "Connect %s Failed: " % uri
            self.last_error += str(e)
        if self.last_error:
            print(self.last_error)
            return False
        else:
            print("connect %s success!" % (uri))
        return True


    def __connect_by_ssh(self):
        uri = "qemu+ssh://%s@%s/system" % (self.login, self.host)
        try:
            self.conn = libvirt.open(uri)
            self.last_error = None
        except libvirtError as e:
            self.last_error = "Connect %s Failed: " % uri
            self.last_error += str(e)
            self.conn = None
        if self.last_error:
            print(self.last_error)
            return False
        else:
            print("connect %s success!" % (uri))
        return True


    def connect_to_libvirt(self):
        _event_loop = wvmEventLoop()
        _event_loop.start()
        self.__connect_by_unix_socket()
        # connect_by_ssh("root", "172.16.123.137")
        if self.conn is not None:
            keepalive_interval = 5
            keepalive_count = 5
            try:
                self.conn.setKeepAlive(keepalive_interval, keepalive_count)
                try:
                    self.conn.registerCloseCallback(self.__connection_close_callback, None)
                except:
                    pass
            except self.libvirtError as e:
                if self.last_error:
                    self.last_error += str(e)
                else:
                    self.last_error = str(e)
                    return False
                    
        return True
    
    def get_conn(self):
        return self.conn
    

    def connect_close(self):
        if self.conn is not None and self.conn.isAlive():
            try:
                self.conn.unregisterCloseCallback()
                self.conn.close()
    
            except libvirtError as e:
                print("close failed")
                return 
            print("close success!")




if __name__ == "__main__":
    conn = ConnectLibvirtd()
    conn.connect_to_libvirt()

# =============================================================================
#     if ret_info is None:
#         do_connect(conn)
# =============================================================================

    # while True:
    #     print('timer.sleep(6)-----------')
    #     time.sleep(6)
    conn.connect_close()
