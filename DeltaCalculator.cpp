#include "DeltaCalculator.h"

#include <tuple>
#include <string.h>
#include <iostream> // todo:remove

DeltaCalculator::DeltaCalculator() {}

DeltaCalculator::~DeltaCalculator() {}

DeltaCalculator::DeltaCalculator(char *data1, char *data2, const short chunkSize)
{
    this->chunkSize = chunkSize;

    this->chunks1.data = data1;
    this->chunks1.hash = 0;
    this->chunks1.size = strlen((char *)data1);
    this->chunks1.chunkCount = chunks1.size / chunkSize;

    if (chunks1.chunkCount < 2)
    {
        fprintf(stderr, "At least 2 chunk needed: file1\n");
        exit(EXIT_SUCCESS);
    }

    this->chunks2.data = data2;
    this->chunks2.hash = 0;
    this->chunks2.size = strlen((char *)data2);
    this->chunks2.chunkCount = chunks2.size / chunkSize;
    ;

    if (chunks2.chunkCount < 2)
    {
        fprintf(stderr, "At least 2 chunk needed: file2\n");
        exit(EXIT_SUCCESS);
    }

    this->chunks1.startIdx = this->chunks2.startIdx = 0;
    this->chunks1.endIdx = this->chunks2.endIdx = chunkSize - 1;
    this->chunks1.lastChunk = this->chunks2.lastChunk = false;
}

void DeltaCalculator::initHashes()
{
    for (short i = 0; i < chunkSize; ++i)
    {
        chunks1.hash += chunks1.data[i];
        chunks2.hash += chunks2.data[i];
    }
}

uint rollHash(uint oldHash, char firstChar, char nextChar)
{
    std::cout << "rollHash: removed: " << firstChar << " added: "  << nextChar << std::endl;
    return oldHash - firstChar + nextChar;
}

void DeltaCalculator::lastChunk()
{
    std::cout << "--------------------------------------\nlastChunk" << std::endl;
    std::cout << "chunks1: " << chunks1.data[chunks1.startIdx] << std::endl;
    std::cout << "chunks2: " << chunks2.data[chunks2.startIdx] << std::endl;

    if (chunks2.size == chunks1.size)
    {
        for (int i = chunks1.startIdx; i < chunks1.size; ++i)
        {
            std::cout <<" hash1: " << chunks1.hash << " hash2: " << chunks2.hash << std::endl;
            chunks1.hash = rollHash(chunks1.hash, chunks1.data[i], 0);
            chunks2.hash = rollHash(chunks2.hash, chunks2.data[i], 0);
            if (chunks1.hash != chunks2.hash)
            {
                std::cout << "Pushing: " << i << std::endl;
                changes.push_back(i);
            }
        }
    }
}

void DeltaCalculator::iterate()
{
    chunks1.startIdx++;
    chunks1.endIdx++;
    chunks2.startIdx++;
    chunks2.endIdx++;
    std::cout << "Iterating" << std::endl;
    chunks1.lastChunk = (chunks1.endIdx >= chunks1.size);
    if (chunks1.lastChunk == true)
    {
        lastChunk();
        return;
    }

    chunks1.hash = rollHash(chunks1.hash, chunks1.data[chunks1.startIdx-1], chunks1.data[chunks1.endIdx]);

    chunks2.lastChunk = (chunks2.endIdx >= chunks2.size);
    if (chunks2.lastChunk == true)
    {
        lastChunk();
        return;
    }
    
    chunks2.hash = rollHash(chunks2.hash, chunks2.data[chunks2.startIdx-1], chunks2.data[chunks2.endIdx]);
}

std::vector<std::vector<uint>> DeltaCalculator::calculateDelta()
{
    std::cout << "calculating delta" << std::endl;
    initHashes();

    if (chunks1.size > chunks2.size)
    {
        handleRemovals(chunks2.size);
    }
    else if (chunks1.size < chunks2.size)
    {
        handleAdditions(chunks1.size);
    }

    while(!chunks1.lastChunk || chunks2.lastChunk)
    {
        std::cout << "hash1: " << chunks1.hash << " hash2: " << chunks2.hash << std::endl;
        if (chunks1.hash != chunks2.hash)
        {
            std::cout << "adding" << chunks1.startIdx << std::endl;
            changes.push_back(chunks1.startIdx);
        }

        iterate();
        std::cout << "#################################" << std::endl;
    }

    return {std::move(additions), std::move(changes), std::move(removals)};
}

void DeltaCalculator::handleAdditions(long startIdx)
{
    std::cout << "additions, starting from: " << startIdx << " char: " << chunks2.data[startIdx] << std::endl;
    for (long i = startIdx; i < chunks2.size; ++i)
    {
        additions.push_back(i);
    }
}

void DeltaCalculator::handleRemovals(long startIdx)
{
    std::cout << "removals, starting from: " << startIdx << " char: " << chunks1.data[startIdx] << std::endl;
    for (long i = startIdx; i <= chunks1.size; ++i)
    {
        removals.push_back(i);
    }
}

std::vector<char *> DeltaCalculator::getChunks()
{
    return {chunks1.data, chunks2.data};
}
