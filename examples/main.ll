var cnt -> number := 0;
var age -> number;
var name -> string := "John";
var print -> func -> void;

func printname() -> func -> void {
  >> "Name:" >> name;
  func pp() -> void {
    >> "Name:" >> name;
    return;
  }
  return pp;
}
printname();


