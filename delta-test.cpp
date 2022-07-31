#include "DeltaCalculator.h"
#include "FileIO.h"
#include <assert.h>
#include <iostream>
#include <string.h>
#include <string>

void freeData(char *c1, char *c2)
{
    free(c1);
    c1 = 0;
    free(c2);
    c2 = 0;
}

void printVector(std::vector<uint> vec, std::string vecName)
{
    std::cout << vecName << std::endl;
    for (auto elem: vec)
    {
        std::cout << elem << std::endl;
    }
    std::cout << "##################" << std::endl;
}

int main(void)
{
    // file operation tests
    char *s1 = FileIO::readFile("/home/saidoff/playground/rolling-hash/input/digits.txt");
    char *s2 = FileIO::readFile("/home/saidoff/playground/rolling-hash/input/name.txt");

    std::string str1{s1};
    std::string str2{s2};

    assert(str1 == "1234567890");
    assert(str2 == "ahmet sait tekkurt");
    std::cout << "File has been read succesfully" << std::endl;

    // Delta Calculator constructor test
    DeltaCalculator d1(s1,s2);
    auto vec = d1.getChunks();
    std::string str3{(char *)vec[0]};
    std::string str4{(char*)vec[1]};

    assert(str3 == "1234567890");
    assert(str4 == "ahmet sait tekkurt");
    std::cout << "DeltaCalculator with given string is constructed: success "<< std::endl;

    char *data1 = 0, *data2 = 0;

    // same input
    data1 = strdup("helloworld");
    data2 = strdup("helloworld");
    DeltaCalculator d4(data1, data2);
    std::vector<std::vector<uint>> delta = d4.calculateDelta();
    assert(delta[0].empty());
    assert(delta[1].empty());
    assert(delta[2].empty());
    freeData(data1, data2);
    std::cout << "All equal case is success" << std::endl;

    data1 = strdup("helloworlda");
    data2 = strdup("helloworld");
    DeltaCalculator d5(data1, data2);
    std::vector<std::vector<uint>> delta2 = d5.calculateDelta();
    assert(delta2[0].empty());
    assert(delta2[1].empty());
    assert(delta2[2].size() == 1);
    freeData(data1,data2);
    std::cout << "Remove 1 char case is success" << std::endl;


    // removal of 7
    data1 = strdup("helloworlda123457");
    data2 = strdup("helloworld");
    DeltaCalculator d6(data1, data2);
    std::vector<std::vector<uint>> delta3 = d6.calculateDelta();
    assert(delta3[0].empty());
    assert(delta3[1].empty());
    assert(delta3[2].size() == 7);
    freeData(data1, data2);
    std::cout << "Remove 7 char case is success" << std::endl;


    // 1 change and 3 removal
    data1 = strdup("hell@world123");
    data2 = strdup("helloworld");
    DeltaCalculator d7(data1, data2);
    std::vector<std::vector<uint>> delta4 = d7.calculateDelta();
    // printVector(delta4[1], "changes");
    // printVector(delta4[2], "removals");
    assert(delta4[0].empty());
    std::vector<uint> expected = {1,2,3,4};
    assert(delta4[1] == expected);
    assert(delta4[2].size() == 3);
    freeData(data1, data2);
    std::cout << "1 change and 3 removal case is success" << std::endl;

    // addition of one
    data1 = strdup("helloworld");
    data2 = strdup("helloworld1");
    DeltaCalculator d8(data1, data2);
    std::vector<std::vector<uint>> delta5 = d8.calculateDelta();
    assert(delta5[0].size() == 1);
    assert(delta5[1].empty());
    assert(delta5[2].empty());
    freeData(data1,data2);
    std::cout << "Addition of one charactes case is success" << std::endl;

    // addition of 3
    data1 = strdup("helloworld");
    data2 = strdup("helloworld123");
    DeltaCalculator d9(data1, data2);
    std::vector<std::vector<uint>> delta6 = d9.calculateDelta();
    assert(delta6[0].size() == 3);
    assert(delta6[1].empty());
    assert(delta6[2].empty());
    freeData(data1,data2);
    std::cout << "Addition of 3 charactes case is success" << std::endl;

    // addition of 3 change 1
    data1 = strdup("helloworld");
    data2 = strdup("hellow@rld123");
    DeltaCalculator d10(data1, data2);
    std::vector<std::vector<uint>> delta7 = d10.calculateDelta();
    assert(delta7[0].size() == 3);
    expected.clear();
    expected = {3,4,5,6};
    assert(delta7[1] == expected );
    assert(delta7[2].empty());
    freeData(data1,data2);
    std::cout << "Addition of 3, change of one 1 case is success" << std::endl;

    // Chunk count check : expecting a stderr msg
    data1 = strdup("sh");
    data2 = strdup("longdata");
    DeltaCalculator d2(data2, data1); // toggle these 2 to see both chunk size checks
    DeltaCalculator d3(data1, data2);
    free(data1);
    free(data2);

    return 1;
}