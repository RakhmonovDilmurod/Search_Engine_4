#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include <vector>

void performSearch(InvertedIndex& inv, ConverterJSON& conv) {
    try {
        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(searchSer.requestsInput());

        std::vector<std::vector<std::pair<int, float>>> readyResults;
        for (const auto& outer : searchResults) {
            std::vector<std::pair<int, float>> innerReady;
            for (const auto& relIndex : outer) {
                innerReady.emplace_back(static_cast<int>(relIndex.doc_id), static_cast<float>(relIndex.rank.count));
            }
            readyResults.push_back(innerReady);
        }

        conv.putAnswers(readyResults);
    }catch (const std::bad_alloc& e) {
    std::cerr << "Out of memory: " << e.what() << std::endl;
    exit(EXIT_FAILURE);

    } catch (OpeningError ex) {
        std::cerr << ex.what() << std::endl;
    } catch (JsonFileContainingError ex) {
        std::cerr << ex.what() << std::endl;
    }
}

int main(){
    ConverterJSON conv;
    InvertedIndex inv;


    inv.UpdateDocumentBase(conv.GetTextDocuments());

    performSearch(inv, conv);

    return 0;
}
