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
    answers.push_back({ {1, 0.5f}, {2, 0.8f} });
    converter.putAnswers(answers);
    json answersJsonFile;
    std::filesystem::path basePath = std::filesystem::current_path() / "config";
    std::filesystem::path answersJsonPathF = basePath / "answers.json";
    std::string answersJsonPath = answersJsonPathF.string();
    std::ifstream ifs(answersJsonPath);
    ifs >> answersJsonFile;
    ASSERT_TRUE(answersJsonFile.contains("answers"));
    auto answersJson = answersJsonFile["answers"];
    ASSERT_TRUE(answersJson.is_object());
    int count = 0;
    for (const auto& answer : answersJson) {
        ASSERT_TRUE(answer.is_object());
        ASSERT_TRUE(answer.contains("result"));
        ASSERT_TRUE(answer["result"].is_string());
        ASSERT_TRUE(answer.contains("relevance"));
        ASSERT_TRUE(answer["relevance"].is_array());
        for (const auto& relevance : answer["relevance"]) {
            ASSERT_TRUE(relevance.is_object());
            ASSERT_TRUE(relevance.contains("docid"));
            ASSERT_TRUE(relevance["docid"].is_number_integer());
            ASSERT_TRUE(relevance.contains("rank"));
            ASSERT_TRUE(relevance["rank"].is_number_float());
            count++;
        }
    }
    ASSERT_EQ(count, 2);
}