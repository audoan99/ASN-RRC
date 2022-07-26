# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the GCC AND G++ compiler
RUN apt-get -y install gcc
RUN apt-get -y install g++

# Install the GNU MAKE
RUN apt-get -y install make

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /Documents/HCL/project1

# Specify the working directory
WORKDIR /Documents/HCL/project1

# Use Clang to compile the Test.cpp source file
RUN make build

# Run the output program from the previous step
CMD ["./main"]