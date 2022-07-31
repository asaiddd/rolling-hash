#include "DeltaCalculator.h"
#include "FileIO.h"
#include <assert.h>
#include <iostream>
#include <string.h>
#include <string>

int main(void)
{

    // file operation tests
    char *s1 = FileIO::readFile("digits.txt");
    char *s2 = FileIO::readFile("name.txt");

    std::string str1{s1};
    std::string str2{s2};

    assert(str1 == "1234567890");
    assert(str2 == "ahmet sait tekkurt");

    // Delta Calculator constructor test
    DeltaCalculator d1(s1,s2);
    auto vec = d1.getChunks();
    std::string str3{(char *)vec[0]};
    std::string str4{(char*)vec[1]};

    assert(str3 == "1234567890");
    assert(str4 == "ahmet sait tekkurt");

    char *data1, *data2;

    // same input
    data1 = strdup("helloworld");
    data2 = strdup("helloworld");
    DeltaCalculator d4(data1, data2);
    std::vector<std::vector<uint>> delta = d4.calculateDelta();
    assert(delta[0].empty());
    assert(delta[1].empty());
    assert(delta[2].empty());
    free(data1);
    free(data2);

    // Chunk count check
    data1 = strdup("sh");
    data2 = strdup("longdata");
    DeltaCalculator d2(data2, data1);
    DeltaCalculator d3(data1, data2);

    return 1;
}