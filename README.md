# LangLang
![LangLangLogo](./img/logo.png)

> "The progressive development of man is vitally dependent on invention."
> Nikola Tesla

## Get started with LangLang

#### How run the interpreter
After cloning this repository on your computer, run the prompt below to generate the interpreter build.
```bash
g++ -g -std=c++20 $(find src/ -type f -name "*.cpp") -o langlang.out
./langlang.out <langlang_file>
```
#### How run tests
```bash
g++ -I../googletest/googletest/include -I../googletest/googletest \
    -L../googletest/build/lib -lgtest -lgtest_main -pthread \
    $(find tests/ -type f -name "*.cpp") $(find src/*/ -type f -name "*.cpp") -o run_tests.out && ./run_tests.out
./run_tests.out
```

## 1. Introduction
LangLang is an object-oriented programming language with robust features such as higher-order functions, conditional structures, loops, class support, and inheritance. It is designed to be expressive and easy to use, with built-in input (`<<`) and output (`>>`) capabilities for direct terminal interaction.

#### Hello, World!
Copy and run the file below to have your first contact with LangLang
```
>> "Hello, World!";
```
Run with the command:
```bash
./langlang.out hello.ll
```

## 2. Basic Syntax
#### 2.1 Variable Declaration
In LangLang, variables are declared using `var`, followed by the variable name, its type, and an initial value:
```
var x -> number := 5;
var name -> string := "John";
```
-   `->`: Defines the variable type.
-   `:=`: Assigns an initial value.

#### 2.2 Data Types
LangLang supports the following data types:
-   `number`: Represents integers or floating-point numbers.
-   `string`: Represents character strings.
-   `func`: Represents functions.
-   `<identifier>`: User-defined classes can also be used as types.

#### 2.3 Comments
LangLang supports comments using the following syntax:
```
<> This is a comment
```

## 3. Conditional Structures
LangLang supports conditional structures using the `if-else` syntax:
```
if (condition) {
  // code block
} else {
  // alternative block
}
```
Example:
```
if (10%3 == 1) {
  >> "Yes";
} else {
  >> "No";
}
```

## 4. Loops
LangLang supports both `while` and `for` loops for repeating blocks of code.

#### 4.1 `while` Loop
The `while` loop repeats a block of code as long as a condition is true:
```
while (x > 0) { 
	>> x;
	x := x - 1;
}
```

#### 4.2 `for` Loop
The `for` loop in LangLang is used to iterate over a range of values. Its syntax is:
```
for (var i -> number := start_value; i < end_value; i := i + step_value) {
  // code block
}
```
-   `start_value`: Initial value of the loop variable.
-   `end_value`: Condition to terminate the loop.
-   `step_value`: Value by which the loop variable is incremented each iteration.

## 5. Functions
LangLang allows the creation of functions using the `func` keyword:
```
func function_name() -> return_type {
  // function body
  // return <value>
}
```
Example:
```
func counter() -> func -> void {
  var i -> number := 0;
  func count() -> void {
    i := i + 1;
    >> i;
  }
  return count;
}
```

#### 5.1 Higher-Order Functions
Functions can return other functions, allowing a functional programming style:
```
var cc -> func -> void := counter();
cc();
```

## 6. Classes and Objects
LangLang is an object-oriented language, supporting the creation of classes and instances:

#### 6.1 Class Definition
Classes are defined using the `class` keyword:
```
class person {
  var age -> number := 12;
  var name -> string := "John";

  func print() -> void {
    >> "Hello, I am" >> name;
  }
}
```

#### 6.2 Class Instance
To create an instance of a class:
```
var p -> person := person();
p.print();
```

### 6.3 Inheritance

LangLang supports inheritance between classes. A child class inherits the properties and methods of the parent class:
```
class student : person {
  var code -> number;
  
  func print() -> void {
    >> "Hi, I am a student, my name is" >> name;
  }
}
```

## 7. Input and Output
LangLang has built-in support for input with `<<` and output with `>>`:
```
>> "Enter your name:";
<< name;
```
