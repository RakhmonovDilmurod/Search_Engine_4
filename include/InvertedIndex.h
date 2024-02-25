#include "ConverterJSON.h"
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>

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
    if (input_docs.empty()) {
        return;
    }
    docs = input_docs;
    for (size_t i = 0; i < input_docs.size(); ++i) {
        IndexDocument(input_docs[i], i);
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

    void IndexDocument(const std::string& doc, size_t doc_id) {
    if (doc.empty()) {
        return;
    }
    std::map<std::string, size_t> word_count;
    std::istringstream iss(doc);
    std::string word;
    while (iss >> word) {
        std::string cleanedWord = WordCleaning(word);
        ++word_count[cleanedWord];
    }

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
   const std::map<std::string, std::vector<Entry>>& getFreqDictionary() const {
    return freq_dictionary;
}
};