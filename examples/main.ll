var cnt -> number := 0;
var age -> number;
var name -> string;

name := "John";

func printname() -> {
  >> "Name:" >> name;
  return;
}

printname();
func fat() -> number {
  cnt := cnt + 1;
  if (cnt > 5) {
    return 1;
  }
  return cnt * fat();
}

for (cnt := 1024; cnt > 0; cnt := cnt / 2) {
  >> "Valor de cnt:" >> cnt;
}

>> fat();
