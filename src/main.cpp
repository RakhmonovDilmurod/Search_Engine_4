#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"

// Function to handle the main search process
void performSearch(InvertedIndex& inv, ConverterJSON& conv) {
    try {
        SearchServer searchSer(inv);
        std::vector<std::vector<RelativeIndex>> searchResults = searchSer.search(searchSer.requestsInput());

        // Convert search results to the desired format
        std::vector<std::vector<std::pair<int, float>>> readyResults;
        for (const auto& outer : searchResults) {
            std::vector<std::pair<int, float>> innerReady;
            for (const auto& relIndex : outer) {
                innerReady.emplace_back(static_cast<int>(relIndex.doc_id), static_cast<float>(relIndex.rank.count));
            }
            readyResults.push_back(innerReady);
        }

        // Update answers
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

int main(int argc, char** argv) {
    ConverterJSON conv;
    InvertedIndex inv;

    // Update document base
    inv.UpdateDocumentBase(conv.GetTextDocuments());

    // Perform search
    performSearch(inv, conv);

    return 0;
}
