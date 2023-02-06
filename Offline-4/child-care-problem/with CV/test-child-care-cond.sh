#!/bin/bash

gcc -o child-care child-care-condition-variable.c -lpthread
./child-care $1 $2
