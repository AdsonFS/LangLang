<> Objetivo da demo 1

var name -> string := "";
>> "Enter your name: ";
<< name;

var count -> number := 0;
>> "Enter the number of elements: ";
<< count;

var index -> number := 0;
var sum -> number := 0;
>> "Enter the elements: ";
while (index < count) {
  var value -> number := 0;
  << value;
  sum := sum + value;
  index := index + 1;
}

>> "Hello," >> name + "!";
>> "The sum is: " >> sum;
>> "The average is:" >> sum / count;
