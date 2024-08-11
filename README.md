# LangLang

## How run the interpreter
```bash
g++ -g -std=c++20 $(find src/ -type f -name "*.cpp") -o langlang.out
./langlang.out <langlang_file>

## How run tests
```bash
g++ -I../googletest/googletest/include -I../googletest/googletest \
    -L../googletest/build/lib -lgtest -lgtest_main -pthread \
    $(find tests/ -type f -name "*.cpp") $(find src/*/ -type f -name "*.cpp") -o run_tests.out && ./run_tests.out
./run_tests.out
```

