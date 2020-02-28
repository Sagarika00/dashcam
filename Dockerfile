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
    python2.7 \
    python2.7-dev \
    python3 \
    python3-dev -y

run apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x \
    gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 \
    gstreamer1.0-pulseaudio -y

run wget https://bootstrap.pypa.io/get-pip.py

run python3 get-pip.py

run apt-get install python3-gi python3-gi-cairo python3-cairo \
    python3-cairo-dev gir1.2-gtk-3.0 build-essential \
    python-gobject \
    python-gobject-2-dev \
    python-gi-cairo python-gi-dev \
    python-gi \
    cmake unzip pkg-config libjpeg-dev libpng-dev libtiff-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev \
    libgtk-3-dev \
    software-properties-common \
    libatlas-base-dev gfortran -y

# install MPI
run apt-get install libcr-dev mpich python3-mpi4py -y
run add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"

# run pip install --user mpi4py PyGObject pycairo gTTS opencv-python numpy

# run mkdir /home/mpi4py
# workdir /home/mpi4py

# run curl -Lso - https://bitbucket.org/mpi4py/mpi4py/downloads/mpi4py-3.0.3.tar.gz | tar -zxf - > /dev/null
# run ls
# workdir /home/mpi4py/mpi4py-3.0.3
# run python3 setup.py install --user

run pip install --user gTTS opencv-python numpy mpi4py

# run mkdir /home/opencv

# workdir /home/opencv

# run wget -O opencv.zip https://github.com/opencv/opencv/archive/3.4.4.zip

# run mkdir /home/opencv-contrib

# workdir /home/opencv-contrib

# run wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/3.4.4.zip

run apt-get install libgirepository1.0-dev gcc libcairo2-dev pkg-config python3-dev gir1.2-gtk-3.0 -y

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
