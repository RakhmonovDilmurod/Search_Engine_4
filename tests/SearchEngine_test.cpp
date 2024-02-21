#include "../include/SearchEngine.h"
#include "../include/InvertedIndex.h"
#include "gtest/gtest.h"

TEST(SearchServer, search) {
    // Initialize an InvertedIndex with some documents
    InvertedIndex index;
    std::vector<std::string> docs = {
        "This is the first document",
        "This document is the second",
        "And this is the third one",
        "Is this the first document?"
    };
    index.UpdateDocumentBase(docs);

    // Initialize a SearchServer with the InvertedIndex
    SearchServer searchServer(index);

    // Define a vector of queries
    std::vector<std::string> queries = {
        "first document",
        "second document",
        "third document",
        "not existent"
    };

    // Call the search method and store the results
    std::vector<std::vector<RelativeIndex>> results = searchServer.search(queries);

    // Check the results
    std::vector<RelativeIndex> expected1 = {
        {0, 2},
        {3, 2}
    };
    EXPECT_EQ(results[0], expected1);

    std::vector<RelativeIndex> expected2 = {
        {1, 2}
    };
    EXPECT_EQ(results[1], expected2);

    std::vector<RelativeIndex> expected3 = {
        {2, 2}
    };
    EXPECT_EQ(results[2], expected3);

    std::vector<RelativeIndex> expected4 = {};
    EXPECT_EQ(results[3], expected4);
}
