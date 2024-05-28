#!/bin/bash
TMP_AVI=$(mktemp /tmp/outXXXXXXXXXX.avi)
ffcast -s ffmpeg -y -r 20 -vcodec huffyuv $TMP_AVI \
&& convert -set delay 10 -layers Optimize $TMP_AVI out.gif
