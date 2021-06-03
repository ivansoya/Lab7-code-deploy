#!/bin/bash
ps cax | grep server > output.txt
if [ -s output.txt ]
    echo "There no processes"
else
    killall server
fi
rm output.txt
