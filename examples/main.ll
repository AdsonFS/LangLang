var i -> number := 0;

func add() -> number {
  return 1+1;
}
var r -> number := 12;
r := 23;

func counter() -> func -> void {
  var i -> number := 0;
  func count() -> void {
    i := i + 1;
    >> i;
  }

  return count;
}
var cc -> func -> void;
var dd -> func -> func -> void;
counter();
dd := counter();
cc := counter();
dd := cc;
cc();
