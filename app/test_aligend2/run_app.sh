#!/bin/bash
PWD=$(dirname $0)
CORE_FILE=/root/test_core/core-%e-%p-%t

echo "${CORE_FILE}" > /proc/sys/kernel/core_pattern
ulimit -c unlimited

chmod +x t_unligned

./t_unligned 1
