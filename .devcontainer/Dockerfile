# Use the base image
FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

# Install dependencies
RUN apt-get update && apt-get install -y build-essential cmake git python3

# Set the working directory to /workspaces
WORKDIR /workspaces

# Install Emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git emsdk && \
    cd emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest && \
    echo "source /workspaces/emsdk/emsdk_env.sh" >> ~/.bashrc && \
    chown -R vscode:vscode /workspaces

# Use bash for the following commands
SHELL ["/bin/bash", "-c"]

# Ensure Emscripten environment variables are set
ENV PATH="/workspaces/emsdk:/workspaces/emsdk/node/12.18.1_64bit/bin:/workspaces/emsdk/upstream/emscripten:${PATH}"
ENV EMSDK="/workspaces/emsdk"
ENV EM_CONFIG="/workspaces/emsdk/.emscripten"
ENV EM_CACHE="/workspaces/emsdk/.emscripten_cache"

# Clone and build Raylib for Web
RUN git clone https://github.com/raysan5/raylib.git raylib && \
    cd raylib/src && \
    source /workspaces/emsdk/emsdk_env.sh && \
    make PLATFORM=PLATFORM_WEB -j$(nproc)

# Source .bashrc to ensure emsdk is available in all terminal instances
RUN echo "source ~/.bashrc" >> /root/.bashrc