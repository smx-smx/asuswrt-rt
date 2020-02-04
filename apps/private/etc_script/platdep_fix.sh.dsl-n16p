#!/bin/sh

#add more switch buffer to cache the burst of the WiFi side
ethphxcmd gsww2 0 1fe0 200d4008

#share QDMA IRQ to CPU 0 & 2
echo 5 > /proc/irq/23/smp_affinity
