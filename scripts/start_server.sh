#!/bin/bash
cd /home/ec2-user/app
touch log.txt
g++ server.cpp -pthread -o server
./server 3425 > log.txt &
