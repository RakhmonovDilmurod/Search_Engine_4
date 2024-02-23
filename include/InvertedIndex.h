#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <string>
#include "ConverterJSON.h"
#include <shared_mutex>

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
    std::vector<std::string> docs;// список содержимого документов
    void UpdateDocumentBase(const std::vector<std::string>& input_docs) {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < input_docs.size(); ++i) {
            threads.emplace_back(&InvertedIndex::IndexDocument, this, input_docs[i], i);
        }
        for (auto& thread : threads) {
           if (thread.joinable()) {
            thread.join();
           }
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word) {
        std::mutex mutex_;
        std::lock_guard<std::mutex> lock(mutex_);
        if (freq_dictionary.find(word) != freq_dictionary.end()) {
            return freq_dictionary[word];
        }
        return {};
    }
 
private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    mutable std::shared_mutex mutex_;

    void IndexDocument(const std::string& doc, size_t doc_id) {
        std::map<std::string, size_t> word_count;
        std::istringstream iss(doc);
        std::string word;
        while (iss >> word) {
        std::string cleanedWord = WordCleaning(word);
        ++word_count[cleanedWord];
            }

        std::lock_guard<std::shared_mutex> lock(mutex_);
        for (const auto& [word, count] : word_count) {
            if (freq_dictionary.find(word) == freq_dictionary.end()) {
                freq_dictionary[word] = {{doc_id, count}};
            } else {
                freq_dictionary[word].emplace_back(Entry{doc_id, count});
            }
        }
    }

    std::string WordCleaning(std::string& word) {
        while (!word.empty() && !isalnum(word.front())) {
            word.erase(word.begin());
        }
        while (!word.empty() && !isalnum(word.back())) {
            word.pop_back();
        }
        return word;
    }

public:
    auto getFreqDictionary() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return freq_dictionary;
    }
};
