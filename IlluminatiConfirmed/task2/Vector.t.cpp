#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "Vector.h"

using IlluminatiConfirmed::Vector;

TEST(VectorTest, CheckDefaultConstructor)
{
    // test 1
    Vector <int> v1;
    ASSERT_EQ(v1.capacity(), 0);

    // test 2
    Vector <int> v2(0);
    ASSERT_EQ(v2.capacity(), 0);

    // test 3
    Vector <int> v3(10);
    ASSERT_EQ(v3.capacity(), 10);

    // test 4
    Vector <int> v4(-10);
    ASSERT_EQ(v4.capacity(), 0);

    // test 5
    Vector <int> v5(1000000000000000);
    ASSERT_EQ(v5.capacity(), 0);
}

TEST(VectorTest, CheckAccessWithBrackets)
{
    // test 1
    Vector <int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);

    ASSERT_EQ(v1[0], 1);

    // check write
    v1[0] = 10;
    ASSERT_EQ(v1[0], 10);

    ASSERT_EQ(v1[4], 5);

    ASSERT_ANY_THROW(v1[10]);

    ASSERT_ANY_THROW(v1[-10]);

    ASSERT_ANY_THROW(v1[10000000000000]);

    Vector <int> v2;
    ASSERT_ANY_THROW(v2[2]);    // empty

    Vector <int> v3(5);
    v3[0] = 100;
    ASSERT_EQ(v3[0], 100);
}

TEST(VectorTest, CheckAccessWithAt)
{
    Vector <int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);

    // low threshold
    ASSERT_EQ(v1.at(0), 1);    // check read

    v1[0] = 10;
    ASSERT_EQ(v1.at(0), 10);   // check write

    // high thersshold
    ASSERT_EQ(v1.at(4), 5);    // read

    v1[4] = 50;
    ASSERT_EQ(v1.at(4), 50);   // write

    // out of range: too high
    ASSERT_ANY_THROW(v1.at(10));

    // out of range: too low
    ASSERT_ANY_THROW(v1.at(-10));

    // out of range: wrong high number
    ASSERT_ANY_THROW(v1.at(100000000000000));

    Vector <int> v2;
    ASSERT_ANY_THROW(v2.at(2));
}

TEST(VectorTest, CheckSize)
{
    Vector <int> v1;

    v1.push_back(1);
    ASSERT_EQ(v1.size(), 1);

    v1.push_back(10);
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1.capacity(), 5);

    v1.push_back(10);
    v1.push_back(10);
    v1.push_back(10);
    ASSERT_EQ(v1.capacity(), 5);

    v1.push_back(10);
    ASSERT_EQ(v1.capacity(), 10);
    ASSERT_EQ(v1.size(), 6);

    v1.reserve(20);
    ASSERT_EQ(v1.size(), 6);
    ASSERT_EQ(v1.capacity(), 20);

    v1.clear();
    ASSERT_EQ(v1.capacity(), 20);
    ASSERT_EQ(v1.size(), 0);
}

TEST(VectorTest, CheckMaxSize)
{
    // test 1
    std::vector <std::string> v1(5);
    size_t expectedSize = v1.max_size();

    Vector <std::string> vStd1(5); //FIXME: wtf??  Neilana: ну надоже max_size как-то проверить...
    size_t actualSize =  v1.max_size();

    ASSERT_EQ(expectedSize, actualSize);

    // test 2
    std::vector <std::string> vStd2(10);
    expectedSize = vStd2.max_size();

    Vector <std::string> v2(10);
    actualSize =  v2.max_size();

    ASSERT_EQ(expectedSize, actualSize);
}

TEST(VectorTest, CheckOperatorAssignment)
{
    Vector<double> v1(10);
    for (size_t i = 0; i<10; i++)
    {

        v1[i] = i*10;
    }
    Vector<double> v2(10);
    for (size_t i = 0; i<10; i++)
    {
        v2[i] = i*1000;
    }

    v2 = v1;

    for (size_t i = 0; i<10; i++)
    {
        ASSERT_EQ(v1.at(i), i*10);
        ASSERT_EQ(v2.at(i), i*10);
    }
}

TEST(VectorTest, CheckConstructorDefaultValue)
{
    Vector<double> v1(10,10.1);
    for (size_t i = 0; i < 10; i++)
    {
        ASSERT_EQ(v1.at(i), 10.1);
    }

    Vector <int> v2(10);
    for (size_t i = 0; i < 10; i++)
    {
        ASSERT_EQ(v2.at(i), 0);
    }

    Vector <std::string> v3(10);
    for (size_t i = 0; i < 10; i++)
    {
        ASSERT_EQ(v3.at(i), "");
    }
}

TEST(VectorTest, CheckAccessAssignmentOpConst)
{
    const Vector<double> v1(10);
    //v1[1] = 0; //It does not compile if all is ok
}

TEST(VectorTest, CheckEqualOperator)
{
    const Vector<double> v1(10, 10.2);
    const Vector<double> v2(10, 10.2);

    ASSERT_TRUE(v1 == v2);

    Vector<double> v3(10, 10.2);

    v3[9] = 0;

    ASSERT_FALSE(v3 == v1);
}

TEST(VectorTest, CheckReserve)
{
    Vector<double> v1(2);
    size_t expectedCapacity = 2;
    size_t actualCapacity = v1.capacity();
    ASSERT_EQ(actualCapacity, expectedCapacity);

    v1.reserve(5);
    expectedCapacity = 5;
    actualCapacity = v1.capacity();
    ASSERT_EQ(actualCapacity, expectedCapacity);
}

TEST (VectorTest, CheckSwap)
{
    Vector <double> v1(10);
    for (size_t i = 0; i<10; i++)
        v1[i] = i*10;

    Vector <double> v2(5);
    for (size_t i = 0; i<5; i++)
        v2[i] = i*1000;

    v1.swap(v2);

    for (size_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(v1[i], i*1000);
        ASSERT_EQ(v2[i], i*10);
    }
}

TEST (VectorTest, CheckResize)
{
    // test 1
    Vector <double> v1(10);
    for (size_t i = 0; i<10; i++)
        v1[i] = i*10;

    v1.resize(5);
    for (size_t i = 0; i < 5; i++)
        ASSERT_EQ(v1[i], i*10);

    // test 2
    Vector <double> v2(5);
    for (size_t i = 0; i<5; i++)
        v2[i] = i*1000;

    v2.resize(10);
    for (size_t i = 0; i < 5; i++)
        ASSERT_EQ(v2[i], i*1000);

    for (size_t i = 5; i < 10; i++)
        ASSERT_EQ(v2[i], 0);
}

TEST (VectorTest, CheckAggregateInitialization)
{
    // test 1
    Vector<double> v1 = {10,20,30,40,50,60};

    for (size_t i = 0; i < v1.capacity(); i++)
        ASSERT_EQ(v1.at(i), (i+1)*10);

    // test 2
    Vector<double> v2 {10,20,30,40,50,60};

    for (size_t i = 0; i < v2.capacity(); i++)
        ASSERT_EQ(v2.at(i), (i+1)*10);

    // test 3
    Vector<double> v3 ({10,20,30,40,50,60});

    for (size_t i = 0; i < v3.capacity(); i++)
        ASSERT_EQ(v3.at(i), (i+1)*10);
}

TEST (VectorTest, CheckNewElement)
{
    Vector <int> *v1 = new (0) Vector <int>;
    ASSERT_EQ(v1->size(), 0);

    Vector <int> *v2 = new (0) Vector <int> (3);        // i don't know who might use that form and why... but it works
    ASSERT_EQ(v2->size(), 3);
}

TEST (VectorTest, CheckNewArray)
{
    Vector <int> *v3 = new Vector <int> [5];        // creates an array of 5 vectors
    for (int vectorN = 0; vectorN < 5; vectorN++)
    {    for (int elementN = 0; elementN < 10; elementN++)
            (v3 + vectorN)->push_back(vectorN * elementN);
        std::cout << v3 << "\n";
    }

    for (int vectorN = 0; vectorN < 5; vectorN++)
        for (int elementN = 0; elementN < 10; elementN++)
        {
            int expectedValue = vectorN * elementN;
            int actualValue = (v3 + vectorN)->at(elementN);
            ASSERT_EQ(actualValue, expectedValue);
        }
}

TEST (VectorIteratorsTest, CheckIterators) //эммм..))
{
    //--, ++, +, -
    const Vector<double> v0 = {10,20,30,40,50,60};

    IlluminatiConfirmed::Vector<double>::const_iterator it0 = v0.begin();

    ASSERT_EQ(v0.at(3) , (*(it0+3)));
    ASSERT_EQ(v0.at(0),(*(it0++)));
    ASSERT_EQ(v0.at(1), (*(it0++)));
    ASSERT_EQ(v0.at(1), (*(it0-1)));
    ASSERT_EQ(v0.at(1), (*(--it0)));
    ASSERT_EQ(v0.at(1), (*(it0--)));

    //begin, end
    Vector<double> v1 = {10,20,30,40,50,60};
    size_t i = 0;
    ASSERT_EQ(6,v1.size());
    for (IlluminatiConfirmed::Vector<double>::iterator it = v1.begin(); it != v1.end(); it++)
    {
        ASSERT_EQ((*it), ((i++)+1)*10);
    }

    const Vector<double> v2 = {10,20,30,40,50,60};

    ASSERT_EQ(6,v2.size());

    //begin, end, ++: check const
    i = 0;
    for (IlluminatiConfirmed::Vector<double>::const_iterator it = v2.begin(); it != v2.end(); it++)
    {
        //*it = 10; //It does not compile if all is ok
        ASSERT_EQ((*it), ((i++)+1)*10);
    }

    //[]
    const Vector<double> v3 = {10,20,30,40,50,60};

    IlluminatiConfirmed::Vector<double>::const_iterator it3 = v3.begin();

    ASSERT_EQ(it3[5], 60);

    //==, !=
    Vector<double> v4 = {10,20,30,40,50,60};
    Vector<double> v5 = {10,20,30,40,50,60};
    IlluminatiConfirmed::Vector<double>::iterator it4 = v4.begin();
    IlluminatiConfirmed::Vector<double>::iterator it5 = v5.begin();
    ASSERT_TRUE(it4 != it5);

    ASSERT_FALSE(it4 == it5);

    //>
    Vector<double> v6 = {10,20,30,40,50,60};
    IlluminatiConfirmed::Vector<double>::iterator it6 = v6.begin();
    IlluminatiConfirmed::Vector<double>::iterator it7 = v6.begin();
    it7++;
    ASSERT_TRUE(it6 < it7);

    ASSERT_TRUE(it7 > it6); // And so on .. this is utopia
}

TEST (BaseContainerTest, CheckAbstract) //эммм..))
{
    //IlluminatiConfirmed::ContainerInterface<double> s; //It does not compile if all is ok
}
