FROM gcc:10.2

RUN apt-get update
RUN apt-get -y install git curl libncurses5 lib32z1 libusb-0.1 libelf1 vim

RUN mkdir -p /tools
WORKDIR /tools

RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz \
    && tar xvf gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz \
    && rm gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz

WORKDIR /work

ENV PATH $PATH:/tools/gcc-arm-none-eabi-9-2020-q2-update/bin