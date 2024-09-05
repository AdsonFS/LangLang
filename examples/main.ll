var cnt -> number := 0;
var age -> number;
var name -> string;

func print() -> {
  >> "Hello World!";
  cnt := cnt + 1;
  if (cnt > 5) {
    return; 
  }
  print();
}

for (var i -> number := 0; i < 10; i := i + 1) {
  >> "Valor de i:" >> i;
}

for (cnt := 1024; cnt > 0; cnt := cnt / 2) {
  >> "Valor de cnt:" >> cnt;
}

print();
