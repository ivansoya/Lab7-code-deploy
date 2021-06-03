#!/bin/bash
ps cax | grep server > /dev/null
if [ $? -eq 0 ]; then
  killall server
else
  echo "Process is not running."
fi
