>> "start: main.ll";

>> 12 / 3;

func counter() -> func -> void {
  var i -> number := 0;
  func count() -> void {
    i := i + 1;
    >> i;
  }

  return count;
}

var cc -> func -> void;
var dd -> func -> void;
counter();
cc := counter();
dd := cc;
cc();
cc();
cc();
dd();
dd();
dd();

var x -> number := 5;

class address {
  var street -> string := "123 Main St";
  var city -> string := "Springfield";
  var state -> string := "IL";
  var zip -> string := "62704";

  func print() -> void {
    >> street;
    >> city;
    >> state;
    >> zip;
  }
}

class person {
  var age -> number := 12 + x;
  var name -> string := "John";
  var addr -> address := address();

  func print() -> void {
    >> "Hello, I am" >> name;
  }
}

class student : person {
  var code -> number := 35;

  func print() -> void {
    >> "Hi, I am a student, my name is" >> name;
  }
}

func newperson() -> person {
  var p -> person := person();
  >> p.age;
  >> p.name;
  p.addr.print();
  return p;
}
var person1 -> person := newperson();

var p -> person := person();
p.addr.print();

var lout -> func -> void := p.print;
lout();


var s -> student := student();
s.print();
