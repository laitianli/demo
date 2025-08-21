# -*- coding: utf-8 -*-
"""
Created on Wed Aug 20 16:25:13 2025

@author: ltl
"""

"""
<guest>
    <os_type>hvm</os_type>
    <arch name="x86_64">
      <wordsize>64</wordsize>
      <emulator>/usr/libexec/qemu-kvm</emulator>
      <machine maxCpus="240">pc-i440fx-rhel7.0.0</machine>
      <machine canonical="pc-i440fx-rhel7.0.0" maxCpus="240">pc</machine>
      <machine maxCpus="240">rhel6.0.0</machine>
      <machine maxCpus="240">rhel6.1.0</machine>
      <machine maxCpus="240">rhel6.2.0</machine>
      <machine maxCpus="240">rhel6.3.0</machine>
      <machine maxCpus="240">rhel6.4.0</machine>
      <machine maxCpus="240">rhel6.5.0</machine>
      <machine maxCpus="240">rhel6.6.0</machine>
      <domain type="qemu"/>
      <domain type="kvm">
        <emulator>/usr/libexec/qemu-kvm</emulator>
      </domain>
    </arch>
    <features>
      <cpuselection/>
      <deviceboot/>
      <disksnapshot default="off" toggle="no"/>
      <acpi default="on" toggle="yes"/>
      <apic default="on" toggle="no"/>
    </features>
  </guest>
"""
import libxml2
class getCapabilities():
    def __init__(self, xml):
        self.capxml = xml
        self.os_type = 'hvm'
        self.arch_name = None
        self.wordsize = '64'
        self.emulator_type = 'kvm'
        self.emulator=None
        self.listMachines = []
        
    def __str__(self):
        return "Guest Capabilities: arch: %s, wordsize: %s emulator_type: %s, emulator: %s machines: %s" % \
            (self.arch_name, self.wordsize, self.emulator_type, self.emulator, self.listMachines)
        
    def parseGuestXML(self):
        doc = libxml2.parseDoc(self.capxml)
        try:
            xpath_ctx = doc.xpathNewContext()
            arch_node = xpath_ctx.xpathEval('//arch[@name="x86_64"]')[0]
            if arch_node is not None:
                self.arch_name = 'x86_64'
                wordsize = arch_node.xpathEval('./wordsize')[0].content
                if wordsize:
                    self.wordsize = wordsize
                self.listMachines = [node.content for node in arch_node.xpathEval('./machine')]
                # print(self.listMachines)
                kvm_emulator = arch_node.xpathEval("./domain[@type='kvm']/emulator")
                if kvm_emulator:
                    self.emulator_type = 'kvm'
                    self.emulator = kvm_emulator[0].content
                
            # print(arch_node)
        finally:
            xpath_ctx.xpathFreeContext()
            doc.freeDoc()
            

if __name__ == '__main__':
    xml = None
    with open('./xml/capabilities.xml', 'r') as file:
        xml = file.read()
    if xml is not None:
        caps = getCapabilities(xml)
        caps.parseGuestXML()
        print(caps)
        print(caps.listMachines)
        