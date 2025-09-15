# 🚀 Project 2: Advanced Search Engine in C++

> A fully functional **Mini Search Engine** built from scratch in C++  
> featuring **TF-IDF ranking, PageRank, Trie-based autocomplete, and snippets**.

---

## 📌 Overview
This project simulates how a real search engine works — from indexing documents to ranking them based on relevance.  
It uses **TF-IDF** and **PageRank** to score results, and a **Trie** for auto-suggestions.

---

## ✨ Features
- 🔎 **Full-Text Search** – Search through multiple documents.
- 📊 **TF-IDF + PageRank Ranking** – More relevant results appear first.
- ⌨️ **Auto-Complete Suggestions** – Get instant search term suggestions.
- 📜 **Smart Snippets** – Shows the matching part of the text.
- ➕ **Add New Documents** – Dynamically update the search index.
- 📈 **Search Statistics** – Tracks total searches and top terms.

---

## 🛠 Tech Stack
- **Language:** C++
- **Data Structures:** Trie, HashMap, Graph
- **Algorithms:** TF-IDF, Simplified PageRank, Prefix Search (DFS)

---

## ⚡ How It Works
1. **Indexing:** Loads all documents and removes stopwords.
2. **Ranking:** Calculates TF-IDF score + PageRank to rank results.
3. **Query Processing:** Breaks user query into tokens and matches them.
4. **Suggestions:** Uses Trie to offer prefix-based auto-complete.
5. **Output:** Displays top results with snippet and relevance score.

---

## ▶️ Usage
### Compile:
```bash
g++ main.cpp -o search_engine
