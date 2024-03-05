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
#include "InvertedIndex.h"
#include <set>
#include <cmath>


struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
SearchServer(InvertedIndex& index);
private:
    ConverterJSON converter;
    InvertedIndex index;
    std::vector<std::string> requestsInput = converter.GetRequests();
    
public:
std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
        std::vector<std::vector<RelativeIndex>> results;

        for (const auto& query : queries_input) {
            std::vector<RelativeIndex> query_results;
            std::map<size_t, float> doc_freq; 

          
            std::vector<std::string> words;
            std::stringstream ss(query);
            std::string word;
            while (ss >> word) {
                words.push_back(word);
            }

            
            std::vector<std::string> unique_words(words.size());
            std::unique_copy(words.begin(), words.end(), unique_words.begin());

           
            std::map<std::string, int> word_count;
            for (const auto& word : unique_words) {
                word_count[word] = index.GetWordCount(word).size();
            }
            std::sort(unique_words.begin(), unique_words.end(),
                [&word_count](const std::string& a, const std::string& b) {
                    return word_count[a] < word_count[b];
                });

         
            for (const auto& word : unique_words) {
                const auto& entries = index.GetWordCount(word);
                for (const auto& entry : entries) {
                    auto it = doc_freq.find(entry.doc_id);
                    if (it == doc_freq.end()) {
                        doc_freq[entry.doc_id] = entry.count;
                    } else {
                        it->second += entry.count;
                    }
                }
            }

        
            float max_relevance = 0;
            for (const auto& doc_freq_entry : doc_freq) {
                float relevance = static_cast<float>(doc_freq_entry.second) / index.getFreqDictionary().size();
                if (relevance > max_relevance) {
                    max_relevance = relevance;
                }
                query_results.push_back({ doc_freq_entry.first, relevance });
            }

            
            std::sort(query_results.begin(), query_results.end(),
                [](const RelativeIndex& a, const RelativeIndex& b) {
                    return a.rank > b.rank;
                });

            results.push_back(query_results);
        }

        return results;
    }};