var cnt -> number;
var count -> number := 2;
var name -> string := "John";

func print() -> {
  >> "Hello World!";
  cnt := count := 0;
  >> "Hello World!";
  while(cnt < 10) {
    cnt := cnt + 1;
    >> "Name:" >> name;
  }
}

print();
