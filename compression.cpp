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

unordered_map<char, string> generateHuffmanCodes(HuffmanNode* root, string code = "") {
    unordered_map<char, string> huffmanCodes;
    if (root) {
        if (root->symbol != '\0') {
            huffmanCodes[root->symbol] = code;
        }
        unordered_map<char, string> left_codes = generateHuffmanCodes(root->left, code + "0");
        unordered_map<char, string> right_codes = generateHuffmanCodes(root->right, code + "1");
        for (auto& pair : left_codes) {
            huffmanCodes[pair.first] = pair.second;
        }
        for (auto& pair : right_codes) {
            huffmanCodes[pair.first] = pair.second;
        }
    }
    return huffmanCodes;
}

HuffmanNode* buildHuffmanTree(unordered_map<char, int>& frequencies) {
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
    unordered_map<char, int> frequencies;
    for (char symbol : filteredGenomeData) {
        frequencies[symbol]++;
    }

    // Display symbol frequencies
    cout << "Symbol Frequencies:" << endl;
    for (auto& pair : frequencies) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Build Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Generate Huffman codes
    unordered_map<char, string> huffmanCodes = generateHuffmanCodes(root);

// Encode genome data using Huffman codes
string encodedGenomeData;
for (char symbol : genomeData) {
    encodedGenomeData += huffmanCodes[symbol];
}

// Display Huffman codes and compressed data
cout << "\nHuffman Codes:" << endl;
for (auto& pair : huffmanCodes) {
    cout << pair.first << ": " << pair.second << endl;
}

// Display compressed data
cout << "\nCompressed Data: " << encodedGenomeData << endl;

// Write encoded data to file
ofstream outputFile("compressed_genome_data.txt", ios::binary);
if (!outputFile.is_open()) {
    cout << "Error: Unable to create the output file." << endl;
    return 1;
}

outputFile << encodedGenomeData;

outputFile.close();

    // Clean up the Huffman tree (optional)
    // Implement cleanup function to delete nodes in the tree

   cout << "Genome data compressed and saved to 'compressed_genome_data.txt'." << endl;

    // Decode the data if necessary (using the Huffman tree)

    return 0;
}