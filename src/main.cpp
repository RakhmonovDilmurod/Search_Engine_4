#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include <vector>

int main() {
     ConverterJSON conv;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::vector<std::string> texts;
    std::vector<std::string> requests;
    InvertedIndex inv;
    inv.UpdateDocumentBase(conv.GetTextDocuments());
    try {
        SearchServer searchSer(inv);
        searchSer.search(searchSer.requestsInput());
        texts = conv.GetTextDocuments();
        requests = conv.GetRequests();
        std::vector<std::vector<std::pair<int, float>>> readyResults;
        for (const auto& outer : searchSer.search(requests)) {
            std::vector<std::pair<int, float>> innerReady;
            for (const auto& relIndex : outer) {
                innerReady.emplace_back(static_cast<int>(relIndex.doc_id), static_cast<float>(relIndex.rank.count));
            }
            readyResults.push_back(innerReady);
        }
        conv.putAnswers(readyResults);
        std::cout << "Finished successfully!" << std::endl;
    } catch (OpeningError &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (JsonFileContainingError &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

}