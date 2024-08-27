<> This is a comment
<> This is another comment

if (2 && (2 != 2 > 1)) 
{
  string hello = "Hello";
  >> "True";
}
string name = "Adson" + " " + "Santos";
>> "Hello," >> name;

number cnt = 10;
while (cnt > 0) {
  >> cnt;
  cnt = cnt - 2;
}

if ((name < "ZZZ") && 1 < 2) {
  >> "True";
  string name = "<empty>";
  >> "IF: " >> name;
}

func func1 {
  number age = 12 + 8;

  func func2 {
    number age = 1;
    string name= "ss";
    >> age >> name;
  }
  func2();
  >> "FUNC 1 ->" >>name>> age;
}

func func2{
  string name = "Santos";
  >> (2 + 2) >> name >> ("Hello: " + (name));
  name = "Sr. " + name;
  >> "FUNC2:">>name;
  func1();
}
func2();
func1();
>>name;
