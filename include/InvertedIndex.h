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

   void UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs.clear();
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < input_docs.size(); ++doc_id) {
        const std::string& document_content = input_docs[doc_id];
        docs.push_back(document_content);
        std::istringstream iss(document_content);
        std::string word;
        while (iss >> word) {
            freq_dictionary[word].push_back({doc_id, 1});
        }
    }
}

    std::vector<Entry> GetWordCount(const std::string& word){
        std::vector<Entry> result;
    auto it = freq_dictionary.find(word);
        if (it != freq_dictionary.end()) {
            result = it->second; } 
    return result;
}

    auto getFreqDictionary() {
        return freq_dictionary;
}


public:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};
