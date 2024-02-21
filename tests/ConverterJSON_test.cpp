#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(ConverterJSON, readConfig) {
    ConverterJSON cj;
    std::vector<std::string> documents = cj.GetTextDocuments();
    // Check if the number of documents is correct
    int expectedDocuments = 3; // Change this to the number of documents in your config.json
    ASSERT_EQ(documents.size(), expectedDocuments);

    // Check if the documents match the expected values
    std::vector<std::string> expectedDocs = {"doc1.txt", "doc2.txt", "doc3.txt"}; // Change this to the expected document names
    for (int i = 0; i < expectedDocuments; i++) {
        ASSERT_EQ(documents[i], expectedDocs[i]);
    }
}

TEST(ConverterJSON, readRequests) {
    ConverterJSON converter;
    std::vector<std::string> requests = converter.GetRequests();
    // Add your expectations here, for example:
    EXPECT_FALSE(requests.empty());
    EXPECT_EQ(requests.size(), 3); // Or any other expected size.
}

TEST(ConverterJSON, getResponsesLimit) {
    ConverterJSON converter;
    int limit = converter.GetResponsesLimit();
    // Add your expectations here, for example:
    EXPECT_GT(limit, 0);
    EXPECT_LE(limit, 100); // Or any other expected range.
}

TEST(ConverterJSON, putAnswersTest) {
    // Create an instance of the ConverterJSON class
    ConverterJSON converter;
    // Create a sample vector of answers
    std::vector<std::vector<std::pair<int,float>>> sampleAnswers ={
        {{1, 0.9}, {2, 0.8}},
        {{3, 0.7}, {4, 0.6}},
        {} }; // An empty answer
    // Call the putAnswers method with the sample answers
    ASSERT_NO_THROW(converter.putAnswers(sampleAnswers));
}
