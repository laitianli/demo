#!/bin/bash
NIC_NAME=ens38
VLANID=5
VLAN_NIC=${NIC_NAME}.${VLANID}

ip link set dev ${VLAN_NIC} down
ip link delete ${VLAN_NIC}
