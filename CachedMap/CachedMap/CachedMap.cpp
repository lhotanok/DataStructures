#include "cachedmap.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
    cached_map< int, string, external_storage> cms{ 100 };

    // Example usage of cached_map data structure

    cms.insert({ 0, "Task: Multiply the given numbers and store the result." });
    cms.insert({ 1, "Task: Create a two-dimensional array filled with integer values." });
    cms.insert_or_assign({ 0, "Task: Compute the sum of the first 100 prime numbers." });

    auto it = cms.find(1);
    cout << (*it).second << endl;

    cms.insert_or_assign({ 1, "Task: Represent the chessboard using the square matrix." });
    cms.insert({ 2, "Task: Enroll yourself in the online course of data science." });

    it = cms.find(1);
    auto stored = *it;
    ++it;

    auto end_it = cms.end();
    auto begin_it = cms.begin();

    for (auto iter = cms.begin(); iter != cms.end(); ++iter)
    {
        cout << (*iter).second << endl;
    }
}
