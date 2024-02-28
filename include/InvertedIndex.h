#include "ConverterJSON.h"
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <mutex>
#include <thread>
#include <sstream>

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    std::vector<std::string> docs; // Содержимое документов

    void UpdateDocumentBase(const std::vector<std::string>& input_docs) {
        std::mutex mutex;
        std::unique_lock<std::mutex> lock(mutex);
        docs = input_docs;

        std::vector<std::thread> threads;
        for (size_t i = 0; i < input_docs.size(); i++) {
            threads.emplace_back([&, i](){
                std::unique_lock<std::mutex> lock(mutex);
                auto indexed_doc = WordCount(docs[i]);
                MergeIndexedDocument(indexed_doc);
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word) {
        auto it = freq_dictionary.find(word);
        if (it != freq_dictionary.end()) {
            return it->second;
        }
        return {};
    }

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    std::unordered_map<std::string, size_t> WordCount(const std::string& doc) {
        std::unordered_map<std::string, size_t> words;
        std::istringstream iss(doc);
        std::string buf;
        while (!iss.eof()) {
            iss >> buf;
            buf = WordCleaning(buf);
            if (words.count(buf) == 0) {
                words.insert(std::pair<std::string, size_t>(buf, 1));
            } else {
                words[buf] += 1;
            }
        }
        return words;
    }

    void MergeIndexedDocument(const std::unordered_map<std::string, size_t>& indexed_doc) {
        std::mutex mutex;
        std::unique_lock<std::mutex> lock(mutex);
        for (auto& word : indexed_doc) {
            if (freq_dictionary.count(word.first) == 0) {
                freq_dictionary[word.first] = std::vector<Entry>{ Entry{word.second, 1} };
            } else {
                freq_dictionary[word.first].push_back(Entry{word.second, 1});
            }
        }
    }

public:
    std::string WordCleaning(std::string& word) {
        while (!word.empty() && !isalnum(word.front())) {
            word.erase(word.begin());
        }
        while (!word.empty() && !isalnum(word.back())) {
            word.pop_back();
        }
        return word;
    }
 auto getFreqDictionary() const {
        std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        return freq_dictionary;
      
    }
};