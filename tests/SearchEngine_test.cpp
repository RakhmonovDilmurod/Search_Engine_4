#include "gtest/gtest.h"
#include "SearchEngine.h"
#include "InvertedIndex.h"

TEST(SearchServerTest, Search) {
    InvertedIndex index;
    index.UpdateDocumentBase({ "This is the first document.", "This is the second document." });
    SearchServer searchServer(index);
    std::vector<std::string> queries;
    queries.push_back("document first");
    auto results = searchServer.search(queries);
    ASSERT_TRUE(!results.empty());
    ASSERT_EQ(results[0].size(), 2);
    ASSERT_EQ(results[0][0].doc_id, 0);
    ASSERT_FLOAT_EQ(results[0][0].rank, 0.5);
    ASSERT_EQ(results[0][1].doc_id, 1);
    ASSERT_FLOAT_EQ(results[0][1].rank, 0.5);
}

TEST(SearchServerTest, Search_EmptyQueries) {
    InvertedIndex index;
    index.UpdateDocumentBase({ "This is the first document.", "This is the second document." });
    SearchServer searchServer(index);
    std::vector<std::string> queries;
    auto results = searchServer.search(queries);
    ASSERT_TRUE(results.empty());
}


