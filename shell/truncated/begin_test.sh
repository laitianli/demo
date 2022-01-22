#!/bin/bash
PWD=$(dirname $0)
echo "pwd=$PWD"
nohup ${PWD}/run_test.sh | ${PWD}/truncatedOutPut.sh ${PWD}/log_run_test.txt &
