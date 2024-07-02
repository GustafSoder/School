#!/bin/bash
for i in {1..100}
do
    { time ./mdu -j$i /pkg/ ; 
    } |& grep -Eo "[0-9]+\.[0-9]+" | tee -a outputfile.txt
done