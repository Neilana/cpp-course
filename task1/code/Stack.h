/// @file Stack.h

// include guard 
#pragma once

// headers
#include <iostream>
#include <fstream>
#include <limits>
#include <exception>        // exception
#include <new>  
#include <algorithm>        // swap

// FYI: Что касается POISON values, я бы их оставил только в режиме DEBUG.

/* STACK: CLASS INTERFACE */

/**
*   @class Stack Stack.h "Stack.h"
*   @brief Stack class.
*
*   My implementation of the stack structure.
*/
namespace MyNamespace
{
    template <class T>
    class Stack
    {
    public:
        // types definition
        // FYI: Мне кажется, это уже излишне:
        //    1) объем памяти, занимаемой стеком >> sizeof(size_t)
        //    2) размер стека (capacity) не может быть больше, чем size_t, если мы конечно не биты в нем храним.
        typedef size_t size_type;   ///< type of indexes used inside the array 
        
        // constructors and destructor
        Stack(size_type capacity = DEFAULT_CAPACITY);   ///< constructor
        Stack(const Stack<T> &obj);                     ///< copy constructor
        ~Stack();                                       ///< destructor
        
        // main functions for stack
        void push (const T&  value);   ///< add value to the stack
        void pop();                    ///< remove value from the stack
        T& top();                      ///< access the top element     
        bool empty() const;            ///< check if there's elements in the stack

        // operators overload
        Stack<T>& operator=(const Stack<T> &obj2);    ///< assignment operator overload

        // getters
        // FYI: Здесь можно оставить просто size_type
        Stack<T>::size_type capacity() const { return m_capacity; };
        Stack<T>::size_type size() const { return m_id; } ; ///< returns current number of elemetns in the stack
        Stack<T>::size_type id() const{ return m_id; };     ///< returns id of the stack


    private:
        // static class members
        static const size_type DEFAULT_CAPACITY = 6;///< is used in constructor when capacity wasn't defined
        static const size_type INCREMENT_CAPACITY = 6;  ///< add this number to the current capacity when stack is full
        static const T POISON_VALUE;                ///< instead of deleted elements
        static const std::string DUMP_FILE_NAME;    ///< name of the file where debug information is stored
        // FYI: В чем смысл этой переменной?! Если посмотреть, сколько стеков создается разными неявными вызовами конструкторов,
        // то логично в деструкторе уменьшать это число и убедится, что все сходится к 0.
        static int stacksCount;                     ///< total amount of created stacks 

        // information about the stack
        size_type m_size;       ///< current number of elements in the stack
        size_type m_capacity;   ///< maximum numbers of elements in the stack
        size_type m_id;         ///< id of the stack 
        
        T *m_data;     ///< array of stack values 
        
        // check and debug functions
        bool ok() const;                                    ///< check if the stack is valid
        void dump(const std::string &message = "" ) const;  ///< write debug info in the file
    };

    // inline functions implementation
/*    template <class T>
    inline Stack<T>::size_type Stack<T>::capacity() const { return m_capacity; }

    template <class T>
    inline Stack<T>::size_type Stack<T>::id() const { return m_id; }

    template <class T>
    inline Stack<T>::size_type Stack<T>::size() const { return m_size; }*/
}


/* STACK: TEMPLATE FUNCTIONS IMPLEMENTATION */

using MyNamespace::Stack;
using namespace std;

// initialize static members
template <class T>
const string Stack<T>::DUMP_FILE_NAME = "dumpFile.txt";

template <class T>
const T Stack<T>::POISON_VALUE = 666; // FIXME: Плохое число. Оно вполне может встретиться в стеке и в нормальной ситуации.

template <class T>
int Stack<T>::stacksCount = 0;

/**
*   Creates empty stack with given capacity. If capacity wasn't privided then capacity
*   is equal to Stack::DEFAULT_CAPACITY. Also assigns new id (Stack::m_id) which is 
*   equal to the number of already existing stacks.
*
*   @param capacity - number of elements in the stack.
*/
template <class T>
Stack<T>::Stack(Stack::size_type capacity) : m_size(0), m_capacity(capacity),
                 m_id(stacksCount), m_data(NULL)
{
    // FIXME: Почему не DUMP_FILE_NAME?
    // FIXME: Мне кажется не логичным фиксировать имя файла с дампом. А если несколько программ
    // параллельно захочется запустить?! Обычно в таких случаях генерят имя файла, содержащее
    // текущие дату/время и какую-нибудь случайную последовательность. + ко всему, дампить всегда -
    // плохое решение. Это отнимает время. Дампить надо только в дебаг режиме. Поэтому dump лучше делать
    // макросом, печатающим только тогда, когда мы явно задали константу DEBUG при компиляции программы.
    // Впоследствии это все хорошо бы заменить логгером с несколькими приоритетами сообщений.
    if (stacksCount == 0)
        remove("dumpFile.txt");
    string message = "Creating stack...";   
    try 
    {
        // FIXME: Раз уж у тебя есть POISON VALUE - я бы посоветовал заполнить им весь буффер изначально.
        // Иначе непонятно, нужен ли такой POISON_VALUE, который обозначает выделенную память, не содержащую
        // разумных значений, но не всегда.
        m_data = new T[DEFAULT_CAPACITY];
        stacksCount++;

        message += " Stack created!\n"; 
    } catch (exception& e)
    {                    
        string error = e.what();
        message += " Hey, sth is wrong! Exception caught: " + error; 
    }
    dump(message);
}

/**
*   Creates new stack as a copy of other stack.
*   
*   @param obj - a reference to a stack that is being used to initialize new stack.  
*/
template <class T>
Stack<T>::Stack(const Stack &obj)
{
    stacksCount++;
    // FYI: Есть такое хорошее predefined значение __FUNCTION__. Тогда можно подставлять прототип функции,
    // в которой ты находишься. Еще есть __LINE__. Из них хорошо бы генерировать сообщение об ошибке в
    // макросе dump.
    string message = "Creating stack from copy constructor (copy stack #" + 
                     to_string( obj.id() ) + ")... ";

    m_id = stacksCount;
    m_size = obj.m_size;
    m_capacity = obj.m_capacity;
    try
    {
        m_data = new T [m_size];

        // FYI: Есть такая клевая функция, memcpy - лучше использовать ее.
        for (size_type i = 0; i < m_size; i++)
            m_data[i] = obj.m_data[i];

        message += "Success!\n";
    } catch (exception &e)
    {
        // FIXME: Такого рода исключения лучше кидать наружу. А то ты фактически замалчиваешь проблему.
        string error = e.what();
        message = "We've got some problems! Exception caught: " + error + ".\n";
    }
    dump(message);
}

/**
*   Destroyes current stack. Frees allocated memory for stack elements. Writes result
*   messages in the dump file.
*/
template <class T>
Stack<T>::~Stack()
{
    dump("Freeing memory...\n");
    if (m_data != NULL) 
        delete [] m_data;
}

/**
*   Adds new value. In case of some problems writes messages in the dump file. Writes 
*   error messages in the dump file.
*   
*   @param value - value to be added in the stack.
*/
template <class T>
void Stack<T>::push(const T& value)
{
    if  (m_size >= m_capacity)
    {
        Stack <T> bufStack(*this);
        if (m_data) 
            delete [] m_data;
        m_capacity += INCREMENT_CAPACITY;
        try
        {
            m_data = new T [m_capacity];
            for (size_type i = 0; i < bufStack.m_size; i++)
                m_data[i] = bufStack.m_data[i];
            
            m_data[m_size++] = value;
        } catch (exception &e)
        {
            string error = e.what();
            string message = "Trying to push()... Exception caught: " + error + ".\n";
            dump(message);
        }
    }
    else
        m_data[m_size++] = value;
}

/**
*   Removes the top value from the stack. Writes error messages in the dump
*   file.
*/
template <class T>
void Stack<T>::pop()
{
    if (!empty())
        m_data[m_size--] = POISON_VALUE;
    else
        dump("Trying pop()... Stack is empty!\n");
}

/**
*   Returns reference on the top value. Writes error messages in the dump file.
*
*   @return @c result - the reference on the top element.
*/
template <class T>
T& Stack<T>::top()
{
    T *result = &m_data[0];     // we always have address of the 0th element

    if (!empty())
        result = &m_data[m_size-1];
    else
       *result = POISON_VALUE, dump("Trying top()... Stack is empty!\n");

    return *result; 
}

/**
*   Checks whether there's any elements in the stack.
*
*   @return @c true - if stack is empty (no elements);
*   @return @c false - if stack is not empty.
*/
template <class T>
bool Stack<T>::empty() const
{
    return (m_size == 0);
}

/**
*   Check whether stack is valid or not.
*
*   @return @c true - stack is valid;
*   @return @c false - stack is bad :(
*/
template <class T>
bool Stack<T>::ok() const
{
    // FIXME: А как насчет проверить, что m_data не NULL?! 
    return (m_size <= m_capacity);
}

/**
*   Writes debug messages to the file. File name is determined by the 
*   Stack::DUMP_FILE_NAME variable. 
*   
*   @param message - message to write in the file.
*/
template <class T>
void Stack<T>::dump(const string &message) const
{
    // FYI: Лучше держать файл с логами всегда открытым и писать туда когда нужно.

    // open dump file and write main info abot the stack
    ofstream dumpFile(DUMP_FILE_NAME, std::ios_base::app);
    dumpFile << "Stack #" << m_id << endl;
    dumpFile << "(Size: " << m_size << ", capacity: " << m_capacity << ")\n";

    // write elements of the stack into a string
    string elements = "";
    if (m_size > 0) 
        for (size_type i = 0; i < m_size; i++)
            elements += "[" +to_string(i) + "] = " + to_string(m_data[i]) + "\n";

    // write elements and message to the file and close it
    dumpFile << elements << message << endl;
    dumpFile.close();
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T> &obj2)
{
    // copy-swap
    // Магия! ОНО РЕАЛЬНО РАБОТАЕТ!!!
    // FIXME: Хм. Я Бы никогда не стал так определять оператор =, потому что он портит объект obj2.
    // Когда человек пишет оператор =, он не ожидает того, что объект obj2 будет изменен. А ты его портишь.
    // К тому же, это не скомпилируется, так как obj2 - const Stack&.
    swap(m_size, obj2.m_size);
    swap(m_capacity, obj2.capacity);
    swap(m_data, obj2.m_data);
    
    return *this;
}
