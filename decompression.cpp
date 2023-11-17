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

void decompressHuffman(HuffmanNode* root, const string& compressedData, ofstream& outputFile) {
    HuffmanNode* current = root;

    for (char bit : compressedData) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            // Leaf node reached, output the symbol
            outputFile << current->symbol;
            current = root;  // Reset to the root for the next iteration
        }
    }
}

int main() {
    ifstream compressedFile("compressed_genome_data.txt", ios::binary);
    if (!compressedFile.is_open()) {
        cout << "Error: Unable to open the compressed file." << endl;
        return 1;
    }

    string compressedData;
    char bit;
    while (compressedFile.get(bit)) {
        compressedData += bit;
    }

    compressedFile.close();

    // Calculate symbol frequencies for filtered data
    unordered_map<char, int> frequencies = calculateFrequencies(compressedData);

    // Your existing code to build Huffman tree and display frequencies
    // ...

    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Your remaining code for Huffman codes, compressed data, and writing to a file
    // ...

    // Decompress and write to file
    ofstream decodedFile("decoded_data.txt");
    if (!decodedFile.is_open()) {
        cout << "Error: Unable to create the output file for decoded data." << endl;
        return 1;
    }

    decompressHuffman(root, compressedData, decodedFile);

    decodedFile.close();

    cout << "Data decompressed and saved to 'decoded_data.txt'." << endl;

    return 0;
}
