var a -> string := "global";
{
  func show() -> void {
    >> a;
  }
  show();
  var a -> string := "block";
  show();
}

func counter() -> func -> void {
  var i -> number := 0;
  func count() -> void {
    i := i + 1;
    >> i;
  }

  return count;
}

var cc -> func -> void; 
cc := counter();
cc();
cc();
cc();
