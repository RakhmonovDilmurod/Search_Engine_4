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
    std::map<std::string, std::vector<Entry>> freq_dictionary = index.getFreqDictionary();
    std::vector<std::string> requestsInput = converter.GetRequests();
    
public:
std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result = {};
    for (const auto& i : requestsInput) {
        std::map<std::string, std::pair<size_t, int>> wordRelevance;
        std::vector<float> absoluteRelevance = {};
        std::vector<RelativeIndex> relativeRelevance;
        result.emplace_back();
        std::istringstream iss(i);
        std::string buf;
        std::vector<std::string> uniqueWords = {};
        while (!iss.eof()) {
            iss >> buf;
            buf = index.WordCleaning(buf);
            if (std::count(uniqueWords.begin(), uniqueWords.end(), buf) == 0) {
                uniqueWords.push_back(buf);
            }
        }
        for (const auto& j : uniqueWords) {
            for (size_t k = 0; k < converter.GetTextDocuments().size(); k++) {
                if (freq_dictionary.count(j) != 0) {
                    wordRelevance[j] = {k, freq_dictionary[j][k].count};
                } else {
                    wordRelevance[j] = {k, 0};
                }
            }
        }
        std::vector<int> documentFrequencies(converter.GetTextDocuments().size(), 0);
        for (const auto& j : wordRelevance) {
            documentFrequencies[j.second.first] += j.second.second;
        }
        float maxFrequency = *std::max_element(documentFrequencies.begin(), documentFrequencies.end());
        if (maxFrequency == 0) {
            maxFrequency = 1;
        }
        for (size_t j = 0; j < converter.GetTextDocuments().size(); j++) {
            absoluteRelevance.push_back(documentFrequencies[j] * 1.0 / maxFrequency);
        }
        for (size_t j = 0; j < absoluteRelevance.size(); j++) {
            relativeRelevance.push_back(RelativeIndex{(size_t)j, absoluteRelevance[j]});
        }
        result.push_back(relativeRelevance);
    }
    return result;
  }
};