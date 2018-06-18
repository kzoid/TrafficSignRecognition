#!/bin/bash
# Compile imglab

cd dlib/tools/imglab
rm -R build; mkdir build
cd build; cmake .. && cmake --build .


echo ""
echo "Create XML from sample images:"
echo "dlib/tools/imglab/build/imglab -c images/pare/training.xml images/pare/train/*.(jpg|jpeg|png)"
echo "dlib/tools/imglab/build/imglab images/pare/training.xml"
echo "dlib/tools/imglab/build/imglab -c images/pare/testing.xml images/pare/test/*.(jpg|jpeg|png)"
echo "dlib/tools/imglab/build/imglab images/pare/testing.xml"
echo ""
echo "Use shift+click to draw a box around signs."
echo ""
