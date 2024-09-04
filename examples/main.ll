var cnt -> number;
var age -> number;
var name -> string;

func print() -> {
  >> "Hello World!";
}

for (var i -> number := 0; i < 10; i := i + 1) {
  >> "Valor de i:" >> i;
}

for (cnt := 1024; cnt > 0; cnt := cnt / 2) {
  >> "Valor de cnt:" >> cnt;
}

print();
