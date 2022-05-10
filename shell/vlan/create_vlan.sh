#!/bin/bash
NIC_NAME=ens38
VLANID=5
VLAN_NIC=${NIC_NAME}.${VLANID}
VLAN_IP=20.20.20.3/24
VLAN_BRD=20.20.20.255

ip link add link ${NIC_NAME}  name ${VLAN_NIC} type vlan id ${VLANID}
#ip link
ip -d link show ${VLAN_NIC}
ip addr add ${VLAN_IP} brd ${VLAN_BRD} dev ${VLAN_NIC}
ip link set dev ${VLAN_NIC} up
ip addr 
