#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

class AhoCorasick {
private:
    struct TrieNode {
        unordered_map<char, int> children;
        vector<string> output;
        int fail = 0;
    };

    vector<TrieNode> trie;

public:
    AhoCorasick(const vector<string>& words) {
        trie.emplace_back(); // Root node
        buildTrie(words);
        buildFailureLinks();
    }

    void buildTrie(const vector<string>& words) {
        for (const string& word : words) {
            int node = 0;
            for (char ch : word) {
                if (trie[node].children.find(ch) == trie[node].children.end()) {
                    trie[node].children[ch] = trie.size();
                    trie.emplace_back();
                }
                node = trie[node].children[ch];
            }
            trie[node].output.push_back(word);
        }
    }

    void buildFailureLinks() {
        queue<int> q;
        for (auto& p : trie[0].children) {
            trie[p.second].fail = 0;
            q.push(p.second);
        }

        while (!q.empty()) {
            int curr = q.front(); q.pop();
            for (auto& p : trie[curr].children) {
                char ch = p.first;
                int nextNode = p.second;
                q.push(nextNode);

                int failNode = trie[curr].fail;
                while (failNode && trie[failNode].children.find(ch) == trie[failNode].children.end()) {
                    failNode = trie[failNode].fail;
                }
                trie[nextNode].fail = trie[failNode].children.count(ch) ? trie[failNode].children[ch] : 0;
                trie[nextNode].output.insert(trie[nextNode].output.end(), trie[trie[nextNode].fail].output.begin(), trie[trie[nextNode].fail].output.end());
            }
        }
    }

    vector<pair<int, string>> search(const string& text) {
        vector<pair<int, string>> foundPatterns;
        int node = 0;
        for (int i = 0; i < text.size(); i++) {
            while (node && trie[node].children.find(text[i]) == trie[node].children.end()) {
                node = trie[node].fail;
            }
            node = trie[node].children.count(text[i]) ? trie[node].children[text[i]] : 0;
            for (const string& pattern : trie[node].output) {
                foundPatterns.emplace_back(i - pattern.size() + 1, pattern);
            }
        }
        return foundPatterns;
    }
};

int main() {
    vector<string> patterns = {"he", "she", "his", "hers"};
    string text = "ahishers";
    AhoCorasick aho(patterns);
    vector<pair<int, string>> matches = aho.search(text);

    for (const auto& match : matches) {
        cout << "Pattern '" << match.second << "' found at index " << match.first << endl;
    }
    return 0;
}
