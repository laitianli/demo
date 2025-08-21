# -*- coding: utf-8 -*-
"""
Created on Wed Aug 20 14:36:41 2025

@author: ltl
"""
import libxml2
import util
import getCapabilities

class createVMXML():
    def __init__(self, name):
        self.name = name
        self.memTotal = 131072  #128M
        self.memCurrent = 131072 #128M
        self.vcpu = 1
        self.strXML = None
        
    def setMemory(self, memTotal, memCurrent):
        self.memTotal = memTotal
        self.memCurrent = memCurrent
    
    def setVCPU(self, vcpu_count):
        self.vcpu = vcpu_count
    
    def __createMemoryNode(self, parentNode):
        if parentNode is not None:
            memoryNode = parentNode.newChild(None, 'memory', '%d' % self.memTotal)
            memoryNode.setProp('unit', 'KiB')
            
            currMemoryNode = parentNode.newChild(None, 'currentMemory', '%d' % self.memCurrent)
            currMemoryNode.setProp('unit', 'KiB')
    
    def __createVCPUNode(self, parentNode):
        if parentNode is not None:
            vcpuNode = parentNode.newChild(None, 'vcpu', '%d' % self.vcpu)
            vcpuNode.setProp('placement', 'static')
    
    def __createOSNode(self, parentNode):
        if parentNode is not None:
            osNode = libxml2.newNode('os')
            parentNode.addChild(osNode)
            
            typeNode = libxml2.newNode('type')
            typeNode.setProp('arch', 'x86_64')
            typeNode.setProp('machine', 'pc-i440fx-rhel7.0.0')
            typeNode.setContent('hvm')
            osNode.addChild(typeNode)
            
            bootNode = libxml2.newNode('boot')
            # hd:从硬盘启动(Hard Disk)，fd: 从软盘启动(Floppy Disk)，cdrom: 从光驱启动(CD-ROM)，network:网络启动(PXE)
            bootNode.setProp('dev', 'hd')
            bootNode.setProp('dev', 'cdrom')
            osNode.addChild(bootNode)
            
            
    def __createFeaturesNode(self, parentNode):
        if parentNode is not None:
            featuresNode = libxml2.newNode('features')
            acpiNode = libxml2.newNode('acpi')
            apicNode = libxml2.newNode('apic')
            featuresNode.addChild(acpiNode)
            featuresNode.addChild(apicNode)
            parentNode.addChild(featuresNode)
        
    def __addcpufeatureNode(self, cpuNode, strPolicy, strFeatureVal):
        featureNode = cpuNode.newChild(None, 'feature', None)
        featureNode.setProp('policy', '%s' % strPolicy)
        featureNode.setProp('name', '%s' % strFeatureVal)
        
    def __createCPUNode(self, parentNode):
        if parentNode is not None:
            cpuNode = libxml2.newNode('cpu')
            cpuNode.setProp('mode','custom')
            cpuNode.setProp('check', 'full')
            parentNode.addChild(cpuNode)
            
            modelNode = cpuNode.newChild(None, 'model', 'Skylake-Server-IBRS')
            modelNode.setProp('fallback', 'forbid')
            
            
            cpuNode.newChild(None, 'vendor', 'Intel')
            self.__addcpufeatureNode(cpuNode, 'disable', 'ds')
            self.__addcpufeatureNode(cpuNode, 'disable', 'acpi')
            self.__addcpufeatureNode(cpuNode, 'require', 'ss')
            self.__addcpufeatureNode(cpuNode, 'disable', 'ht')
            self.__addcpufeatureNode(cpuNode, 'disable', 'tm')
            self.__addcpufeatureNode(cpuNode, 'disable', 'dtes64')
            self.__addcpufeatureNode(cpuNode, 'disable', 'monitor')
            self.__addcpufeatureNode(cpuNode, 'disable', 'ds_cpl')
            self.__addcpufeatureNode(cpuNode, 'disable', 'vmx')
            self.__addcpufeatureNode(cpuNode, 'disable', 'smx')
            self.__addcpufeatureNode(cpuNode, 'disable', 'est')
            self.__addcpufeatureNode(cpuNode, 'disable', 'tm2')
            self.__addcpufeatureNode(cpuNode, 'disable', 'xtpr')
            self.__addcpufeatureNode(cpuNode, 'disable', 'pdcm')
            self.__addcpufeatureNode(cpuNode, 'disable', 'dca')
            self.__addcpufeatureNode(cpuNode, 'disable', 'osxsave')
            self.__addcpufeatureNode(cpuNode, 'disable', 'tsc_adjust')
            
            
            self.__addcpufeatureNode(cpuNode, 'require', 'clflushopt')
            self.__addcpufeatureNode(cpuNode, 'disable', 'intel-pt')
            self.__addcpufeatureNode(cpuNode, 'disable', 'pku')
            self.__addcpufeatureNode(cpuNode, 'disable', 'md-clear')
            self.__addcpufeatureNode(cpuNode, 'disable', 'stibp')
            self.__addcpufeatureNode(cpuNode, 'disable', 'ssbd')
            self.__addcpufeatureNode(cpuNode, 'require', 'hypervisor')
            self.__addcpufeatureNode(cpuNode, 'disable', 'arat')
            self.__addcpufeatureNode(cpuNode, 'disable', 'spec-ctrl')
            
        
    def __createClockNode(self, parentNode):
        if parentNode is not None:
            clockNode = libxml2.newNode('clock')
            clockNode.setProp('offset', 'utc')
            parentNode.addChild(clockNode)
            
            timerRTCNode = libxml2.newNode('timer')
            timerRTCNode.setProp('name', 'rtc')
            timerRTCNode.setProp('tickpolicy', 'catchup')
            clockNode.addChild(timerRTCNode)
            
            timerPITNode = libxml2.newNode('timer')
            timerPITNode.setProp('name', 'pit')
            timerPITNode.setProp('tickpolicy', 'delay')
            clockNode.addChild(timerPITNode)
            
            timerHPETNode = libxml2.newNode('timer')
            timerHPETNode.setProp('name', 'hpet')
            timerHPETNode.setProp('present', 'no')
            clockNode.addChild(timerHPETNode)
            
            
            
    def __createPMNode(self, parentNode):
        if parentNode is not None:
            pmNode = libxml2.newNode('pm')
            parentNode.addChild(pmNode)
            
            suspend2memNode = libxml2.newNode('suspend-to-mem')
            suspend2memNode.setProp('enabled', 'no')
            pmNode.addChild(suspend2memNode)
            
            suspend2diskNode = libxml2.newNode('suspend-to-disk')
            suspend2diskNode.setProp('enabled', 'no')
            pmNode.addChild(suspend2diskNode)
            
                  
    
    def __createDevice(self, parentNode):
        if parentNode is not None:
            devNode = libxml2.newNode('devices')
            devNode.newChild(None, 'emulator', '/usr/libexec/qemu-kvm')
            parentNode.addChild(devNode)
            
            self.__addISODisk2Device(devNode)
            self.__addDisk2Device(devNode)
            
            self.__addEHCI2Device(devNode)
            
            # for portID in range(3):
            #     self.__addUHCI2Device(devNode, portID+1)
                
            self.__addIDEController2Device(devNode)
            
            self.__addVirtioSerialController2Device(devNode)
            
            self.__addPCIRootController2Device(devNode)
            
            self.__addInterface2Device(devNode)
            
            self.__addSerial2Device(devNode)
            
            self.__addConsole2Device(devNode)
            #self.__addSpicevmcChann2Device(devNode)
            
            self.__addInputMouse2Device(devNode)
            self.__addInputKeyboard2Device(devNode)
            
            self.__addGraphics2Device(devNode)
            
            self.__addMemballoon2Device(devNode)
                
    def __addISODisk2Device(self, devNode):
        if devNode is not None:
            diskNode = libxml2.newNode('disk')
            diskNode.setProp('type', 'file')
            diskNode.setProp('device', 'cdrom')
            devNode.addChild(diskNode)
            
            subDrvNode = libxml2.newNode('driver')
            subDrvNode.setProp('name', 'qemu')
            subDrvNode.setProp('type', 'raw')
            diskNode.addChild(subDrvNode)
            
            subSrcFileNode = libxml2.newNode('source')
            subSrcFileNode.setProp('file', '/home/os/CentOS-7-x86_64-Everything-1708.iso')
            diskNode.addChild(subSrcFileNode)
            
            subTargetNode = libxml2.newNode('target')
            subTargetNode.setProp('dev', 'hda')
            subTargetNode.setProp('bus', 'ide')
            diskNode.addChild(subTargetNode)
            
            diskNode.newChild(None, 'readonly', None)
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'drive')
            subAddressNode.setProp('controller', '0')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('target', '0')
            subAddressNode.setProp('unit', '0')
            diskNode.addChild(subAddressNode)
            
    def __addDisk2Device(self, devNode):
        if devNode is not None:
            diskNode = libxml2.newNode('disk')
            diskNode.setProp('type', 'file')
            diskNode.setProp('device', 'disk')
            devNode.addChild(diskNode)
            
            subDrvNode = libxml2.newNode('driver')
            subDrvNode.setProp('name', 'qemu')
            subDrvNode.setProp('type', 'qcow2')
            diskNode.addChild(subDrvNode)
            
            subSrcFileNode = libxml2.newNode('source')
            subSrcFileNode.setProp('file', '/var/lib/libvirt/images/centos7.0.qcow2')
            diskNode.addChild(subSrcFileNode)
            
            subTargetNode = libxml2.newNode('target')
            subTargetNode.setProp('dev', 'vda')
            subTargetNode.setProp('bus', 'virtio')
            diskNode.addChild(subTargetNode)
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            diskNode.addChild(subAddressNode)
    
    def __addEHCI2Device(self, devNode):
        if devNode is not None:
            ctrNode = libxml2.newNode('controller')
            ctrNode.setProp('type', 'usb')
            ctrNode.setProp('index', '0')
            ctrNode.setProp('model', 'ich9-ehci1')
            devNode.addChild(ctrNode)
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            ctrNode.addChild(subAddressNode)
               

    def __addUHCI2Device(self, devNode, portID):
        if devNode is not None:
            ctrNode = libxml2.newNode('controller')
            ctrNode.setProp('type', 'usb')
            ctrNode.setProp('index', '0')
            ctrNode.setProp('model', 'ich9-uhci%d' % portID)
            devNode.addChild(ctrNode)
            
            masterNode = ctrNode.newChild(None, 'master', None)
            masterNode.setProp('startport', '%s' % portID) 
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            ctrNode.addChild(subAddressNode)
            
    def __addIDEController2Device(self, devNode):
        if devNode is not None:
            ctrNode = libxml2.newNode('controller')
            ctrNode.setProp('type', 'ide')
            ctrNode.setProp('index', '0')
            devNode.addChild(ctrNode)
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            ctrNode.addChild(subAddressNode)
             
    def __addVirtioSerialController2Device(self, devNode):
        if devNode is not None:
            ctrNode = libxml2.newNode('controller')
            ctrNode.setProp('type', 'virtio-serial')
            ctrNode.setProp('index', '0')
            devNode.addChild(ctrNode)
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            ctrNode.addChild(subAddressNode)
            
    def __addPCIRootController2Device(self, devNode):
        if devNode is not None:
            ctrNode = libxml2.newNode('controller')
            ctrNode.setProp('type', 'pci')
            ctrNode.setProp('index', '0')
            ctrNode.setProp('model', 'pci-root')
            devNode.addChild(ctrNode)
    
    def __addInterface2Device(self, devNode):
        if devNode is not None:
            ifNode = libxml2.newNode('interface')
            ifNode.setProp('type', 'direct')
            devNode.addChild(ifNode)
            
            macNode = libxml2.newNode('mac')
            macNode.setProp('address', '%s' % util.randomMAC())
            ifNode.addChild(macNode)
            
            srcNode = libxml2.newNode('source')
            srcNode.setProp('dev', 'enp3s0')
            srcNode.setProp('mode', 'bridge')
            ifNode.addChild(srcNode)
            
            modelNode = ifNode.newChild(None, 'model', None)
            modelNode.setProp('type', 'e1000')
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            ifNode.addChild(subAddressNode)
            
    def __addSerial2Device(self, devNode):
        if devNode:
            serialNode = libxml2.newNode('serial')
            serialNode.setProp('type', 'pty')
            devNode.addChild(serialNode)
            
            subTargetNode = serialNode.newChild(None, 'target', None)
            subTargetNode.setProp('port', '0')
            
    def __addConsole2Device(self, devNode):
        if devNode:
            consoleNode = libxml2.newNode('console')
            consoleNode.setProp('type', 'pty')
            devNode.addChild(consoleNode)
            
            subTargetNode = consoleNode.newChild(None, 'target', None)
            subTargetNode.setProp('type', 'serial')
            subTargetNode.setProp('port', '0')
            
    def __addSpicevmcChann2Device(self, devNode):
        if devNode:
            channNode = libxml2.newNode('channel')
            channNode.setProp('type', 'spicevmc')
            devNode.addChild(channNode)
            
            subTargetNode = channNode.newChild(None, 'target', None)
            subTargetNode.setProp('type', 'virtio')
            subTargetNode.setProp('name', 'comm.redhat.spice.0')
            
            subAddressNode = channNode.newChild(None, 'address', None)
            subAddressNode.setProp('type', 'virtio-serial')
            subAddressNode.setProp('controller', '0')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('port', '2')
            
    def __addInputMouse2Device(self, devNode):
        if devNode is not None:
            inputNode = libxml2.newNode('input')
            inputNode.setProp('type', 'mouse')
            inputNode.setProp('bus', 'ps2')
            devNode.addChild(inputNode)

    def __addInputKeyboard2Device(self, devNode):
        if devNode is not None:
            keyboardNode = libxml2.newNode('input')
            keyboardNode.setProp('type', 'keyboard')
            keyboardNode.setProp('bus', 'ps2')
            devNode.addChild(keyboardNode)
    
    """
    <graphics type='vnc' port='-1' autoport='yes' websocket='5700' listen='0.0.0.0'>
      <listen type='address' address='0.0.0.0'/>
    </graphics>
    """
    def __addGraphics2Device(self, devNode):
        if devNode:
            graphicsNode = devNode.newChild(None, 'graphics', None)
            graphicsNode.setProp('type', 'vnc')
            graphicsNode.setProp('port', '-1')
            graphicsNode.setProp('autoport', 'yes')
            graphicsNode.setProp('websocket', '-1')
            graphicsNode.setProp('listen', '0.0.0.0')
            subListenNode = graphicsNode.newChild(None, 'listen', None)
            subListenNode.setProp('type', 'address')
            subListenNode.setProp('address', '0.0.0.0')
            
            subImageNode = graphicsNode.newChild(None, 'image', None)
            subImageNode.setProp('compression', 'off')
            
    def __addMemballoon2Device(self, devNode):
        if devNode:
            memballoonNode = devNode.newChild(None, 'memballoon', None)
            memballoonNode.setProp('model', 'virtio')
            
            subAddressNode = libxml2.newNode('address')
            subAddressNode.setProp('type', 'pci')
            subAddressNode.setProp('domain', '0x0000')
            subAddressNode.setProp('bus', '0')
            subAddressNode.setProp('slot', '0')
            subAddressNode.setProp('function', '0')
            memballoonNode.addChild(subAddressNode)
            
    
    def create(self):
        # 创建 XML 文档
        doc = libxml2.newDoc("1.0")
        
        # 创建根节点 <domain type='kvm'>
        root = doc.newDocNode(None, "domain", None)
        root.setProp("type", "kvm")  # 设置属性 type='kvm'
        doc.setRootElement(root)     # 设为根节点
        
        # 创建子节点 <name>centos</name>
        root.newChild(None, "name", "%s" % self.name)  # 直接指定节点内容
        root.newChild(None, 'uuid', '%s' % util.randomUUID())
        ##创建内存节点
        self.__createMemoryNode(root)
        
        ##创建vcpu节点
        self.__createVCPUNode(root)
        
        ##创建os节点
        self.__createOSNode(root)
        
        ##创建features节点
        self.__createFeaturesNode(root)
        
        ##创建cpu节点
        self.__createCPUNode(root)
        
        ##创建clock节点
        self.__createClockNode(root)
        
        ##创建子节点on_poweroff on_reboot on_crash
        root.newChild(None, 'on_poweroff', 'destroy')
        root.newChild(None, 'on_reboot', 'restart')
        root.newChild(None, 'on_crash', 'destroy')
        
        ##创建pm节点
        self.__createPMNode(root)
        
        ##创建device节点
        self.__createDevice(root)
        
        # 输出 XML 到控制台（格式化）
        self.strXML = doc.serialize(encoding="UTF-8", format=1)
        #xml_str.replace("\"", "\'")
        print(self.strXML.decode('utf-8'))
        
        # 保存到文件（可选）
        # doc.saveFile("vm_config.xml")
        # with open('%s.xml' % self.name, 'w') as file:
        #     file.write(self.strXML)
        
        # 释放资源
        doc.freeDoc()


if __name__ == '__main__':
    xml = createVMXML('vm-centos')
    xml.setMemory(8388608, 8388608)
    xml.setVCPU(4)
    xml.create()
    
