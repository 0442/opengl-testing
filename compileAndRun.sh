#!/bin/sh
iName="main.cpp"
oName="glbin"
g++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl $iName -o $oName
./$oName
