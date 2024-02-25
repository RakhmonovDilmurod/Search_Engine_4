#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

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
    const std::string requestsJsonPath = "config/requests.json";
    const std::string answersJsonPath = "config/answers.json";

public:
    std::vector<std::string> GetTextDocuments() {
      std::vector<std::string> documents{};

        try {
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

            configFile.close();
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }

        return documents;
    }

    int GetResponsesLimit() {
        try {
            std::ifstream configFile(configJsonPath);
            if (!configFile.is_open()) {
                throw OpeningError(configJsonPath);
            }

            json config;
            configFile >> config;

            configFile.close();

            return config.contains("max_responses") ? config["max_responses"].get<int>() : 5;
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return 5;
        }
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

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        try {
            std::ofstream answersFile(answersJsonPath);
            json answersJson;
            std::ifstream existingAnswersFile(answersJsonPath);
            if (existingAnswersFile.is_open()) {
                existingAnswersFile >> answersJson;
                existingAnswersFile.close();
            }
            if (!answersFile.is_open()) {
                throw OpeningError(answersJsonPath);
            }

            int requestNumber = 1;
            for (const auto& answer : answers) {
                json answerObj;
                if (answer.empty()) {
                    answerObj["result"] = "false";
                } else {
                    answerObj["result"] = "true";
                    json relevanceArray = json::array();
                    for (const auto& doc : answer) {
                        json block;
                        block["docid"] = doc.first;
                        block["rank"] = doc.second;
                        relevanceArray.push_back(block);
                    }
                    answerObj["relevance"] = relevanceArray;
                }

                answersJson["answers"]["request" + std::to_string(requestNumber)] = answerObj;
                requestNumber++;
            }

            answersFile << std::setw(4) << answersJson;
            answersFile.close();
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }
    }
};
