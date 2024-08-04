# LangLang

## How run tests
```bash
g++ -I../googletest/googletest/include -I../googletest/googletest \
    -L../googletest/build/lib -lgtest -lgtest_main -pthread \
    tests/test_main.cpp -o test_main.out
./test_main.out
```

