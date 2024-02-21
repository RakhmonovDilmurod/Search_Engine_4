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

  const std::string configJsonPath   = "config/config.json";
  const std::string requestsJsonPath = "config/requests.json";
  const std::string answersJsonPath  = "config/answers.json";
    json answersJsonFile;
    json requestsJsonFile;

public:
    ConverterJSON() = default;

  std::vector<std::string> GetTextDocuments() {
    std::vector<std::string> documents;
    
    std::ifstream configFile(configJsonPath);

    if (!configFile.is_open()) {
        throw OpeningError(configJsonPath);
    }

    json config;
    configFile >> config;

    if (config.empty()) {
        throw OpeningError(configJsonPath);
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


void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ofstream ofstreamJsonFile(answersJsonPath, std::ios::app); 
    for (int i = 0; i < answers.size(); i++) {
        std::string strI = "";
        for (int n = 0; n < 3 - std::to_string(i).length(); n++) {
            strI += "0";
        }
        strI += std::to_string(i + 1);

        json answerObj;
        if (answers[i].empty()) {
            answerObj["result"] = "false";
        } else {
            answerObj["result"] = "true";
            json relevanceArray = json::array();  
            for (int j = 0; j < answers[i].size(); j++) {
                json block;
                block["docid"] = answers[i][j].first;
                block["rank"] = answers[i][j].second;
                relevanceArray.push_back(block);
            }
            answerObj["relevance"] = relevanceArray;
        }

        answersJsonFile["answers"]["request" + strI] = answerObj;

    }

    ofstreamJsonFile << std::setw(4) << answersJsonFile;
    ofstreamJsonFile.close();
}
 
 };
