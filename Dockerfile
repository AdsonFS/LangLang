FROM gcc:latest AS builder

WORKDIR /app
COPY . .
RUN g++ -std=c++20 $(find src/ -type f -name "*.cpp") -o langlang.out
# CMD ["./lang++"]
ENTRYPOINT ["tail", "-f", "/dev/null"]

