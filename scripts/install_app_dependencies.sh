#!/bin/bash
cd /home/ec2-user/app
if [ -e log.txt ]; then
    echo "Лог существует"
else
    touch log.txt
fi
sudo yum install -y gcc-c++
