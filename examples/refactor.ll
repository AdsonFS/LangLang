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
cc := counter();

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
if (x > -12) {
  var y -> number := 10;
  dd := counter();
  cc();
  dd();
}
var lout -> func -> void := p.print;
lout();
