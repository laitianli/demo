# -*- coding: utf-8 -*-
"""
Created on Wed Aug 20 09:52:20 2025

@author: ltl
"""

import random
import libxml2

def randomMAC():
    oui = [0x52, 0x54, 0x00]
    mac = oui + [random.randint(0x00, 0xff),
                 random.randint(0x00, 0xff),
                 random.randint(0x00, 0xff)]
    return ':'.join(map(lambda x: "%02x" % x, mac))


def randomUUID():
    u = [random.randint(0, 255) for _ in range(0, 16)]
    return '-'.join(["%02x" * 4, "%02x" * 2, "%02x" * 2, "%02x" * 2, "%02x" * 6]) % tuple(u)


def get_xml_path(xml, path=None, func=None):
    doc = None
    ctx = None
    result = None
    
    try:
        doc = libxml2.parseDoc(xml)
        ctx = doc.xpathNewContext()
        if path is not None:
            ret = ctx.xpathEval(path)
            if ret is not None:
                if type(ret) == list:
                    if len(ret) >= 1:
                        if func:
                            result = func(ret)
                        else:
                            result = ret[0].content
                else:
                    result = ret
                    
        elif func:
            result = func(ctx)
        else:
            raise ValueError("'path' or 'func' argument is required!")
    finally:
        if doc is not None:
            doc.freeDoc()
        if ctx is not None:
            ctx.xpathFreeContext()
    
    return result

def test():
    print("mac: %s" % randomMAC())
    print("uuid: %s" % randomUUID())
    
    list1 = [1, 2, 3]
    list2 = [4, 5, 6]
    result = map(lambda x, y: x + y, list1, list2)
    print(list(result))  # 输出: [5, 7, 9]
    
    names = ['alice', 'bob', 'charlie']
    formatted = map(str.capitalize, names)
    print(list(formatted))  # 输出: ['Alice', 'Bob', 'Charlie']

if __name__ == '__main__':
    
    test()
    
    