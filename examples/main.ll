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

class person {
  var age -> number := 12 + x;
  var name -> string := "John";

  func print() -> void {
    >> "Hello";
  }
}

var p -> person := person();
p.print();

var lout -> func -> void := p.print;
lout();
