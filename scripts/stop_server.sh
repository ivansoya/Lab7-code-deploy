#!/bin/bash
ps cax | grep server
if [ $? -eq 0 ]; then
  killall server
else
  echo "Process is not running."
fi
