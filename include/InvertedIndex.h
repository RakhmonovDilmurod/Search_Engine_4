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
    std::vector<std::mutex> mutexes(input_docs.size());
    std::vector<std::thread> threads = {};
    for (int i = 0; i < input_docs.size(); i++) {
        threads.emplace_back([&](int index) {
    std::unique_lock<std::mutex> lock(mutexes[index]);
    InvertedIndex::IndexDocument(input_docs[index], index);}, i);
    }
    for (int i = 0; i < input_docs.size(); i++) {
        threads[i].join();
    }
}
   std::vector<Entry> GetWordCount(const std::string& word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}
   
    std::map<std::string, std::vector<Entry>> freq_dictionary;
 private:   
   void IndexDocument(const std::string& doc, size_t doc_id){ 
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    std::map<std::string,size_t> words;
    std::istringstream iss(doc);
    std::string buf;
    while (!iss.eof()){
        iss >> buf;
        buf = WordCleaning(buf);
        if (words.count(buf) == 0){
            words.insert(std::pair<std::string,size_t>(buf,1));
        } else {
            words[buf] += 1;
        }
    }
    for (auto & word : words) {
        
        mutex.lock();
        if (freq_dictionary.count(word.first) == 0) {
           freq_dictionary[word.first] = std::vector<Entry>{ Entry{doc_id, word.second} };
           freq_dictionary[word.first] = {Entry{doc_id, word.second} };;
        } else {
            freq_dictionary[word.first].push_back(Entry{doc_id, word.second});
        }
        mutex.unlock();
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

public:
   const std::map<std::string, std::vector<Entry>>& getFreqDictionary() const {
    return freq_dictionary;
  }
};