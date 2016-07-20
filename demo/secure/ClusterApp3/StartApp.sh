#!/bin/sh

export FB_MULTI_BUFFER=3
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

./GtfStartup $PWD

