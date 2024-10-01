FROM ubuntu:latest

RUN apt update -y && apt upgrade -y
RUN apt install -y build-essential git htop curl zlib1g zlib1g-dev zsh
RUN chsh -s $(which zsh)
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.2.1/zsh-in-docker.sh)"
RUN apt-get install -y libgl1-mesa-dev libx11-dev libxext-dev libxrandr-dev
RUN git clone https://github.com/ocen-lang/ocen && cd ocen && ./meta/bootstrap.sh
RUN echo 'export OCEN_ROOT=/ocen/' >> ~/.zshrc
RUN echo 'export PATH=$PATH:$OCEN_ROOT/bootstrap/' >> ~/.zshrc