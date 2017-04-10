#!/bin/bash

for i in `seq 1 $1`;
do
        echo "Starting team $i"
	for j in `seq 1 $2`;
	do
		./lemipc `pwd` $i&
                if [ $i -eq 1 ]; then
                  if [ $j -eq 1 ]; then
                    echo "server ok..."
                    sleep 0.1
                  fi
                fi
		sleep 0.01
	done
done
