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
    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0][0].doc_id, 1);
}

TEST(SearchServerTest, Search_EmptyQueries) {
    InvertedIndex index;
    index.UpdateDocumentBase({ "This is the first document.", "This is the second document." });
    SearchServer searchServer(index);
    std::vector<std::string> queries;
    auto results = searchServer.search(queries);
    ASSERT_TRUE(results.empty());
}


