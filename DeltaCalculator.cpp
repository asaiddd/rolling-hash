#include "DeltaCalculator.h"

#include <string.h>

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
    return oldHash - firstChar + nextChar;
}

void DeltaCalculator::lastChunk()
{
    if (chunks2.size == chunks1.size)
    {
        for (int i = chunks1.startIdx; i < chunks1.size; ++i)
        {
            chunks1.hash = rollHash(chunks1.hash, chunks1.data[i], 0);
            chunks2.hash = rollHash(chunks2.hash, chunks2.data[i], 0);
            if (chunks1.hash != chunks2.hash)
            {
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

// ret: additions, changes, removals
std::vector<std::vector<uint>> DeltaCalculator::calculateDelta()
{
    initHashes();

    if (chunks1.size > chunks2.size)
    {
        handleRemovals(chunks2.size+1);
    }
    else if (chunks1.size < chunks2.size)
    {
        handleAdditions(chunks1.size);
    }

    while(!chunks1.lastChunk && !chunks2.lastChunk)
    {
        if (chunks1.hash != chunks2.hash)
        {
            changes.push_back(chunks1.startIdx);
        }

        iterate();
    }

    return {std::move(additions), std::move(changes), std::move(removals)};
}

void DeltaCalculator::handleAdditions(long startIdx)
{
    for (long i = startIdx; i < chunks2.size; ++i)
    {
        additions.push_back(i);
    }
}

void DeltaCalculator::handleRemovals(long startIdx)
{
    for (long i = startIdx; i <= chunks1.size; ++i)
    {
        removals.push_back(i);
    }
}

std::vector<char *> DeltaCalculator::getChunks()
{
    return {chunks1.data, chunks2.data};
}
