<> This is a comment
<> This is another comment

var cnt -> number;
if (2 && (2 != 2 > 1)) 
{
  var hello -> string  := "Hello";
  >> "True";
}
var name -> string  := "Adson" + " " + "Santos";
>> "Hello," >> name;

cnt := 10;
while (cnt > 0) {
  >> cnt;
  cnt := cnt - 2;
}

if ((name < "ZZZ") && 1 < 2) {
  >> "True";
  var name -> string  := "<empty>";
  >> "IF: " >> name;
}

func func1 () -> {
  var age -> number  := 12 + 8;

  func func2 () ->  {
    var age -> number  := 1;
    var name -> string := "ss";
    >> age >> name;
  }
  func2();
  >> "FUNC 1 ->" >>name>> age;
}

func func2 () ->{
  var name -> string  := "Santos";
  >> (2 + 2) >> name >> ("Hello: " + (name));
  name := "Sr. " + name;
  >> "FUNC2:">>name;
  func1();
}
func2();
func1();
>>name;
