<> Funcoes alinhadas
if (1 < 2 && (4 > 0 - - (-5)) == 1) {
  >> "apenas testando if";
}


func  f1() -> {
  var content -> string := "escopo 1";
  >> "f1: " >> content;

  func f2() -> {
    var content -> string := "escopo 2";
    func f3() -> {
      content := "alterado em f3";
      >> "f3: " >> content;
    }


    >> "f2: " >> content;
    f3();
    >> "f2: " >> content;
  }
  

  f2();
  >> "f1: " >> content;
}

f1();
