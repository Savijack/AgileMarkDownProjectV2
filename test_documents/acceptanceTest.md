# Assignment 1: Acceptance Test

## Overview
This document outlines the acceptance test for the project. The purpose of this test is to ensure that the developed system meets the specified requirements and functions as intended. I am will use this file to demonstrate that the system behaves correctly under various scenarios.

--- 

### Lorem Ipsum Section
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin pretium dui ac purus tincidunt suscipit. Maecenas congue porta lectus in bibendum. Morbi nec lacus dolor. Etiam porttitor libero vel ornare maximus. Nunc viverra libero in rutrum pellentesque. Duis orci erat, varius mattis turpis ut, rhoncus pretium turpis. Etiam mattis leo nec leo semper lacinia.

Integer at ex at erat aliquam rutrum. Sed facilisis lorem sed pulvinar dapibus. Suspendisse commodo feugiat purus, vel tincidunt dolor euismod vel. Nam nec ipsum ac ex auctor pellentesque. Vivamus vel odio dui. Etiam aliquet velit eget justo tempus suscipit sed et sem. Sed non orci turpis. Proin et eros porta, elementum eros at, facilisis eros. Nulla eu porttitor nisl. Donec sit amet vehicula odio, at lacinia lacus. Curabitur eget magna auctor, cursus quam semper, tempor neque. Maecenas rutrum consequat pretium. Morbi fringilla ex eu nunc rhoncus, quis placerat tortor luctus.

Aliquam sed nisi ultrices, faucibus tortor vel, blandit purus. Integer dignissim velit sit amet purus convallis, fringilla facilisis augue tristique. Nulla facilisi. Nulla at consequat odio. Donec odio mi, consectetur mattis condimentum ac, suscipit nec purus. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Praesent tempor fermentum accumsan. In tristique tincidunt est, a tincidunt sapien sollicitudin sit amet. Nam hendrerit commodo diam eget gravida. Donec vestibulum sodales purus scelerisque malesuada. Praesent ornare enim eu euismod tincidunt. Aliquam viverra nibh pulvinar mauris vulputate, quis commodo felis efficitur.

## Some Emphasized Text
Here is some text with **bold** and *italic* formatting to demonstrate emphasis in the document. Here is a simple code snippet `int x;` to illustrate inline code formatting. You can find my GitHub account here: [markm208](https://github.com/markm208) and a picture of me here: ![Me](https://avatars.githubusercontent.com/u/934916?v=4)

## Code Block Example
Below is an example of a code block demonstrating a simple function in C/C++. I'll discuss:
1. printing to the console
2. flow of control
3. the main function

``` file="main.cpp" 
#include <iostream>
using namespace std;

int main()
{
    cout << "Hello World!!!!" << endl;

    return 0;
}
```

In order to print to the console window one needs to include the iostream library and use the standard namespace. 

``` file="main.cpp" highlight="1,2"
#include <iostream>
using namespace std;

int main()
{
    cout << "Hello World!!!!" << endl;

    return 0;
}
```

The main function is the entry point of the program, and it returns an integer value upon completion.

``` file="main.cpp" highlight="4-9"
#include <iostream>
using namespace std;

int [<main()>]
{
    cout << "Hello World!!!!" << endl;

    return 0;
}
```

`cout` is used to print text to the console. The `<<` operator is used to send the string "Hello World!!!!" followed by an newline to the output stream.

``` file="main.cpp" highlight="6"
#include <iostream>
using namespace std;

int main()
{
    [<cout << "Hello World!!!!" << endl;>]

    return 0;
}
``` 

`main` returns 0 to indicate that the program has completed successfully.

``` file="main.cpp" highlight="8"
#include <iostream>
using namespace std;

int main()
{
    cout << "Hello World!!!!" << endl;

    [<return 0;>]
}
```

The output of the following program will be:
```program-output
Hello World!!!!
```

If I repeated the `cout` a few times like this:

``` file="main.cpp" highlight="6-8"
#include <iostream>
using namespace std;

int main()
{
    [<cout>] << "Hello World!!!!" << [<endl;>]
    [<cout>] << "Hello World!!!!" << [<endl;>]
    [<cout>] << "Hello World!!!!" << [<endl;>]

    return 0;
}
``` 

Then the output would be:

```program-output
Hello World!!!!
Hello World!!!!
Hello World!!!!
```

### Status Table
:::table cols="[*Feature*][Not Started][Doing][Done]"
[Basic Markdown][✓][][]
[Extended Code Markdown][✗][✓][]
[Table Markdown][✗][✗][✓]
:::
