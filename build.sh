#!/bin/bash
# Build TrafficSignRecognition

rm -R build; mkdir build
cd build; cmake .. && cmake --build .
 
