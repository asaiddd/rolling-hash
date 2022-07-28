#include "DeltaCalculator.h"

#include <tuple>
#include <iostream> // todo:remove

DeltaCalculator::DeltaCalculator() {}

DeltaCalculator::~DeltaCalculator() {}

DeltaCalculator::DeltaCalculator(unsigned char *data1, unsigned char *data2, const short chunkSize)
{
    this->chunkSize = chunkSize;

    this->chunks1.data = data1;
    this->chunks1.hash = 0;
    this->chunks1.size = sizeof(data1);
    this->chunks1.chunkCount = chunks1.size / chunkSize;

    if (chunks1.chunkCount < 2)
    {
        fprintf(stderr, "At least 2 chunk needed: file1\n");
        exit(EXIT_SUCCESS);
    }

    this->chunks2.data = data2;
    this->chunks2.hash = 0;
    this->chunks2.size = sizeof(data2);
    this->chunks2.chunkCount = chunks2.size / chunkSize;
    ;

    if (chunks1.chunkCount < 2)
    {
        fprintf(stderr, "At least 2 chunk needed: file2\n");
        exit(EXIT_SUCCESS);
    }

    this->chunks1.startIdx = this->chunks2.startIdx = 0;
    this->chunks1.endIdx = this->chunks2.endIdx = chunkSize - 1;
    this->chunks1.finished = this->chunks2.finished = false;

    std::cout << chunks1.data << chunks2.data << std::endl;
}

void DeltaCalculator::initHashes()
{
    for (short i = 0; i < chunkSize; ++i)
    {
        std::cout << "old hash: " << chunks1.hash << " element: " << chunks1.data[i] << ":" << (int)chunks1.data[i];
        chunks1.hash += chunks1.data[i];
        std::cout << " | new hash: " << chunks1.hash << std::endl;

        std::cout << "old hash: " << chunks2.hash << " element: " << chunks1.data[i] << ":" << (int)chunks2.data[i];
        chunks2.hash += chunks2.data[i];
        std::cout << " | new hash: " << chunks2.hash << std::endl;
    }
}

// zero means iteration is end
int DeltaCalculator::iterate()
{
    chunks1.startIdx++;
    chunks1.endIdx++;
    chunks1.finished = (chunks1.endIdx >= chunks1.size);
    if (chunks1.finished == true)
    {
        std::cout << "chunks1.finished" << std::endl;
        handleAdditions();
        return 0;
    }
    chunks1.hash = chunks1.hash - chunks1.data[chunks1.startIdx] + chunks1.data[chunks1.endIdx];

    chunks2.startIdx++;
    chunks2.endIdx++;
    chunks2.finished = (chunks2.endIdx >= chunks2.size);
    if (chunks2.finished == true)
    {
        std::cout << "chunks2 finished" << std::endl;
        handleRemovals();
        return 0;
    }
    chunks2.hash = chunks2.hash - chunks2.data[chunks2.startIdx] + chunks2.data[chunks2.endIdx];

    return 1;
}

std::tuple<std::vector<uint>, std::vector<uint>, std::vector<uint>> DeltaCalculator::calculateDelta()
{
    std::cout << "calculating delta" << std::endl;
    initHashes();

    for (long i = chunks1.endIdx; i < chunks1.size; ++i)
    {
        std::cout << "loopCnt: " << i << std::endl;
        if (chunks1.hash != chunks2.hash)
        {
            changes.push_back(chunks1.startIdx);
        }

        if (iterate() == 0)
        {
            break;
        }
    }

    return std::make_tuple(std::move(additions), std::move(changes), std::move(removals));
}

void DeltaCalculator::handleAdditions()
{
    for (long i = chunks1.endIdx; i < chunks2.size; ++i)
    {
        additions.push_back(chunks2.data[i]);
    }
}

void DeltaCalculator::handleRemovals()
{
    for (long i = chunks1.endIdx; i < chunks1.size; ++i)
    {
        removals.push_back(chunks1.data[i]);
    }
}
