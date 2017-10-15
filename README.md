# P-plus-plus
A C++ header that enables many Python-like features in C++

I made this spring of 2017 durning my CS 121 class after my teacher recommended that I share it. I thought I would post it on github for others to access. It hasn't been changed from its original post and the google drive links still work.


# Here is the original classroom post:

Just for fun over spring break I made a header file (Python.h) It allows some cool Python-like stuff to happen in C++
 
 
for example: (all of the below code works inside C++)


```C++
// strings and numbers can be stored in the same List 
// and you can have lists of lists 
List a_list = { 10 , 20 , 30 };
a_list.append("string");
a_list.append(a_list);    // a list can even append itself 

 
print "print works a lot like print in python 2.7\n", 
      "you can even use commas instead of << \n" ;
 
 
For(each)in(a_list)
    print each , '\n';

// (will print out)
// 10
// 20
// 30
// string
// List
//     10
//     20
//     30
//     string
 
 
var hello_str = "hello ";
print hello_str * 3; 
// (will print out)
// hello hello hello 
```

There’s a lot of other functionality I built in. I thought some of you guys might be interested in it. So here’s the header file and a demo program to go with it.
 
 
If you want to play with the header, you’ve actually already got access to it. Just add the below line to your program, run it inside of build.tamu.edu , and it should work. ( Note: when you compile it, it’s going to give you a whole bunch of warnings, but you can ignore them)
```C++
#include "/home/ugrads/j/jeff.hykin/Python/Python.h"
```

If you want you can view/download the files from here
demo     -  https://goo.gl/Zv8k0A 
Python.h - https://goo.gl/mjDDWz 
 
 
You can also look at the demo on build.tamu.edu with the command
```
cat /home/ugrads/j/jeff.hykin/Python/python_demo.cpp
```
and the Python.h file with the command
```
cat /home/ugrads/j/jeff.hykin/Python/Python.h
```
 
 
Let me know if you guys hit any bugs or have any questions;
