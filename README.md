# LangLang


## How run tests
```bash
g++ -I../googletest/googletest/include -I../googletest/googletest \
    -L../googletest/build/lib -lgtest -lgtest_main -pthread \
    tests/*/tests.cpp src/*/*.cpp -o run_tests.out
./run_tests.out
```

