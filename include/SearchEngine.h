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


struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer{
public:
    SearchServer(InvertedIndex& idx) : _index(idx) {}

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
        std::vector<std::vector<RelativeIndex>> results;
        for (const auto& query : queries_input) {
            std::vector<std::string> words = SplitAndSort(query);
            std::vector<size_t> doc_ids = FindDocuments(words);
            if (doc_ids.empty()) {
                results.emplace_back();
                continue;
            }
            std::vector<RelativeIndex> relative_indices;
            for (size_t doc_id : doc_ids) {
                float relevance = CalculateRelevance(doc_id, words);
                relative_indices.push_back({doc_id, relevance});
            }
            std::sort(relative_indices.begin(), relative_indices.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
                return a.rank > b.rank;
            });
            results.push_back(relative_indices);
        }
        return results;
    }

private:
    InvertedIndex& _index;

    std::vector<std::string> SplitAndSort(const std::string& query) {
        std::vector<std::string> words;
        std::istringstream iss(query);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        std::sort(words.begin(), words.end(), [&](const std::string& a, const std::string& b) {
            return _index.GetWordCount(a).size() < _index.GetWordCount(b).size();
        });
        return words;
    }

    std::vector<size_t> FindDocuments(const std::vector<std::string>& words) {
        std::vector<size_t> doc_ids;
        if (words.empty()) return doc_ids;
        std::set<size_t> common_docs;
        for (const auto& word : words) {
            std::vector<Entry> word_docs = _index.GetWordCount(word);
            std::set<size_t> word_doc_ids;
            for (const auto& entry : word_docs) {
                word_doc_ids.insert(entry.doc_id);
            }
            if (common_docs.empty()) {
                common_docs = word_doc_ids;
            } else {
                std::set<size_t> intersection;
                std::set_intersection(common_docs.begin(), common_docs.end(), word_doc_ids.begin(), word_doc_ids.end(),
                                      std::inserter(intersection, intersection.begin()));
                common_docs = intersection;
            }
        }
        doc_ids.assign(common_docs.begin(), common_docs.end());
        return doc_ids;
    }

    float CalculateRelevance(size_t doc_id, const std::vector<std::string>& words) {
        float max_relevance = 0.0f;
        float total_relevance = 0.0f;
        for (const auto& word : words) {
            std::vector<Entry> word_docs = _index.GetWordCount(word);
            for (const auto& entry : word_docs) {
                if (entry.doc_id == doc_id) {
                    total_relevance += entry.count;
                    break;
                }
            }
        }
        for (const auto& word : words) {
            std::vector<Entry> word_docs = _index.GetWordCount(word);
            for (const auto& entry : word_docs) {
                max_relevance += entry.count;
            }
        }
        return total_relevance / max_relevance;
    }
};

