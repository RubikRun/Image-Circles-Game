#!/bin/bash
g++ -O3 -o game main.cpp Game.cpp Circle.cpp Image.cpp -I SFML-2.5.1/include -L SFML-2.5.1/lib -l sfml-graphics -l sfml-window -l sfml-system
