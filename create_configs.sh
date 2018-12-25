#!/bin/bash
rm -rf Debug
rm -rf Release
mkdir Debug
mkdir Release
( cd Debug && cmake ../ )
( cd Release && cmake  ../ )
