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
    ConverterJSON conv;
    InvertedIndex() = default;
    std::vector<std::string> docs = conv.GetTextDocuments(); // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь

    
   void UpdateDocumentBase(std::vector<std::string> input_docs) {
    freq_dictionary.clear();
    size_t doc_id = docs.size();
    for (const std::string& document_content : input_docs) {
        docs.push_back(document_content);
        std::istringstream iss(document_content);
        std::string word;
        while (iss >> word) {
            auto it = freq_dictionary.find(word);
            if (it != freq_dictionary.end()) {
                for (auto& entry : it->second) {
                    if (entry.doc_id == doc_id) {
                        entry.count++;
                        break;
                    }
                }
            } else {
                freq_dictionary[word] = { {doc_id, 1} };
            }
        }
        doc_id++;
    }
}

    auto getFreqDictionary() {
        return freq_dictionary;
}

    std::vector<Entry> GetWordCount(const std::string& word) {
       std::vector<Entry> result;
       auto it = freq_dictionary.find(word);
       if (it != freq_dictionary.end()) {
           result = it->second;
    }
       return result;
  }
};
