#include <iostream>
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
#include "SearchEngine.h"



int main(int argc, char** argv) {
    ConverterJSON conv;
    InvertedIndex inv;
    
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::vector<std::string> texts;
    std::vector<std::string> requests;
    inv.UpdateDocumentBase(conv.GetTextDocuments());

    try {
        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(searchSer.requestsInput());
        texts = conv.GetTextDocuments();
        requests = conv.GetRequests();
        std::vector<std::vector<std::pair<int, float>>> ReadyResults;
         for (const auto& outer : searchResults){
             std::vector<std::pair<int, float>> inner_Ready;
              for (const auto& relIndex : outer){
                inner_Ready.emplace_back(static_cast<int>(relIndex.doc_id), static_cast<float>(relIndex.rank.count));
              }
                ReadyResults.push_back(inner_Ready);
         }
        conv.putAnswers(ReadyResults);
    } catch (OpeningError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (JsonFileContainingError ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;

}
