#!/bin/bash

echo -n "clean .pyc file..."
find . -name "*.pyc" | xargs rm -rf
echo "done"

echo -n "clean temp xml file..."
rm -rf *.xml
echo "done"