#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include "gtest/gtest.h"
#include <cstdlib>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>



using json = nlohmann::json;

class OpeningError : public std::exception {
private:
    std::string message;

public:
    OpeningError(const std::string  fileName)
        : message("Failed to open file: " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class JsonFileContainingError : public std::exception {
private:
    std::string message;

public:
    JsonFileContainingError(const std::string fileName, const std::string& containerName)
        : message("Failed to find container " + containerName + " in file " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};


class ConverterJSON {
private:

    const std::string configJsonPath   = "../config/config.json";
    const std::string requestsJsonPath =  "../config/requests.json";
    const std::string answersJsonPath  =  "../config/answers.json";
    json answersJsonFile;
    json requestsJsonFile;

public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> documents;
        std::ifstream configFile(configJsonPath,std::ios::in);

        if (!configFile.is_open()) {
            throw OpeningError(configJsonPath);
        }

        json config;
        configFile >> config;

        if (config.empty()) {
            throw OpeningError(requestsJsonPath);
        }

        for (const auto& doc : config["files"]) {
            documents.push_back(doc.get<std::string>());
        }

        return documents;
    }

    int GetResponsesLimit() {
        std::ifstream configFile(configJsonPath);
        json config;
        configFile >> config;

        return config.contains("max_responses") ? config["max_responses"].get<int>() : 0; // Default value or throw an exception.
    }

    std::vector<std::string> GetRequests() {
        std::ifstream ifSJsonFile(requestsJsonPath);
        ifSJsonFile >> requestsJsonFile;
        std::vector<std::string> requests = {};

        if (requestsJsonFile.contains("requests")) {
            for (const auto& i : requestsJsonFile["requests"]) {
                requests.push_back(i.get<std::string>());
            }

        return requests;
    }
        else{
            throw JsonFileContainingError(requestsJsonPath, "requests");
        }
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        for (int i = 0; i < answers.size(); i++) {
            std::string strI = "";
            for (int n = 0; n < 3 - std::to_string(i).length(); n++) {
                strI += "0";
            }
            strI += std::to_string(i + 1);

          
            if (answers[i].empty()){

                answersJsonFile["answers"]["request" + strI]["result"] = "false";

            }else{

                answersJsonFile["answers"]["request" + strI]["result"] = "true";
                for (int j = 0; j < answers[i].size(); j++) {
                    json::value_type block;
                    block["docid"] = answers[i][j].first;
                    block["rank"] = answers[i][j].second;
                    answersJsonFile["answers"]["request" + strI]["relevance"].push_back(block);
                }
            }
            std::ofstream ofstreamJsonFile(answersJsonPath);
            ofstreamJsonFile <<std::setw(4)<< answersJsonFile;
            ofstreamJsonFile.close();
        }
    }
};

struct Entry {
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};
class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs){
        docs = input_docs;
        freq_dictionary.clear();
        for (size_t doc_id = 0; doc_id < input_docs.size(); ++doc_id) {
            std::map<std::string, int> word_count;
            std::istringstream iss(input_docs[doc_id]);
            std::string word;
            while (iss >> word) {
                word_count[word]++;
            }
            for (const auto& p : word_count) {
                freq_dictionary[p.first].push_back({doc_id,size_t(p.second)});
            }
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word){
        std::vector<Entry> result;
        auto it = freq_dictionary.find(word);
        if (it != freq_dictionary.end()) {
            result = it->second;
        }
        return result;
    }
    auto getFreqDictionary() {
        return freq_dictionary;
    }
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};

struct RelativeIndex{
    size_t doc_id;
    Entry rank;
    bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer {
private:
    InvertedIndex index;
    ConverterJSON converter;
    std::map<std::string, std::vector<Entry>> freqDictionary;

public:

    SearchServer(InvertedIndex& idx) : index(idx){
    freqDictionary = idx.getFreqDictionary();
    }

    std::vector<std::string> requestsInput() {
    return converter.GetRequests();
    }

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){
    std::vector<std::vector<RelativeIndex>> search_results;
    for (const auto& query : queries_input) {
        std::vector<RelativeIndex> relative_indices;
        std::map<size_t, size_t> doc_ranks;
        std::istringstream iss(query);
        std::string word;
        while (iss >> word) {
            auto entries = _index.GetWordCount(word);
            for (const auto& entry : entries) {
                doc_ranks[entry.doc_id] += entry.count;
            }
        }
        for (const auto& p : doc_ranks) {
            relative_indices.push_back({p.first, p.second});
        }
        std::sort(relative_indices.begin(), relative_indices.end(),[](const RelativeIndex &a,const RelativeIndex &b)
        {
            return a.rank.count > b.rank.count;
        });
        search_results.push_back(relative_indices);
      }
    return search_results;
    }
private:
    InvertedIndex _index;
};


using namespace std;
void TestInvertedIndexFunctionality(
    const vector<string>& docs,
    const vector<string>& requests,
    const std::vector<vector<Entry>>& expected
    ) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
        {
            {0, 1}
        }, {
            {0, 1}, {1, 3}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 2}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
        {
        }, {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}


int main(int argc, char** argv) {
    ConverterJSON conv;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::vector<std::string> texts;
    std::vector<std::string> requests;
    InvertedIndex inv;
    inv.UpdateDocumentBase(conv.GetTextDocuments());

    try {
        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(searchSer.requestsInput());
        texts = conv.GetTextDocuments();
        requests = conv.GetRequests();
        conv.putAnswers({{{0, 0.989}, {1, 0.897}, {2, 0.750}, {3, 0.670}, {4, 0.561}},
                        {{0, 0.769}},
                        {}});
    } catch (OpeningError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (JsonFileContainingError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
