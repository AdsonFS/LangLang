class address {
  var street -> string := "123 Main St";
  var city -> string := "Springfield";
  var state -> string := "IL";
  var zip -> string := "62704";
}

class person {
  var age -> number := 12 + 4;
  var name -> string := "John";
  var addr -> address := address();

  func print() -> void {
    >> "Hello, I am" >> name;
  }
}

class student : person {
  var code -> number := 35;

  <> override do metodo print
  func print() -> void {
    >> "Hi, I am a student, my name is" >> name;
  }
}

var p -> person := person();
var s -> student := student();

p.print();
s.print();

