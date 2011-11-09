#!/bin/sh

if [ -z $1 ]
then
    echo "Usage: $0 map.tmx"
    exit 1
fi

WIDTH=`echo 'cat /map/layer/@width' | xmllint --shell $1 | grep "width=" | sed -e "s/.*width=\"//" | sed -e "s/\".*//"`
HEIGHT=`echo 'cat /map/layer/@height' | xmllint --shell $1 | grep "height=" | sed -e "s/.*height=\"//" | sed -e "s/\".*//"`

echo 'cat /map/layer/data/text()' | xmllint --shell $1 | grep "^  " | sed "s/ *//g" | base64 -d | gunzip - | ./rawmaptoc $WIDTH $HEIGHT




