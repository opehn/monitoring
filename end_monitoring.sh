#!/bin/bash

agent_num=$(ps -A | grep -w "agent" | wc -l)
 
readarray -t pids < <(ps -A | grep -w "agent")

for((i=0; i < $agent_num; i++))
do
	kill $(echo ${pids[$i]} | awk '{print $1}')
done

kill $(ps -A | grep -w "server" | awk '{print $1}')

