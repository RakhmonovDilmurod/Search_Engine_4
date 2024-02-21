#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <mutex>
#include <thread>

struct Entry{
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
}; 

class InvertedIndex {
public:
    InvertedIndex() = default;

    std::vector<std::string> docs;
    std::map<std::string,std::vector<Entry>> freq_dictionary;
    

    std::string wordCleaning(std::string word) {
        while ((word.back() >= ' ' && word.back() <= '/') || (word.back() >= ':' && word.back() <= '@')
               || (word.back() >= '[' && word.back() <= '`') || (word.back() >= '{' && word.back() <= '~')) {
            word.erase(word.end() - 1);
        }
        while ((word.front() >= ' ' && word.front() <= '/') || (word.front() >= ':' && word.front() <= '@')
               || (word.front() >= '[' && word.front() <= '`') || (word.front() >= '{' && word.front() <= '~')) {
            word.erase(0, 1);
        }
        return word;
    }    

    void convertDocumenttoDatabase(const std::string& txt, size_t num) {
        std::mutex freqAccess;
        std::map<std::string,size_t> words;
        std::istringstream iss(txt);
        std::string buf;
        while (!iss.eof()) {
            iss >> buf;
            buf = wordCleaning(buf);
            if (words.count(buf) == 0) {
                words.insert(std::pair<std::string,size_t>(buf, 1));
            } else {
                words[buf] += 1;
            }
        }
        for (auto & word : words) {
            freqAccess.lock();
            if (freq_dictionary.count(word.first) == 0) {
                freq_dictionary[word.first] = std::vector<Entry>{Entry{num, word.second}};
            } else {
                freq_dictionary[word.first].push_back(Entry{num, word.second});
            }
            freqAccess.unlock();
        }
    }   
 
    void UpdateDocumentBase(const std::vector<std::string>& input_docs) {
        docs = input_docs;
        std::vector<std::thread> threads;
        for (int i = 0; i < input_docs.size(); i++) {
            threads.emplace_back(&InvertedIndex::convertDocumenttoDatabase, this, docs[i], i);
        }
        for (int i = 0; i < input_docs.size(); i++) {
            threads[i].join();
        }
    }

    auto getFreqDictionary() {
        return freq_dictionary;
    }

    std::vector<Entry> GetWordCount(const std::string& word) const {
        std::string lowercase_word = word;
        std::transform(lowercase_word.begin(), lowercase_word.end(), lowercase_word.begin(), ::tolower);
        auto it = freq_dictionary.find(lowercase_word);
        if (it != freq_dictionary.end()) {
            return it->second;
        }
        return {};
    }
};
