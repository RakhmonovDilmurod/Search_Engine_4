#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "ConverterJSON.h"
#include <thread>


using namespace std;

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
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    
    
 void UpdateDocumentBase(const vector<string>& input_docs) {
        freq_dictionary.clear();
        size_t doc_id = 0;
        vector<thread> threads;
        docs.clear();

        for (const auto& doc_content : input_docs) {
            docs.push_back(doc_content);
            threads.emplace_back([this, doc_content, doc_id]() {
                istringstream iss(doc_content);
                string word;
                map<string, size_t> word_count;

                while (iss >> word) {
                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    word_count[word]++;
                }

                for (const auto& [word, count] : word_count) {
                    freq_dictionary[word].push_back({doc_id, count});
                }
            });

            doc_id++;
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }

    auto getFreqDictionary() {
        return freq_dictionary;
}

vector<Entry> GetWordCount(const string& word) const {
        string lowercase_word = word;
        transform(lowercase_word.begin(), lowercase_word.end(), lowercase_word.begin(), ::tolower);
        auto it = freq_dictionary.find(lowercase_word);
        if (it != freq_dictionary.end()) {
            return it->second;
        }
        return {};
    }

};
