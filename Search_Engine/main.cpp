#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <memory>
#include<bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Document structure to represent indexed content
struct Document {
    int docId;
    string title;
    string url;
    string content;
    double pageRank;
    int wordCount;
    unordered_map<string, int> termFrequency;
    
    Document() : docId(0), pageRank(1.0), wordCount(0) {}
    
    Document(int id, string t, string u, string c) 
        : docId(id), title(t), url(u), content(c), pageRank(1.0) {
        calculateWordCount();
        calculateTermFrequency();
    }
    
    void calculateWordCount() {
        istringstream iss(content);
        string word;
        wordCount = 0;
        while (iss >> word) {
            wordCount++;
        }
    }
    
    void calculateTermFrequency() {
        termFrequency.clear();
        istringstream iss(content + " " + title);
        string word;
        
        while (iss >> word) {
            // Convert to lowercase and remove punctuation
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            
            if (!word.empty() && word.length() > 2) { // Ignore very short words
                termFrequency[word]++;
            }
        }
    }
};

// Search result structure
struct SearchResult {
    int docId;
    string title;
    string url;
    double relevanceScore;
    vector<string> matchedTerms;
    string snippet;
    
    SearchResult() : docId(0), relevanceScore(0.0) {}
    
    SearchResult(int id, string t, string u, double score) 
        : docId(id), title(t), url(u), relevanceScore(score) {}
    
    bool operator<(const SearchResult& other) const {
        return relevanceScore > other.relevanceScore; // Higher score first
    }
};

// Trie Node for efficient prefix matching
struct TrieNode {
    unordered_map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    unordered_set<int> documentIds; // Documents containing this word
    int frequency; // Global frequency of this word
    
    TrieNode() : isEndOfWord(false), frequency(0) {}
};

// Advanced Search Engine Class
class SearchEngine {
private:
    vector<Document> documents;
    shared_ptr<TrieNode> trieRoot;
    unordered_map<string, vector<int>> invertedIndex;
    unordered_map<string, double> idfCache; // Inverse Document Frequency cache
    unordered_map<int, unordered_set<int>> linkGraph; // For PageRank calculation
    string documentsFile = "search_documents.txt";
    string indexFile = "search_index.txt";
    
    // Stop words to ignore during indexing
    unordered_set<string> stopWords = {
        "the", "a", "an", "and", "or", "but", "in", "on", "at", "to", "for", 
        "of", "with", "by", "is", "are", "was", "were", "be", "been", "have", 
        "has", "had", "do", "does", "did", "will", "would", "should", "could"
    };

public:
    SearchEngine() {
        trieRoot = make_shared<TrieNode>();
        loadData();
        initializeDefaultData();
        buildIndex();
    }
    
    ~SearchEngine() {
        saveData();
    }
    
    // Initialize with sample documents
    void initializeDefaultData() {
        if (documents.empty()) {
            // Add comprehensive sample documents
            documents.push_back(Document(1, 
                "Introduction to Data Structures and Algorithms",
                "https://example.com/dsa-intro",
                "Data structures and algorithms are fundamental concepts in computer science. "
                "Arrays, linked lists, trees, and graphs are essential data structures. "
                "Sorting algorithms like quicksort, mergesort, and heapsort are crucial for efficient programming. "
                "Time complexity analysis helps evaluate algorithm performance using Big O notation. "
                "Hash tables provide O(1) average case lookup time for key-value operations."
            ));
            
            documents.push_back(Document(2,
                "Advanced C++ Programming Techniques",
                "https://example.com/cpp-advanced",
                "C++ is a powerful programming language with advanced features like templates, "
                "polymorphism, and memory management. Smart pointers help manage dynamic memory allocation. "
                "STL containers like vector, map, and unordered_map provide efficient data storage. "
                "Object-oriented programming principles including encapsulation, inheritance, and polymorphism "
                "are key to writing maintainable C++ code. Modern C++ features include auto keyword and lambda functions."
            ));
            
            documents.push_back(Document(3,
                "Machine Learning and Artificial Intelligence",
                "https://example.com/ml-ai",
                "Machine learning algorithms enable computers to learn from data without explicit programming. "
                "Supervised learning includes classification and regression techniques. Neural networks and deep learning "
                "have revolutionized AI applications. Popular algorithms include decision trees, random forests, "
                "support vector machines, and gradient boosting. Python libraries like TensorFlow and PyTorch "
                "facilitate machine learning development."
            ));
            
            documents.push_back(Document(4,
                "Web Development with Modern Technologies",
                "https://example.com/web-dev",
                "Web development involves front-end and back-end technologies. HTML, CSS, and JavaScript "
                "form the foundation of web applications. React, Angular, and Vue.js are popular frontend frameworks. "
                "Node.js enables server-side JavaScript development. RESTful APIs and GraphQL facilitate "
                "data communication between client and server. Database technologies include SQL and NoSQL systems."
            ));
            
            documents.push_back(Document(5,
                "Database Systems and SQL Optimization",
                "https://example.com/database-sql",
                "Relational databases use SQL for data manipulation and querying. Database normalization "
                "reduces redundancy and improves data integrity. Indexing strategies improve query performance. "
                "ACID properties ensure database transaction reliability. NoSQL databases like MongoDB "
                "provide flexible schema design. Query optimization techniques include proper indexing, "
                "join optimization, and query plan analysis."
            ));
            
            documents.push_back(Document(6,
                "Software Engineering Best Practices",
                "https://example.com/software-engineering",
                "Software engineering encompasses the systematic approach to software development. "
                "Version control systems like Git enable collaborative development. Testing methodologies "
                "include unit testing, integration testing, and system testing. Agile development practices "
                "promote iterative development and customer collaboration. Design patterns provide reusable "
                "solutions to common programming problems. Code review and continuous integration improve code quality."
            ));
            
            documents.push_back(Document(7,
                "Computer Networks and Internet Protocols",
                "https://example.com/networking",
                "Computer networks enable communication between distributed systems. TCP/IP protocol suite "
                "forms the foundation of internet communication. HTTP and HTTPS protocols facilitate web "
                "communication. Network security includes encryption, authentication, and firewall protection. "
                "Load balancing and content delivery networks improve web application performance. "
                "Network troubleshooting involves analyzing packet flows and connection states."
            ));
            
            documents.push_back(Document(8,
                "Operating Systems and System Programming",
                "https://example.com/operating-systems",
                "Operating systems manage computer hardware and software resources. Process management "
                "includes scheduling, synchronization, and inter-process communication. Memory management "
                "involves virtual memory, paging, and segmentation. File systems organize and store data "
                "on storage devices. Device drivers facilitate communication between OS and hardware. "
                "System calls provide interface between user programs and kernel services."
            ));
            
            // Build link graph for PageRank (simplified example)
            linkGraph[1] = {2, 3, 4}; // Doc 1 links to docs 2, 3, 4
            linkGraph[2] = {1, 3, 5}; // Doc 2 links to docs 1, 3, 5
            linkGraph[3] = {1, 4, 6}; // And so on...
            linkGraph[4] = {2, 5, 7};
            linkGraph[5] = {3, 6, 8};
            linkGraph[6] = {4, 7, 8};
            linkGraph[7] = {5, 6, 8};
            linkGraph[8] = {6, 7, 1};
            
            calculatePageRank();
        }
    }
    
    // Trie operations
    void insertWordInTrie(const string& word, int docId) {
        shared_ptr<TrieNode> current = trieRoot;
        
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = make_shared<TrieNode>();
            }
            current = current->children[c];
        }
        
        current->isEndOfWord = true;
        current->documentIds.insert(docId);
        current->frequency++;
    }
    
    vector<string> getWordsWithPrefix(const string& prefix) {
        vector<string> results;
        shared_ptr<TrieNode> current = trieRoot;
        
        // Navigate to prefix
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return results; // Prefix not found
            }
            current = current->children[c];
        }
        
        // DFS to find all words with this prefix
        function<void(shared_ptr<TrieNode>, string)> dfs = 
            [&](shared_ptr<TrieNode> node, string currentWord) {
                if (node->isEndOfWord) {
                    results.push_back(currentWord);
                }
                
                for (auto& [ch, child] : node->children) {
                    dfs(child, currentWord + ch);
                }
            };
        
        dfs(current, prefix);
        return results;
    }
    
    // Build inverted index and trie
    void buildIndex() {
        cout << "Building search index..." << endl;
        
        // Clear existing indices
        trieRoot = make_shared<TrieNode>();
        invertedIndex.clear();
        idfCache.clear();
        
        // Build index for each document
        for (const auto& doc : documents) {
            unordered_set<string> uniqueWords;
            
            // Process document content and title
            istringstream iss(doc.content + " " + doc.title);
            string word;
            
            while (iss >> word) {
                // Normalize word
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                
                // Skip stop words and very short words
                if (word.length() <= 2 || stopWords.count(word)) {
                    continue;
                }
                
                // Add to trie
                insertWordInTrie(word, doc.docId);
                
                // Add to inverted index
                if (uniqueWords.find(word) == uniqueWords.end()) {
                    invertedIndex[word].push_back(doc.docId);
                    uniqueWords.insert(word);
                }
            }
        }
        
        // Calculate IDF for all terms
        calculateIDF();
        
        cout << "Index built successfully! Indexed " << documents.size() 
             << " documents with " << invertedIndex.size() << " unique terms." << endl;
    }
    
    // Calculate Inverse Document Frequency
    void calculateIDF() {
        int totalDocs = documents.size();
        
        for (const auto& [term, docList] : invertedIndex) {
            int docsContainingTerm = docList.size();
            idfCache[term] = log(double(totalDocs) / docsContainingTerm);
        }
    }
    
    // Calculate PageRank using simplified algorithm
    void calculatePageRank(int iterations = 10, double dampingFactor = 0.85) {
        int numDocs = documents.size();
        vector<double> oldRanks(numDocs + 1, 1.0);
        vector<double> newRanks(numDocs + 1, 1.0);
        
        for (int iter = 0; iter < iterations; iter++) {
            fill(newRanks.begin(), newRanks.end(), (1.0 - dampingFactor) / numDocs);
            
            for (int docId = 1; docId <= numDocs; docId++) {
                if (linkGraph.count(docId)) {
                    double contribution = dampingFactor * oldRanks[docId] / linkGraph[docId].size();
                    
                    for (int linkedDoc : linkGraph[docId]) {
                        if (linkedDoc <= numDocs) {
                            newRanks[linkedDoc] += contribution;
                        }
                    }
                }
            }
            
            oldRanks = newRanks;
        }
        
        // Update document PageRank values
        for (auto& doc : documents) {
            if (doc.docId <= numDocs) {
                doc.pageRank = newRanks[doc.docId];
            }
        }
    }
    
    // Calculate TF-IDF score
    double calculateTFIDF(const string& term, const Document& doc) {
        auto tfIt = doc.termFrequency.find(term);
        if (tfIt == doc.termFrequency.end()) {
            return 0.0;
        }
        
        double tf = double(tfIt->second) / doc.wordCount;
        double idf = idfCache.count(term) ? idfCache[term] : 0.0;
        
        return tf * idf;
    }
    
    // Advanced search with ranking
    vector<SearchResult> search(const string& query, int maxResults = 10) {
        auto startTime = high_resolution_clock::now();
        
        // Parse query into terms
        vector<string> queryTerms = parseQuery(query);
        if (queryTerms.empty()) {
            return {};
        }
        
        // Find candidate documents
        unordered_map<int, double> candidateScores;
        unordered_map<int, vector<string>> documentMatches;
        
        for (const string& term : queryTerms) {
            if (invertedIndex.count(term)) {
                for (int docId : invertedIndex[term]) {
                    candidateScores[docId] += calculateTFIDF(term, documents[docId - 1]);
                    documentMatches[docId].push_back(term);
                }
            }
        }
        
        // Create search results
        vector<SearchResult> results;
        
        for (const auto& [docId, score] : candidateScores) {
            const Document& doc = documents[docId - 1];
            SearchResult result(docId, doc.title, doc.url, score);
            result.matchedTerms = documentMatches[docId];
            
            // Apply PageRank boost
            result.relevanceScore *= (1.0 + doc.pageRank * 0.1);
            
            // Generate snippet
            result.snippet = generateSnippet(doc.content, queryTerms);
            
            results.push_back(result);
        }
        
        // Sort by relevance score
        sort(results.begin(), results.end());
        
        // Limit results
        if (results.size() > static_cast<size_t>(maxResults)) {
            results.resize(static_cast<size_t>(maxResults));
        }
        
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(endTime - startTime);
        
        cout << "\nSearch completed in " << duration.count() << " ms" << endl;
        cout << "Found " << results.size() << " results for \"" << query << "\"" << endl;
        
        return results;
    }
    
    // Parse search query
    vector<string> parseQuery(const string& query) {
        vector<string> terms;
        istringstream iss(query);
        string word;
        
        while (iss >> word) {
            // Normalize
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            
            if (word.length() > 2 && !stopWords.count(word)) {
                terms.push_back(word);
            }
        }
        
        return terms;
    }
    
    // Generate search result snippet
    // Generate search result snippet
string generateSnippet(const string& content, const vector<string>& queryTerms, int maxLength = 150) {
    // Find the first occurrence of any query term
    size_t bestPos = string::npos;
    
    for (const string& term : queryTerms) {
        size_t pos = content.find(term);
        if (pos != string::npos && (bestPos == string::npos || pos < bestPos)) {
            bestPos = pos;
        }
    }
    
    if (bestPos == string::npos) {
        // No terms found, return beginning of content
        return content.substr(0, static_cast<size_t>(maxLength)) + "...";
    }
    
    // Extract snippet around the found term
    // Use size_t for both arguments to std::min to avoid type mismatch
    size_t startPos = (bestPos >= 50) ? bestPos - 50 : 0;
    size_t length = std::min(static_cast<size_t>(maxLength), content.length() - startPos);
    
    string snippet = content.substr(startPos, length);
   
    // Add ellipsis if needed
    if (startPos > 0) snippet = "..." + snippet;
    if (startPos + length < content.length()) snippet += "...";
    
    return snippet;
}
    // Auto-complete suggestions
    vector<string> getAutoCompleteSuggestions(const string& prefix, int maxSuggestions = 5) {
        vector<string> suggestions = getWordsWithPrefix(prefix);
        
        // Sort by frequency (stored in trie nodes)
        sort(suggestions.begin(), suggestions.end(), [&](const string& a, const string& b) {
            return getWordFrequency(a) > getWordFrequency(b);
        });
        
        if (suggestions.size() > static_cast<size_t>(maxSuggestions)) {
            suggestions.resize(static_cast<size_t>(maxSuggestions));
        }
        
        return suggestions;
    }
    
    // Get word frequency from trie
    int getWordFrequency(const string& word) {
        shared_ptr<TrieNode> current = trieRoot;
        
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return 0;
            }
            current = current->children[c];
        }
        
        return current->isEndOfWord ? current->frequency : 0;
    }
    
    // Add new document to index
    void addDocument(const string& title, const string& url, const string& content) {
        int newDocId = documents.size() + 1;
        documents.push_back(Document(newDocId, title, url, content));
        
        // Update index
        buildIndex();
        
        cout << "Document added successfully! Doc ID: " << newDocId << endl;
    }
    
    // Display search results
    void displaySearchResults(const vector<SearchResult>& results) {
        if (results.empty()) {
            cout << "No results found." << endl;
            return;
        }
        
        cout << "\n" << string(80, '=') << endl;
        cout << "SEARCH RESULTS" << endl;
        cout << string(80, '=') << endl;
        
        for (size_t i = 0; i < results.size(); i++) {
            const SearchResult& result = results[i];
            
            cout << "\n[" << (i + 1) << "] " << result.title << endl;
            cout << "URL: " << result.url << endl;
            cout << "Relevance Score: " << fixed << setprecision(4) << result.relevanceScore << endl;
            cout << "Matched Terms: ";
            for (size_t j = 0; j < result.matchedTerms.size(); j++) {
                cout << result.matchedTerms[j];
                if (j < result.matchedTerms.size() - 1) cout << ", ";
            }
            cout << endl;
            cout << "Snippet: " << result.snippet << endl;
            cout << string(80, '-') << endl;
        }
    }
    
    // Search statistics
    void displaySearchStatistics() {
        cout << "\n=== SEARCH ENGINE STATISTICS ===" << endl;
        cout << "Total Documents Indexed: " << documents.size() << endl;
        cout << "Total Unique Terms: " << invertedIndex.size() << endl;
        cout << "Trie Nodes Created: " << countTrieNodes(trieRoot) << endl;
        cout << "Average Document Length: " << calculateAverageDocLength() << " words" << endl;
        
        // Show top terms by frequency
        vector<pair<string, int>> termFreqs;
        for (const auto& [term, _] : invertedIndex) {
            termFreqs.push_back({term, getWordFrequency(term)});
        }
        
        sort(termFreqs.begin(), termFreqs.end(), 
             [](const auto& a, const auto& b) { return a.second > b.second; });
        
        cout << "\nTop 10 Most Frequent Terms:" << endl;
        for (int i = 0; i < min(10, (int)termFreqs.size()); i++) {
            cout << (i + 1) << ". " << termFreqs[i].first << " (" << termFreqs[i].second << ")" << endl;
        }
    }
    
    // Helper functions
    int countTrieNodes(shared_ptr<TrieNode> node) {
        if (!node) return 0;
        
        int count = 1;
        for (const auto& [_, child] : node->children) {
            count += countTrieNodes(child);
        }
        return count;
    }
    
    double calculateAverageDocLength() {
        if (documents.empty()) return 0.0;
        
        int totalWords = 0;
        for (const auto& doc : documents) {
            totalWords += doc.wordCount;
        }
        
        return double(totalWords) / documents.size();
    }
    
    // File operations
    void loadData() {
        // Implementation would load from files if they exist
        // For demo purposes, we'll use initialized data
    }
    
    void saveData() {
        // Save documents and index to files
        ofstream docFile(documentsFile);
        if (docFile.is_open()) {
            for (const auto& doc : documents) {
                docFile << doc.docId << "\n" << doc.title << "\n" 
                       << doc.url << "\n" << doc.content << "\n"
                       << doc.pageRank << "\n\n";
            }
            docFile.close();
        }
    }
    
    // Interactive search interface
    void interactiveSearch() {
        string query;
        
        while (true) {
            cout << "\n" << string(60, '=') << endl;
            cout << "ADVANCED SEARCH ENGINE" << endl;
            cout << string(60, '=') << endl;
            cout << "Commands:" << endl;
            cout << "  search <query>     - Search for documents" << endl;
            cout << "  suggest <prefix>   - Get auto-complete suggestions" << endl;
            cout << "  add                - Add new document" << endl;
            cout << "  stats              - Show search statistics" << endl;
            cout << "  rebuild            - Rebuild search index" << endl;
            cout << "  exit               - Exit the program" << endl;
            cout << "\nEnter command: ";
            
            getline(cin, query);
            
            if (query.empty()) continue;
            
            if (query == "exit") {
                break;
            } else if (query == "stats") {
                displaySearchStatistics();
            } else if (query == "rebuild") {
                buildIndex();
            } else if (query.substr(0, 6) == "search") {
                string searchQuery = query.substr(7); // Remove "search "
                if (!searchQuery.empty()) {
                    vector<SearchResult> results = search(searchQuery);
                    displaySearchResults(results);
                }
            } else if (query.substr(0, 7) == "suggest") {
                string prefix = query.substr(8); // Remove "suggest "
                if (!prefix.empty()) {
                    vector<string> suggestions = getAutoCompleteSuggestions(prefix);
                    cout << "Auto-complete suggestions for '" << prefix << "':" << endl;
                    for (size_t i = 0; i < suggestions.size(); i++) {
                        cout << (i + 1) << ". " << suggestions[i] << endl;
                    }
                }
            } else if (query == "add") {
                cout << "Enter document title: ";
                string title;
                getline(cin, title);
                
                cout << "Enter document URL: ";
                string url;
                getline(cin, url);
                
                cout << "Enter document content: ";
                string content;
                getline(cin, content);
                
                addDocument(title, url, content);
            } else {
                // Treat as direct search query
                vector<SearchResult> results = search(query);
                displaySearchResults(results);
            }
        }
    }
    
    void run() {
        cout << "Initializing Advanced Search Engine..." << endl;
        interactiveSearch();
        cout << "Thank you for using Advanced Search Engine!" << endl;
    }
};

int main() {
    SearchEngine engine;
    engine.run();
    return 0;
}
