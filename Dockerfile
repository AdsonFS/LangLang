FROM gcc:latest AS builder

WORKDIR /app
COPY . .
RUN g++ -std=c++20 -o lang++ src/main.cpp src/*/*.cpp

# CMD ["./lang++"]
ENTRYPOINT ["tail", "-f", "/dev/null"]

