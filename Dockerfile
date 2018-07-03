# Dockerfile configuration for a GNU Octave instance.

FROM ubuntu:18.04

RUN add-apt-repository ppa:octave/stable && apt-get update && apt-get install -y octave liboctave-dev
RUN cd project-3-barcode-scanner/
RUN octave --no-gui setup.m

CMD bash