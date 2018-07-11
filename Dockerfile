# Dockerfile configuration for a GNU Octave instance.

FROM ubuntu:18.04

RUN apt-get update && apt-get install -y software-properties-common
RUN apt-get install -y octave liboctave-dev
RUN echo "pkg install -forge control\npkg install -forge signal" > setup.m && octave --no-gui setup.m

CMD bash
