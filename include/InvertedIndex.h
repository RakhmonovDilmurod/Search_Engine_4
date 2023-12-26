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
#include "ConvertJSON.h"



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
