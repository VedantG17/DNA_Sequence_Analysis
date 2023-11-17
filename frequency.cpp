#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

using namespace std;

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
};

struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

unordered_map<char, int> calculateFrequencies(const string& data) {
    unordered_map<char, int> frequencies;
    for (char symbol : data) {
        frequencies[symbol]++;
    }
    return frequencies;
}

HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;

    for (auto& pair : frequencies) {
        HuffmanNode* node = new HuffmanNode;
        node->symbol = pair.first;
        node->frequency = pair.second;
        node->left = nullptr;
        node->right = nullptr;
        minHeap.push(node);
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* internalNode = new HuffmanNode;
        internalNode->symbol = '\0';
        internalNode->frequency = left->frequency + right->frequency;
        internalNode->left = left;
        internalNode->right = right;
        minHeap.push(internalNode);
    }

    return minHeap.top();
}

void displayFrequencies(const unordered_map<char, int>& frequencies) {
    cout << "Symbol Frequencies:" << endl;
    for (const auto& pair : frequencies) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main() {
    ifstream inputFile("genome_data.txt");
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open the file." << endl;
        return 1;
    }

    char ch;
    string genomeData;
    while (inputFile.get(ch)) {
        genomeData += ch;
    }

    inputFile.close();

    // Filter out unwanted characters (non-A, T, C, G, a, t, c, g)
    string filteredGenomeData;
    for (char symbol : genomeData) {
        if (symbol == 'A' || symbol == 'T' || symbol == 'C' || symbol == 'G' ||
            symbol == 'a' || symbol == 't' || symbol == 'c' || symbol == 'g') {
            filteredGenomeData += symbol;
        }
    }

    // Calculate symbol frequencies for filtered data
    unordered_map<char, int> frequencies = calculateFrequencies(filteredGenomeData);

    // Display symbol frequencies
    displayFrequencies(frequencies);

    // Build Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Your remaining code for Huffman codes, compressed data, and writing to a file
    
    return 0;
}