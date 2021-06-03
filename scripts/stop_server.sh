#!/bin/bash
ps cax | grep server > output.txt
if [ -s output.txt ]
then
    echo "There no processes"
else
    sudo killall server
fi
rm output.txt
