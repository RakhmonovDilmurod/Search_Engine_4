#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
ConverterJSON converter;
TEST(ConverterJSONTest, GetTextDocumentsTest) {
    std::vector<std::string> documents = converter.GetTextDocuments();
    EXPECT_EQ(documents.size(), 3);
    EXPECT_EQ(documents[0], "test document 1"); 
    
}

TEST(ConverterJSONTest, GetResponsesLimitTest) {
    int limit = converter.GetResponsesLimit();
    EXPECT_EQ(limit, 5);
}

TEST(ConverterJSONTest, GetRequestsTest) {
    std::vector<std::string> requests = converter.GetRequests();
    EXPECT_EQ(requests.size(), 2); 
    EXPECT_EQ(requests[0], "request 1"); 
}

TEST(ConverterJSONTest, PutAnswersTest) {
    std::vector<std::vector<std::pair<int, float>>> answers;
    answers.push_back({{1, 0.5}, {2, 0.8}});
    answers.push_back({{3, 0.9}});
    converter.putAnswers(answers);
    std::ifstream answersFile("answers.json");
    nlohmann::json answersJson;
    answersFile >> answersJson;
    EXPECT_TRUE(answersJson.contains("answers"));
    EXPECT_EQ(answersJson["answers"].size(), 2);
    EXPECT_EQ(answersJson["answers"][0]["result"], "true");
    EXPECT_EQ(answersJson["answers"][0]["relevance"].size(), 2);
    EXPECT_EQ(answersJson["answers"][0]["relevance"][0]["docid"], 1);
    EXPECT_EQ(answersJson["answers"][0]["relevance"][0]["rank"], 0.5);
    EXPECT_EQ(answersJson["answers"][0]["relevance"][1]["docid"], 2);
    EXPECT_EQ(answersJson["answers"][0]["relevance"][1]["rank"], 0.8);
}