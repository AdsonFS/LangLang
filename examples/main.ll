STRING name = "Adson";
>> "Hello:" >> name;
FUNC func1 {
  NUMBER age = 12 + 8;
  >> "FUNC 1 ->" >>name>> age;
}
FUNC func2{
  STRING name = "Santos";
  >> "FUNC 2 ->" >> name;
}
func1;
func1;
func1;
func2;
func1;
func2;
