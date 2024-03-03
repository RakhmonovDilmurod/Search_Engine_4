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
    std::filesystem::path basePath = std::filesystem::current_path() / "config";
    std::filesystem::path configJsonPathF = basePath / "config.json";
    std::filesystem::path requestsJsonPathF = basePath / "requests.json";
    std::filesystem::path answersJsonPathF = basePath / "answers.json";

    const std::string configJsonPath = configJsonPathF.string();
    const std::string requestsJsonPath = requestsJsonPathF.string();
    const std::string answersJsonPath = answersJsonPathF.string();



public:
    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> documents;

        try {
            std::ifstream ifSJsonFile(configJsonPath);
            if (!ifSJsonFile.is_open()) {
                throw OpeningError(configJsonPath);
            }

            json configJsonFile;
            ifSJsonFile >> configJsonFile;

            if (!configJsonFile.contains("files")) {
                throw JsonFileContainingError(configJsonPath, "files");
            }

            for (const auto& i : configJsonFile["files"]) {
                std::filesystem::path bufPath(i);
                if (!std::filesystem::exists(bufPath)) {
                    throw OpeningError(bufPath.string());
                }

                std::ifstream subFile(bufPath);
                if (!subFile.is_open()) {
                    throw OpeningError(bufPath.string());
                }

                // Check if the file is empty
                if (subFile.peek() == std::ifstream::traits_type::eof()) {
                    throw OpeningError(bufPath.string() + " is empty.");
                }

                subFile.seekg(0); // Reset the read position to the beginning
                std::ostringstream sstr;
                sstr << subFile.rdbuf();
                documents.push_back(sstr.str());
            }
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

            return config.contains("config") && config["config"].contains("max_responses")
                       ? config["config"]["max_responses"].get<int>()
                       : 5; 
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

    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
        json answersJsonFile;

        for (int i = 0; i < answers.size(); i++) {
            std::string numOfRequest = std::to_string(i + 1);

            if (!answers[i].empty()) {
                json& requestJson = answersJsonFile["answers"]["request" + numOfRequest];
                requestJson["result"] = true;
                
                for (const auto& j : answers[i]) {
                    json relevance;
                    relevance["docid"] = j.first;
                    relevance["rank"] = j.second;
                    requestJson["relevance"].push_back(relevance);
                }
            }
        }

        if (!answersJsonFile.empty()) {
            std::ofstream ofstreamJsonFile(answersJsonPath);
            ofstreamJsonFile << std::setw(4) << answersJsonFile << std::endl;
        }
    }
};

