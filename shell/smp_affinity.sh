#!/bin/bash
LOG_FILE=/opt/bbu/oam/log/enp184s0f1_smp_affinity.log
interrupt_id=$(cat /proc/interrupts | grep i40e-enp184s0f1-TxRx | awk -F ":" '{print $1}')

>${LOG_FILE}

echo "=============================before change smp_affinity ============================================ " >> ${LOG_FILE}
for id in ${interrupt_id}; do echo -n "$id: "; cat /proc/irq/${id}/smp_affinity ; echo ""; done  >> ${LOG_FILE}
##cpu0  1
##cpu1  2
##cpu2  4
##cpu3  8
##cpu4  16
##cpu5  32
##cpu6  64
##cpu7  128
##cpu8  256
##cpu9 512
##cpu10 1024
##cpu11 == 2048
for id in ${interrupt_id}; do echo "2048" > /proc/irq/${id}/smp_affinity; done

echo "=============================change smp_affinity result ============================================ " >> ${LOG_FILE}
for id in ${interrupt_id}; do echo -n "$id: "; cat /proc/irq/${id}/smp_affinity ; echo ""; done  >> ${LOG_FILE}
