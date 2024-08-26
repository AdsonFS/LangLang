<> This is a comment
<> This is another comment

IF (1 == 2 > 1) {
  >> "True";
}

STRING name = "Adson" + " " + "Santos";
<< name ;
>> "Hello," >> name;

NUMBER cnt = 10;
WHILE (cnt > 0) {
  >> cnt;
  cnt = cnt - 2;
}

IF ((name < "ZZZ") && 1 < 2) {
  >> "True";
  STRING name = "<empty>";
  >> "IF: " >> name;
}

FUNC func1 {
  NUMBER age = 12 + 8;

  FUNC func2 {
    NUMBER age = 1;
    STRING name= "ss";
    >> age >> name;
  }
  func2();
  >> "FUNC 1 ->" >>name>> age;
}

FUNC func2{
  STRING name = "Santos";
  >> (2 + 2) >> name >> ("Hello: " + (name));
  name = "Sr. " + name;
  >> "FUNC2:">>name;
  func1();
}
func2();
func1();
>>name;
