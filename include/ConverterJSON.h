#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <filesystem>

using json = nlohmann::json;

class OpeningError : public std::exception {
private:
    std::string message;

public:
    OpeningError(const std::string& fileName)
        : message("Failed to open file: " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class JsonFileContainingError : public std::exception {
private:
    std::string message;

public:
    JsonFileContainingError(const std::string& fileName, const std::string& containerName)
        : message("Failed to find container " + containerName + " in file " + fileName + ".") {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ConverterJSON {
private:
    const std::string configJsonPath = "config/config.json";
    const std::string requestsJsonPath ="config/requests.json";
    const std::string answersJsonPath = "config/answers.json";

public:
    std::vector<std::string> GetTextDocuments(){
    std::vector<std::string> documents;

    try {
        std::ifstream ifSJsonFile(configJsonPath);
        if (!ifSJsonFile.is_open()) {
            throw OpeningError(configJsonPath);
        }

        json configJsonFile;
        ifSJsonFile >> configJsonFile;

        if (configJsonFile.contains("files")) {
            for (const auto& i : configJsonFile["files"]) {
                std::filesystem::path bufPath(i);
                if (std::filesystem::exists(bufPath)) {
                    std::ifstream subFile(bufPath, std::ios::ate); // Open the file with std::ios::ate to check the length
                    if (subFile.is_open()) {
                        if (subFile.tellg() == 0) { // Check if the file is empty
                            throw OpeningError(bufPath.string() + " is empty.");
                        }
                        subFile.seekg(0); // Reset the read position to the beginning
                        std::ostringstream sstr;
                        sstr << subFile.rdbuf();
                        documents.push_back(sstr.str());
                    } else {
                        throw OpeningError(bufPath.string());
                    }
                } else {
                    throw OpeningError(bufPath.string());
                }
            }
        } else {
            throw JsonFileContainingError(configJsonPath, "files");
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return documents;
}

    int GetResponsesLimit(){
        std::ifstream configFile(configJsonPath);
        json config;
        configFile >> config;
        int limit = config["max_responses"];
        return limit;
    }

    std::vector<std::string> GetRequests() {
        std::vector<std::string> requests;

        try {
            std::ifstream requestsFile(requestsJsonPath);
            if (!requestsFile.is_open()) {
                throw OpeningError(requestsJsonPath);
            }

            json requestsJson;
            requestsFile >> requestsJson;

            if (requestsJson.contains("requests")) {
                for (const auto& req : requestsJson["requests"]) {
                    requests.push_back(req.get<std::string>());
                }
            } else {
                throw JsonFileContainingError(requestsJsonPath, "requests");
            }

            requestsFile.close();
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }

        return requests;
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers) {
    json answersJsonFile;
    for (int i = 0; i < answers.size(); i++) {
        std::string numOfRequest;
        for (int n = 0; n < 3-std::to_string(i).length();n++) {
            numOfRequest += "0";
        }
        numOfRequest += std::to_string(i+1);

        if (!answers[i].empty()) {
            answersJsonFile["answers"]["request" + numOfRequest]["result"] = "true";
            for (auto & j : answers[i]) {
               json::value_type block;
                block["docid"] = j.first;
                block["rank"] = j.second;
                answersJsonFile["answers"]["request" + numOfRequest]["relevance"].push_back(block);
            }
        }
    }

    if (!answersJsonFile.empty()) {
        std::ofstream ofstreamJsonFile(answersJsonPath);
        ofstreamJsonFile << answersJsonFile;

        std::ifstream ifstreamJsonFile(answersJsonPath);
        json memory;
        ifstreamJsonFile >> memory;
        ofstreamJsonFile.close();
        ifstreamJsonFile.close();
    }
}
};
