// print_container.h
// -----------------
// This file contains a template function that prints the content
// of an arbitrary STL container. The container to be printed is
// used as a template argument.
//
// The function receives three arguments (the last two with
// default values)
//
// void print_container(const T& container, const char* name="container:",
//                      int cols=0);
//
// "container" is a reference to the container to be printed
//
// "name" is the C-string that will be printed before the container
//  contents, the default value es "container:"
//
//  "cols" is the number of columns. Default value is 0, in this case the
//  function ignores the columns and writes the elements in one line, 
//  separated by " ". Otherwise, it does the same but prints a newline
//  every count elements
//  ------------------------------------------------------------------

#include<iostream>

template<class T>
void print_container(const T& C, const char* name="container:",
                      int cols=0)
    {
    typename T::const_iterator pos;    // declares the type of pos
    int count=1;

    std::cout << name << '\n';
    if(C.empty())
        {
        std::cout << "Container is empty! " << std::endl;
        return;
        }

    for(pos=C.begin(); pos!=C.end(); ++pos)
        {
        std::cout << *pos;
        switch(cols)
           {
           case 0:  std::cout << ' ';
                    break;
           case 1:  std::cout << '\n';
                    break;
           default: if(count<cols)
                      {
                      std::cout << " ";
                      count++;
                      }
                    else
                      {
                      std::cout << '\n';
                      count=1;
                      }
                    break;
           }
        }
    if(count<cols) std::cout << "\n";
    std::cout <<  std::endl;
    }


 
