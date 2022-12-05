#include <iostream>

#include <set>

#include <string>

#include <vector>

#include <algorithm>

using std::string, std::cin, std::vector, std::set, std::pair, std::cout;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector < string > SplitIntoWords(const string & text) {
    vector < string > words;
    string word;
    for (const char c: text) {
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

[[maybe_unused]] set < string > ParseStopWords(const string & text) {
    set < string > stop_words;
    for (const string & word: SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector < string > SplitIntoWordsWithoutStopWords(const string & text,
                                                           const set < string > & stop_words) {
    vector < string > words;
    for (const string & word: SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

void AddDocument(vector < pair < int, vector < string >>> & documents,
                 const set < string > & stop_words,
                 int document_id,
                 const string & document) {
    const vector < string > words = SplitIntoWordsWithoutStopWords(document, stop_words);
    documents.emplace_back(document_id, words);
}

set < string > ParseQuery(const string & text,
                                    const set < string > & stop_words) {
    set < string > query_words;
    for (const string & word: SplitIntoWordsWithoutStopWords(text, stop_words)) {
        query_words.insert(word);
    }

    return query_words;
}

int MatchDocument(const pair < int, vector < string >> & content,
                  const set < string > & query_words) {
    // this function does not match anything it returns a relevance of a request
    int relevance = 0;
    vector < string > query = content.second;

    for (const auto & i: query) {
        relevance += int(query_words.count(i));
    }

    return relevance;
}

// for each found doc, returns an ID and a relevance
vector < pair < int, int >> FindDocuments(const vector < pair < int, vector < string >>> & documents,
                                                    const set < string > & stop_words,
                                                    const string & query) {

    const set < string > query_words = ParseQuery(query, stop_words);
    vector < int > matched_documents;
    vector < pair < int, int >> id_and_relevance;
    int document_id = 0;

    for (const auto & document: documents) {
        int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            id_and_relevance.emplace_back(document_id, relevance);
        }
        ++document_id;
    }

    return id_and_relevance;
}

int main() {
    const set < string > stop_words {
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
    cout << "How many results are in the Database?" << '\n';
    vector < pair < int, vector < string >>> documents;
    const int document_count = ReadLineWithNumber();


    cout << "Fill the Database with potential results:" << '\n';
    vector < string > original_queries;

    for (int document_id = 0; document_id < document_count; ++document_id) {
        string request = ReadLine();
        AddDocument(documents, stop_words, document_id, request);
        original_queries.push_back(request);
    }


    cout << "Input your request:" << '\n';
    const string query = ReadLine();

    // printing results
    cout << "You might be searching for: " << '\n';
    for (const auto &[document_id, relevance]: FindDocuments(documents, stop_words, query)) {
        cout << original_queries[document_id] << " : ";
        cout << "{ relevance = " << relevance << ", document_id = " << document_id << " }" << '\n';
    } 

    return 0;
}