# Use the official Debian 11 "bullseye" image.
# Explicitly specify the platform for compatibility on Apple Silicon Macs.
FROM --platform=linux/amd64 debian:bullseye

# Set environment variables to non-interactive
ENV DEBIAN_FRONTEND=noninteractive

# Install sudo and other tools
RUN apt-get update && apt-get install -y \
    sudo \
    curl \
    git \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Create a 'student' user with a specific UID and GID (1000 is common)
# This also adds the user to the 'sudo' group and allows passwordless sudo.
ARG USERNAME=student
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo $USERNAME ALL=\(ALL\) NOPASSWD:ALL >> /etc/sudoers \
    && usermod -aG sudo $USERNAME

# Set the working directory for the user
WORKDIR /home/$USERNAME

# Switch to the new user for all subsequent commands
USER $USERNAME

# Default command when the container starts
CMD ["/bin/bash"]