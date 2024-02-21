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
#include <mutex>

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
    vector<string> docs; // список содержимого документов
    map<string,vector<Entry>> freq_dictionary; // частотный словарь
    
    
 void UpdateDocumentBase(const vector<string>& input_docs) {
        freq_dictionary.clear();
        docs.clear();
        size_t doc_id = 0;
        for (const auto& doc_content : input_docs) {
            std::istringstream iss(doc_content);
            std::string word;
            std::map<std::string, size_t> word_count;
            while (iss >> word) {
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                word_count[word]++;
            }
            for (const auto& [word, count] : word_count) {
                auto& entry_list = freq_dictionary[word];
                auto it = std::find_if(entry_list.begin(), entry_list.end(),
                    [&](const Entry& entry) { return entry.doc_id == doc_id; });
                if (it != entry_list.end()) {
                    it->count += count;
                } else {
                    entry_list.push_back({doc_id, count});
                }
            }
            docs.push_back(doc_content);
            ++doc_id;
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
