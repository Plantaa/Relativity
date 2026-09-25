FROM debian

WORKDIR /app

RUN apt update && apt upgrade -y

COPY dependencies.txt dependencies.txt
RUN apt install $(cat dependencies.txt) -y

COPY CMakeLists.txt CMakeLists.txt
COPY assets assets
COPY include include
COPY src src

RUN cmake -S . -B build
RUN cmake --build build

CMD [ "/app/build/ReinoDeEmma" ]