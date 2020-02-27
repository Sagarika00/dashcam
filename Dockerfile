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

run wget https://bootstrap.pypa.io/get-pip.py

run python3 get-pip.py

pip3 install --user mpi4py PyGObject pycairo gTTS opencv-python numpy

run apt-get install python3-gi python3-gi-cairo gir1.2-gtk-3.0 build-essential \
    cmake unzip pkg-config libjpeg-dev libpng-dev libtiff-dev libjasper1 libjasper-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev \
    libgtk-3-dev \
    libatlas-base-dev gfortran


run add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"

# run mkdir /home/opencv

# workdir /home/opencv

# run wget -O opencv.zip https://github.com/opencv/opencv/archive/3.4.4.zip

# run mkdir /home/opencv-contrib

# workdir /home/opencv-contrib

# run wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/3.4.4.zip

run apt-get install libgirepository1.0-dev gcc libcairo2-dev pkg-config python3-dev gir1.2-gtk-3.0

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

RUN apt-get update && apt-get install -y openssh-server

COPY docker.pub /root/.ssh/authorized_keys

COPY user-config.jam /root
COPY project-config.jam /root

RUN mkdir /var/run/sshd

# RUN mkdir /home/boost

# workdir /home/boost

# RUN git clone https://github.com/boostorg/boost



copy . /home/dashcam
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes PasswordAuthentication yes PermitEmptyPasswords no/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

EXPOSE 5678
CMD ["/usr/sbin/sshd", "-D"]

workdir /data

# entrypoint ["alpr"]
