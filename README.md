# Search Engine cpp

Welcome to the Search Engine cpp project! This is a lightweight, command-line-based search engine that provides an effective and efficient way to search through a  dataset of documents.
Description of the application: this application is a search system that uses search indexes to search text
files and return the relevance of each file to answers.json using a config.json file in which links to file locations should be provided. Also, for the application to work correctly, it actually requires requests, which must be provided in requests.json. Examples of exactly how to provide all the necessary data are located in the corresponding files in this folder.

## Prerequisites
To build and run this project, you need to have the following software installed on your system:
 
 - A C++ compiler that supports C++17 or later, such as g++, clang++, or Microsoft Visual C++
 - CMake (version 3.1 or later)

## Building and Running the Project
To build the project, follow these steps:
1. Open a terminal/command prompt and navigate to the project's root directory
2. Create a build directory
3. Run CMake to generate the build files:
   Cmake..
4. nlohmann_json is folder with functions to work with json files , it was added to project
5. Build the project using your preferred build tool.
6. After the build is complete, you can run the search engine.

## Launching the program:
1.Make sure you have all the required dependencies installed.
2.Check for a config.json file in the ../ directory. If the file doesn't exist, create it and make sure it's configured correctly.
3.Open a terminal and go to the directory with the program:
```ruby
bash
   cd .../Search_Engine_4
 ``` 
4.Run the program:
```ruby
bash
   ./SearchEngine
``` 
## Launching the tests: 
1.Make sure you have all the required test dependencies installed.
2.Open a terminal and go to the tests directory:
```ruby
bash
   cd .../Search_Engine_4/tests
```
3.Run the tests: 
```ruby
bash
./GtestTests
```
## Using the Search Engine
To use the search engine, you need to have a dataset of documents. The search engine supports the following files format: config.json , requests.json , answers.json
The search engine provides a simple command-line interface for searching documents. Type the search query and press Enter to receive the search results.

1. Configuration file config.json.
Without it, the application cannot be launched. It contains the name of the search engine,
its version, database update time (how often should it be done
re-indexing the database, re-uploading files and calculating their search engine
rating), maximum number of options in the answer (if not specified, then the value
is chosen to be five).
An example of a config.json file description:
 ```ruby
{
          "config": {
                      "name": "SkillboxSearchEngine",
                      "version": "0.1",
                      "max_responses": 5
                      },
          "files": [
                     "../resources/file001.txt",
                     "../resources/file002.txt",
                     "../resources/file003.txt",
                     "../resources/file004.txt"

   ]
}
```  
3. File with requests requests.json.
It contains queries that need to be processed by the search engine.
An example of a requests.json file description:
 ```ruby
  {
    "requests": [
              "some words..",
              "some words..",
              "some words..",
              "some words..".
              ]
                
  } 
 ```                                                                                                                                             
4. File with answers to requests answers.json.
The results of the search engine are recorded in it. If at start
application, this file does not exist in the directory with the project, then it is necessary
create. If the file already exists, then you need to erase all its contents.
An example of a config.json file description:
```ruby
{
 "answers": {
"request001": {"result": "true","relevance": {"docid": 0, “rank” : 0.989,"docid": 1, “rank” : 0.897,"docid": 2, “rank” : 0.750,"docid": 3, “rank” : 0.670,"docid": 4, “rank” : 0.561}},
"request002": {"result": "true","docid": 0, “rank” : 0.769},
"request003": {"result": "false"}
 }
}
```  
## Google Test for project:

### Add Google test in main.cpp:
```ruby
using namespace std;
void TestInvertedIndexFunctionality(
const vector<string>& docs,
const vector<string>& requests,
const std::vector<vector<Entry>>& expected
) {
std::vector<std::vector<Entry>> result;
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
for(auto& request : requests) {
std::vector<Entry> word_count = idx.GetWordCount(request);
result.push_back(word_count);
}
ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
const vector<string> docs = {
"london is the capital of great britain",
"big ben is the nickname for the Great bell of the striking clock"
};
const vector<string> requests = {"london", "the"};
const vector<vector<Entry>> expected = {
{
{0, 1}
}, {
{0, 1}, {1, 3}
}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
const vector<string> docs = {
"milk milk milk milk water water water",
"milk water water",
"milk milk milk milk milk water water water water water",
"americano cappuccino"
};
const vector<string> requests = {"milk", "water", "cappuchino"};
const vector<vector<Entry>> expected = {
{
{0, 4}, {1, 1}, {2, 5}
}, {
{0, 2}, {1, 2}, {2, 5}
}, {
{3, 1}
}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
const vector<string> docs = {
"a b c d e f g h i j k l",
"statement"
};
const vector<string> requests = {"m", "statement"};
const vector<vector<Entry>> expected = {
{
}, {
{1, 1}
}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}     
```        

## After running
### The result of testing project : 
[==========] Running 9 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 6 tests from ConverterJSON
[ RUN      ] ConverterJSON.readConfig
[       OK ] ConverterJSON.readConfig (0 ms)
[ RUN      ] ConverterJSON.readRequests
[       OK ] ConverterJSON.readRequests (0 ms)
[ RUN      ] ConverterJSON.getResponsesLimit
[       OK ] ConverterJSON.getResponsesLimit (0 ms)
[ RUN      ] ConverterJSON.getRequests
[       OK ] ConverterJSON.getRequests (0 ms)
[ RUN      ] ConverterJSON.getFiles
[       OK ] ConverterJSON.getFiles (0 ms)
[ RUN      ] ConverterJSON.putAnswers
[       OK ] ConverterJSON.putAnswers (0 ms)
[----------] 6 tests from ConverterJSON (30 ms total)

[----------] 1 test from SearchServer
[ RUN      ] SearchServer.search
[       OK ] SearchServer.search (0 ms)
[----------] 1 test from SearchServer (4 ms total)

[----------] 2 tests from InvertedIndex
[ RUN      ] InvertedIndex.updateDocumentBase
[       OK ] InvertedIndex.updateDocumentBase (0 ms)
[ RUN      ] InvertedIndex.getWordCount
[       OK ] InvertedIndex.getWordCount (0 ms)
[----------] 2 tests from InvertedIndex (12 ms total)

[----------] Global test environment tear-down
[==========] 9 tests from 3 test suites ran. (72 ms total)
[  PASSED  ] 9 tests.
```
