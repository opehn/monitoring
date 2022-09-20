#!/bin/bash

declare -a agent_id
read -p "Enter number of agent to run : " agent_num

for((i=0; i < $agent_num; i++))
do
	read -p "Enter $((i+1)) agent id : " agent_id[$i]
done

./server/server $agent_num

for((i=0; i < $agent_num; i++))
do
	LD_PRELOAD=./libs/udp_hook/send_hooking.so ./agent/agent ${agent_id[$i]}
done
