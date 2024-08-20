STRING name = "Adson";
>> "Hello:" >> name;

FUNC func1 {
  NUMBER age = 12 + 8;

  FUNC func2 {
    NUMBER age = 1;
    STRING name= "ss";
    >> age >> name;
  }
  func2;
  >> "FUNC 1 ->" >>name>> age;
}
FUNC func2{
  STRING name = "Santos";
  >> (2 + 2) >> name >> ("Hello: " + (name));
  name = "Sr. " + name;
  >> name;
}
func1;
func1;
func1;
func2;
func1;
func2;
