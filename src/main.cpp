#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include <vector>

int main() {
    try {
        ConverterJSON conv;
        InvertedIndex inv;
        inv.UpdateDocumentBase(conv.GetTextDocuments());

        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(conv.GetRequests());
        std::vector<std::vector<std::pair<int, float>>> readyResults;
        for (const auto& outer : searchResults) {
            std::vector<std::pair<int, float>> innerReady;
            for (const auto& relIndex : outer) {
                innerReady.emplace_back(static_cast<int>(relIndex.doc_id), static_cast<float>(relIndex.rank.count));
            }
        conv.putAnswers(readyResults);

        return 0;
    }
   
    } catch (const std::bad_alloc& e) {
        std::cerr << "Out of memory: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const OpeningError& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const JsonFileContainingError& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}