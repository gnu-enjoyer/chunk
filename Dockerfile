FROM gcc:12 AS compile-image

RUN useradd --system --create-home chunk

RUN apt-get update && apt-get install -y cmake

USER chunk

WORKDIR /home/chunk

COPY --chown=chunk . .

RUN cmake -B build && cmake --build ./build