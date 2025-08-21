#!/usr/bin/env python
#_*_ coding:utf-8 _*_ 

from LVconnect import ConnectLibvirtd
import util
from getCapabilities import getCapabilities
from createXML import createVMXML

def do_domain(vm):
    xml = vm.XMLDesc(0)
    print(xml) #獲取/etc/libvirt/qemu/vm-ubuntu22.04.xml文件內容
    emulator = util.get_xml_path(xml, "/domain/devices/emulator")
    print("emulator: %s" % emulator)
    def if_type_func(ctx):
        res = []
        for type in ctx.xpathEval('/domain/devices/interface/@type'):
            res.append(type.content)
        
        return res
            
            
    first_if_type = util.get_xml_path(xml, "/domain/devices/interface/@type")
    print("first_if_type: %s" % first_if_type)
    
    if_type = util.get_xml_path(xml, None, if_type_func)
    print("if_type: %s" % if_type)
    
    """ <interface type='network'>
      <mac address='52:54:00:80:a6:1d'/>
      <source network='default'/>
      <model type='virtio'/>
      <address type='pci' domain='0x0000' bus='0x00' slot='0x06' function='0x0'/>
    </interface>"""
    def if_mac_func(listXmlNode):
        res = []
        print(listXmlNode)
        for node in listXmlNode:
            child = node.children
            while child is not None:
                if child.name == 'mac':
                    mac = child.prop('address')
                    print('mac: %s' % mac)
                    if mac is not None:
                        res.append(mac)                    
                child = child.next
        return res
    if_mac = util.get_xml_path(xml, "/domain/devices/interface", if_mac_func)
    print('if_mac: %s' % if_mac)


def test_getCapabilites(conn):
    # virsh capabilities
    print("virsh capabilities: conn.getCapabilities():%s" % conn.getCapabilities())
    
def test_domain(conn):
    # virsh list --all
    print("virsh list --all: conn.listDefinedDomains(): %s " % conn.listDefinedDomains())
    for name in conn.listDefinedDomains():
        print("lookupByName(%s): %s" % (name, conn.lookupByName(name)))
        do_domain(conn.lookupByName(name))

def test_device(conn):
    # print("listAllDevices(0): %s" % conn.listAllDevices(0))
    ##獲取物理機設備列表
    # for dev in conn.listAllDevices(0):
    #     print('device name: %s' % dev.name())
    for dev in conn.listAllDevices(0):
        xml = dev.XMLDesc(0)
        print('device xml: %s' % xml)

def test_if(conn):
    ## 查找所有处理down状态的网口
    print("listDefinedInterfaces(): %s" % conn.listDefinedInterfaces())
    print('-' * 100 )
    print("listDefinedNetworks(): %s" % conn.listDefinedNetworks())
    print('-' * 100 )
    print("listDefinedStoragePools(): %s" % conn.listDefinedStoragePools())
    
    print('-' * 100 )
    print("listDomainsID(): %s" % conn.listDomainsID())
    
    print('-' * 100 )
    print("getVersion(): %s" % conn.getVersion())
    print('-' * 100 )
    print("listStoragePools(): %s" % conn.listStoragePools())
    print('-' * 100 )
    print("listNetworks(): %s" % conn.listNetworks())
    for net_name in conn.listNetworks():
        print("networkLookupByName(%s): %s" % (net_name, conn.networkLookupByName(net_name)))
        print(conn.networkLookupByName(net_name).XMLDesc(0))
    print('-' * 100 )
    print("listInterfaces(): %s" % conn.listInterfaces())
    for ifname in conn.listInterfaces():
        print("interfaceLookupByName(%s): %s" % (ifname, conn.interfaceLookupByName(ifname)))
        print(conn.interfaceLookupByName(ifname).XMLDesc(0))
    print('-' * 100 )
    
def test_createXML(conn):
    vm_name='text-vm-centos'
    listVMName = conn.listDefinedDomains()
    if vm_name not in listVMName:
        xmlobj = createVMXML(vm_name)
        xmlobj.setMemory(8388608, 8388608)
        xmlobj.setVCPU(4)
        xmlobj.create()
        ##在/etc/libvirt/qemu/创建相关的xml文件
        conn.defineXML(xmlobj.strXML) 
    
    vm = conn.lookupByName(vm_name)
    if vm:
        vm.create()

def do_connect(conn):
    #test_getCapabilites(conn)
    # test_domain(conn)
    # test_if(conn)
    # test_device(conn)
    test_createXML(conn)

    
def main():
    connLV = ConnectLibvirtd()
    connLV.connect_to_libvirt()
    if connLV.get_conn() is not None:
        do_connect(connLV.get_conn())        

    connLV.connect_close()

if __name__ == '__main__':
    main()