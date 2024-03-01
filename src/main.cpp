#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include <vector>
#include <exception>

int main() {
    try {
        ConverterJSON conv;
        InvertedIndex inv;
        inv.UpdateDocumentBase(conv.GetTextDocuments());
        SearchServer searchSer(inv);
        std::vector<std::string> texts = conv.GetTextDocuments();
        std::vector<std::string> requests = conv.GetRequests();

        std::vector<std::vector<std::pair<int, float>>> readyResults;
        for (const auto& outer : searchSer.search(requests)) {
            std::vector<std::pair<int, float>> innerReady;
            for (const auto& relIndex : outer) {
                innerReady.emplace_back(static_cast<int>(relIndex.doc_id), relIndex.rank);
            }
            readyResults.push_back(innerReady);
        }
        conv.putAnswers(readyResults);
        std::cout << "Finished successfully!" << std::endl;
        
        return 0;
    } catch (const OpeningError& ex) {
        std::cerr << "Error opening file: " << ex.what() << std::endl;
    } catch (const JsonFileContainingError& ex) {
        std::cerr << "Error parsing JSON file: " << ex.what() << std::endl;
    } catch (const std::bad_alloc& ex) {
        std::cerr << "Memory allocation error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 1; 
}
