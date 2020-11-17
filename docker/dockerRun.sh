#!/bin/bash
docker build . -t stemnic/efm32gg-data
docker run -it --privileged -v $(pwd):/work stemnic/efm32gg-data:latest
