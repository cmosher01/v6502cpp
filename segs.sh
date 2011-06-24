#!/bin/sh
sed -f segs.sed | sort -u -g -k 1
