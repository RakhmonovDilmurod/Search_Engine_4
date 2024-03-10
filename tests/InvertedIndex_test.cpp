#include "../include/InvertedIndex.h"
#include "gtest/gtest.h"

InvertedIndex index;


TEST(InvertedIndexTest, UpdateDocumentBaseTest) {
    std::vector<std::string> documents = {"test document 1", "test document 2", "test document 3"};
    index.UpdateDocumentBase(documents);
    auto freq_dict = index.getFreqDictionary();
    EXPECT_EQ(freq_dict.size(), 18); 
 
}

TEST(InvertedIndexTest, GetWordCountTest) {
    std::vector<std::string> documents = {"test document 1", "test document 2", "test document 3"};
    index.UpdateDocumentBase(documents);
    auto freq_dict = index.getFreqDictionary();
    auto word_count = index.GetWordCount("test");
    EXPECT_EQ(word_count.size(), 3);
    EXPECT_EQ(word_count[0].doc_id, 0);
    EXPECT_EQ(word_count[0].count, 1);
    EXPECT_EQ(word_count[1].doc_id, 1);
    EXPECT_EQ(word_count[1].count, 1);
    EXPECT_EQ(word_count[2].doc_id, 2);
    EXPECT_EQ(word_count[2].count, 1);

}