#!/bin/bash
cd /home/ec2-user/app
g++ server.cpp -pthread -o server
./server 3425 
