#include "gtest/gtest.h"
#include "InvertedIndex.h"

TEST(InvertedIndexTest, UpdateDocumentBase) {
    InvertedIndex index;
    std::vector<std::string> documents;
    documents.push_back("This is the first document.");
    documents.push_back("This is the second document.");
    index.UpdateDocumentBase(documents);
    ASSERT_TRUE(!documents.empty());
}

TEST(InvertedIndexTest, GetWordCount) {
    InvertedIndex index;
    std::vector<std::string> documents;
    documents.push_back("This is the first document.");
    documents.push_back("This is the second document.");
    index.UpdateDocumentBase(documents);
    std::vector<Entry> entries = index.GetWordCount("document");
    ASSERT_TRUE(!entries.empty());
   
}

TEST(InvertedIndexTest, GetWordCount_NotFound) {
    InvertedIndex index;
    std::vector<std::string> documents;
    documents.push_back("This is the first document.");
    documents.push_back("This is the second document.");
    index.UpdateDocumentBase(documents);
    std::vector<Entry> entries = index.GetWordCount("unknown");
    ASSERT_TRUE(entries.empty());
}