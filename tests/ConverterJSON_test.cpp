#include "gtest/gtest.h"
#include "ConverterJSON.h"

TEST(ConverterJSONTest, GetTextDocuments) {
    ConverterJSON converter;
    std::vector<std::string> documents = converter.GetTextDocuments();
    ASSERT_TRUE(!documents.empty());
}

TEST(ConverterJSONTest, GetResponsesLimit) {
    ConverterJSON converter;
    int limit = converter.GetResponsesLimit();
    ASSERT_TRUE(limit > 0);
}

TEST(ConverterJSONTest, GetRequests) {
    ConverterJSON converter;
    std::vector<std::string> requests = converter.GetRequests();
    ASSERT_TRUE(!requests.empty());
}

TEST(ConverterJSONTest, PutAnswers) {
    ConverterJSON converter;
    std::vector<std::vector<std::pair<int, float>>> answers;
    converter.putAnswers(answers);
    json answersJsonFile;
    std::filesystem::path basePath = std::filesystem::current_path() / "config";
    std::filesystem::path answersJsonPathF = basePath / "answers.json";
    std::string answersJsonPath = answersJsonPathF.string();
    std::ifstream ifs(answersJsonPath);
    ifs >> answersJsonFile;
    ASSERT_TRUE(answersJsonFile.contains("answers"));
}