#include "../include/SearchEngine.h"
#include "../include/InvertedIndex.h"
#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"


TEST(SearchServerTest, SearchTest) {
    ConverterJSON conv;
    InvertedIndex index;
    index.UpdateDocumentBase(conv.GetTextDocuments());
    SearchServer server(index);
    std::vector<std::vector<RelativeIndex>> results = server.search(conv.GetRequests());
    for (const auto& result : results) {
        ASSERT_FALSE(result.empty());
    }
}