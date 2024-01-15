#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

std::string ReadLine()
{
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int ReadLineWithNumber()
{
    int result = 0;
    std::cin >> result;
    ReadLine();
    return result;
}

std::vector<std::string> SplitIntoWords(const std::string& text)
{
    std::vector<std::string> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

[[maybe_unused]] std::set<std::string> ParseStopWords(const std::string& text)
{
    std::set<std::string> stop_words;
    for (const std::string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

std::vector<std::string> SplitIntoWordsWithoutStopWords(const std::string& text,
    const std::set<std::string>& stop_words)
{
    std::vector<std::string> words;
    for (const std::string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

void AddDocument(std::vector<std::pair<int, std::vector<std::string>>>& documents, const std::set<std::string>& stop_words, int document_id, const std::string& document)
{
    const std::vector<std::string> words = SplitIntoWordsWithoutStopWords(document, stop_words);
    documents.emplace_back(document_id, words);
}

std::set<std::string> ParseQuery(const std::string& text, const std::set<std::string>& stop_words)
{
    std::set<std::string> query_words;
    for (const std::string& word : SplitIntoWordsWithoutStopWords(text, stop_words)) {
        query_words.insert(word);
    }

    return query_words;
}

int MatchDocument(const std::pair<int, std::vector<std::string>>& content, const std::set<std::string>& query_words)
{
    // this function does not match anything it returns a relevance of a request
    int relevance = 0;
    std::vector<std::string> query = content.second;

    for (const auto& i : query) {
        relevance += int(query_words.count(i));
    }

    return relevance;
}

// for each found doc, returns an ID and a relevance
std::vector<std::pair<int, int>> FindDocuments(const std::vector<std::pair<int, std::vector<std::string>>>& documents, const std::set<std::string>& stop_words, const std::string& query)
{

    const std::set<std::string> query_words = ParseQuery(query, stop_words);
    std::vector<int> matched_documents;
    std::vector<std::pair<int, int>> id_and_relevance;
    int document_id = 0;

    for (const auto& document : documents) {
        int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            id_and_relevance.emplace_back(document_id, relevance);
        }
        ++document_id;
    }

    return id_and_relevance;
}

int main()
{
    const std::set<std::string> stop_words {
        "a",
        "an",
        "on",
        "the",
        "in",
        "is",
        "has",
        "been",
        "are",
        "with",
        "for",
        "from",
        "have",
        "be",
        "was"
    }; // those are going to be ignored by the search engine

    // filling our Database with search results
    std::cout << "How many results are in the Database?" << '\n';
    std::vector<std::pair<int, std::vector<std::string>>> documents;
    const int document_count = ReadLineWithNumber();

    std::cout << "Fill the Database with potential results:" << '\n';
    std::vector<std::string> original_queries;

    for (int document_id = 0; document_id < document_count; ++document_id) {
        std::string request = ReadLine();
        AddDocument(documents, stop_words, document_id, request);
        original_queries.push_back(request);
    }

    std::cout << "Input your request:" << '\n';
    const std::string query = ReadLine();

    // printing results
    std::cout << "You might be searching for: " << '\n';
    for (const auto& item : FindDocuments(documents, stop_words, query)) {
        std::cout << original_queries[item.first] << " : ";
        std::cout << "{ relevance = " << item.second << ", document_id = " << item.first << " }" << '\n';
    }

    return 0;
}
