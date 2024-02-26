#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(ConverterJSON, readConfig) {
    ConverterJSON cj;
    std::vector<std::string> documents = cj.GetTextDocuments();
    int expectedDocuments = 3; 
    ASSERT_EQ(documents.size(), expectedDocuments);
    std::vector<std::string> expectedDocs;
    expectedDocs.push_back("document1.txt");
    expectedDocs.push_back("document2.txt");
    expectedDocs.push_back("document3.txt");

    // Iterate through the documents and expectedDocs vectors
    for (size_t i = 0; i < documents.size(); ++i) {
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
