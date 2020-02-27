from ubuntu:18.04

# Install prerequisites
run apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    curl \
    git \
    libcurl3-dev \
    libleptonica-dev \
    liblog4cplus-dev \
    libopencv-dev \
    libtesseract-dev \
    wget \
    python3 \
    python3-dev

RUN mkdir /home/openalpr
workdir /home/openalpr
RUN git clone https://github.com/openalpr/openalpr/

# Copy all data
# copy openalpr /srv/openalpr

# Setup the build directory
run mkdir /home/openalpr/openalpr/src/build
workdir /home/openalpr/openalpr/src/build

# Setup the compile environment
run cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_INSTALL_SYSCONFDIR:PATH=/etc .. && \
    make -j2 && \
    make install

run python3 /home/openalpr/openalpr/src/bindings/python/setup.py install

copy . /home/dashcam

RUN apt-get update && apt-get install -y openssh-server
RUN mkdir /var/run/sshd
RUN echo 'root:password123' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

EXPOSE 5678
CMD ["/usr/sbin/sshd", "-D"]

workdir /data

# entrypoint ["alpr"]
