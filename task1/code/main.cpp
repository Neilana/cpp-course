/**
*   @file       main.cpp
*   @briefi     Task #1: Stack 
*   @details    Implements stack structure. 
*   @author     Alina Voropaeva
*   @date       2017
*/  

// headers
#include<iostream>

// my headers
#include "MyStack.h"

// simple unit-test
#define MY_TEST( condition )  \
    if ( ! (condition) )    \
    cout << "Test failed: " << #condition << endl;  

using namespace std;

int main()
{
    MyStack stack1(6);
    stack1.top();
    
    MyStack stack2(-10);
    stack2.pop();
    
//  cout << stack1.top()++;
//  MY_TEST(stack1.top() == 4); 
    return 0;

}
