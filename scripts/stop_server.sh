#!/bin/bash
ps cax | grep server
if [ $? -eq 0 ]; then
  echo "Process is not running."
else
  echo "Process is not running."
fi
