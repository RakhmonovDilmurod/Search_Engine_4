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
 
    
 void UpdateDocumentBase(const std::vector<std::string>& input_docs) {
        docs.clear();
        freq_dictionary.clear();
        size_t doc_id = 0;
        std::vector<std::thread> threads;
        for (const auto& doc_content : input_docs) {
            threads.emplace_back([this, doc_content, doc_id]() {
                std::istringstream iss(doc_content);
                std::string word;
                while (iss >> word) {
                    auto& entry_list = freq_dictionary[word];
                    auto it = std::find_if(entry_list.begin(), entry_list.end(),
                                           [doc_id](const Entry& entry) { return entry.doc_id == doc_id; });
                    if (it != entry_list.end()) {
                        it->count++;
                    } else {
                        entry_list.push_back({doc_id, 1});
                    }
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

    std::vector<Entry> GetWordCount(const std::string& word) {
       std::vector<Entry> result;
       auto it = freq_dictionary.find(word);
       if (it != freq_dictionary.end()) {
           result = it->second;
    }
       return result;
  }
};
