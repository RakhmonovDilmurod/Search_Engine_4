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
#include <filesystem>
#include "SearchEngine.h"

using json = nlohmann::json;

class OpeningError : public std::exception {
private:
    std::string message;

public:
    OpeningError(const std::string  fileName)
        : message("Failed to open file: " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class JsonFileContainingError : public std::exception {
private:
    std::string message;

public:
    JsonFileContainingError(const std::string fileName, const std::string& containerName)
        : message("Failed to find container " + containerName + " in file " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};


class ConverterJSON {
public:

    const std::string configJsonPath   = "../config.json";
    const std::string requestsJsonPath =  "../requests.json";
    const std::string answersJsonPath  =  "../answers.json";
    json answersJsonFile;
    json requestsJsonFile;

public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> documents;
        std::ifstream configFile(configJsonPath,std::ios::in);

        if (!configFile.is_open()) {
            throw OpeningError(configJsonPath);
        }

        json config;
        configFile >> config;

        if (config.empty()) {
            throw OpeningError(requestsJsonPath);
        }

        for (const auto& doc : config["files"]) {
            documents.push_back(doc.get<std::string>());
        }

        return documents;
    }

    int GetResponsesLimit() {
        std::ifstream configFile(configJsonPath);
        json config;
        configFile >> config;

        return config.contains("max_responses") ? config["max_responses"].get<int>() : 0; // Default value or throw an exception.
    }

    std::vector<std::string> GetRequests() {
        std::ifstream ifSJsonFile(requestsJsonPath);
        ifSJsonFile >> requestsJsonFile;
        std::vector<std::string> requests = {};

        if (requestsJsonFile.contains("requests")) {
            for (const auto& i : requestsJsonFile["requests"]) {
                requests.push_back(i.get<std::string>());
            }

        return requests;
    }
        else{
            throw JsonFileContainingError(requestsJsonPath, "requests");
        }
    }

   void putAnswers( std::vector<std::vector<RelativeIndex>>& answers) {
    answersJsonFile["answers"] = nlohmann::json::array();
    for (size_t i = 0; i < answers.size(); ++i) {
        nlohmann::json answerObj = nlohmann::json::object();
        answerObj["request_id"] = i + 1;
        nlohmann::json relevanceArr = nlohmann::json::array();
        for (const auto& [docid, rank] : answers[i]) {
            nlohmann::json relevanceObj = nlohmann::json::object();
            relevanceObj["docid"] = docid;
            relevanceObj["rank"] = rank.count;
            relevanceArr.push_back(relevanceObj);
        }
        answerObj["relevance"] = relevanceArr;
        answersJsonFile["answers"].push_back(answerObj);
    }
    std::ofstream ofstreamJsonFile(answersJsonPath);
    ofstreamJsonFile << std::setw(4) << answersJsonFile;
    ofstreamJsonFile.close();
}
  
 };
