#include "../include/SearchEngine.h"
#include "../include/InvertedIndex.h"
#include "gtest/gtest.h"

SearchServer server;
TEST(SearchServerTest, SearchTest) {
    std::vector<std::string> queries = {"test query 1", "test query 2"};
    std::vector<std::vector<RelativeIndex>> results = server.search(queries);
    EXPECT_EQ(results.size(), 2); 
    EXPECT_EQ(results[0].size(), 3); 
    EXPECT_EQ(results[0][0].doc_id, 0);
    EXPECT_FLOAT_EQ(results[0][0].rank, 0.333);
    EXPECT_EQ(results[1].size(), 3); 
    EXPECT_EQ(results[1][0].doc_id, 1);
    EXPECT_FLOAT_EQ(results[1][0].rank, 0.333);
  
}