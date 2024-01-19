#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "ConvertJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <gtest/gtest.h>


int main(int argc, char** argv) {
    ConverterJSON conv;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::vector<std::string> texts;
    std::vector<std::string> requests;
    InvertedIndex inv;
    inv.UpdateDocumentBase(conv.GetTextDocuments());

    try {
        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(searchSer.requestsInput());
        texts = conv.GetTextDocuments();
        requests = conv.GetRequests();
        conv.putAnswers({{{0, 0.989}, {1, 0.897}, {2, 0.750}, {3, 0.670}, {4, 0.561}},
                        {{0, 0.769}},
                        {}});
    } catch (OpeningError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (JsonFileContainingError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
    
}
