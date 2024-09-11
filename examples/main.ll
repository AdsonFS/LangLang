var a -> string := "global";

{
  func show() -> void {
    >> a;
  }
  show();
  var a -> string := "block";
  show();
}

var ccc -> number := 0;
func show() -> void {
  ccc := ccc + 1;
  var a -> string := "function";
  >> a;
  if (ccc < 2) {
    show();
  }
  >> a;
  a := "function2";
}
show();

func counter() -> func -> void {
  var i -> number := 0;
  func count() -> void {
    i := i + 1;
    >> i;
  }

  return count;
}

var cc -> func ->  void; 
cc := counter();
