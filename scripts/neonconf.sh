#!/bin/bash -ev

mkdir -p ~/.neon
echo "rpcuser=username" >>~/.neon/neon.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.neon/neon.conf

