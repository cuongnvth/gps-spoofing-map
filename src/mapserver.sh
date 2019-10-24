#!/bin/sh
# RUN MAP SERVER

cd ../mapserver/
sudo pm2 start process.yml
