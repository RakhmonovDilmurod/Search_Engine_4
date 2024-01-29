#include "../include/InvertedIndex.h"
#include "gtest/gtest.h"

TEST(InvertedIndex, updateDocumentBase) {
    InvertedIndex index;

    std::vector<std::string> docs = {
        "This is document one",
        "This document is the second",
        "The third document is this"
    };

    index.UpdateDocumentBase(docs);

    // Check if the documents have been stored correctly
    EXPECT_EQ(docs.size(), index.docs.size());
    for (size_t i = 0; i < docs.size(); ++i) {
        EXPECT_EQ(docs[i], index.docs[i]);
    }

    // Check if the frequency dictionary has been built correctly
    auto freq_dict = index.getFreqDictionary();

    // Check the frequency dictionary for the word "This"
    auto it = freq_dict.find("This");
    ASSERT_NE(it, freq_dict.end());
    EXPECT_EQ(it->second.size(), 3);
    EXPECT_TRUE(it->second[0].doc_id == 0 || it->second[0].doc_id == 1 || it->second[0].doc_id == 2);
    EXPECT_TRUE(it->second[1].doc_id == 0 || it->second[1].doc_id == 1 || it->second[1].doc_id == 2);
    EXPECT_TRUE(it->second[2].doc_id == 0 || it->second[2].doc_id == 1 || it->second[2].doc_id == 2);

    // Check the frequency dictionary for the word "document"
    it = freq_dict.find("document");
    ASSERT_NE(it, freq_dict.end());
    EXPECT_EQ(it->second.size(), 3);
    EXPECT_TRUE(it->second[0].doc_id == 0 || it->second[0].doc_id == 1 || it->second[0].doc_id == 2);
    EXPECT_TRUE(it->second[1].doc_id == 1 || it->second[1].doc_id == 2);
    EXPECT_TRUE(it->second[2].doc_id == 2);

    // Check the frequency dictionary for the word "is"
    it = freq_dict.find("is");
    ASSERT_NE(it, freq_dict.end());
    EXPECT_EQ(it->second.size(), 3);
    EXPECT_TRUE(it->second[0].doc_id == 0 || it->second[0].doc_id == 1 || it->second[0].doc_id == 2);
    EXPECT_TRUE(it->second[1].doc_id == 0 || it->second[1].doc_id == 1 || it->second[1].doc_id == 2);
    EXPECT_TRUE(it->second[2].doc_id == 0 || it->second[2].doc_id == 1 || it->second[2].doc_id == 2);

    // Check the frequency dictionary for the word "the"
    it = freq_dict.find("the");
    ASSERT_NE(it, freq_dict.end());
    EXPECT_EQ(it->second.size(), 3);
    EXPECT_TRUE(it->second[0].doc_id == 1 || it->second[0].doc_id == 2);
    EXPECT_TRUE(it->second[1].doc_id == 1 || it->second[1].doc_id == 2);
    EXPECT_TRUE(it->second[2].doc_id == 2);
}

TEST(InvertedIndex,getWordCount){
    InvertedIndex index;

    std::vector<std::string> docs = {
        "This is document one",
        "This document is the second",
        "The third document is this"
    };

    index.UpdateDocumentBase(docs);

    // Check the word count for the word "This"
    auto word_counts = index.GetWordCount("This");
    ASSERT_EQ(word_counts.size(), 3);
    EXPECT_EQ(word_counts[0].doc_id, 0);
    EXPECT_EQ(word_counts[0].count, 1);
    EXPECT_EQ(word_counts[1].doc_id, 1);
    EXPECT_EQ(word_counts[1].count, 1);
    EXPECT_EQ(word_counts[2].doc_id, 2);
    EXPECT_EQ(word_counts[2].count, 1);

    // Check the word count for the word "document"
    word_counts = index.GetWordCount("document");
    ASSERT_EQ(word_counts.size(), 3);
    EXPECT_EQ(word_counts[0].doc_id, 0);
    EXPECT_EQ(word_counts[0].count, 1);
    EXPECT_EQ(word_counts[1].doc_id, 1);
    EXPECT_EQ(word_counts[1].count, 1);
    EXPECT_EQ(word_counts[2].doc_id, 2);
    EXPECT_EQ(word_counts[2].count, 1);

    // Check the word count for a non-existing word
    word_counts = index.GetWordCount("non_existing_word");
    EXPECT_TRUE(word_counts.empty());
}