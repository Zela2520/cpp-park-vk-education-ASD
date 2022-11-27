#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include "Huffman.h"

#define ORIGINAL_FILE "files/WaP/original"
#define COMPRESSED_FILE "files/WaP/compressed"
#define DECOMPRESSED_FILE "files/WaP/decompressed"

#include <algorithm>
#include <map>
#include <memory>
#include <utility>

#define ASCII_LENGTH 256
#define BITS_IN_BYTE 8
typedef unsigned char byte;

class VectorInputStream : public IInputStream {
public:
    explicit VectorInputStream() : pos(0) {
        printf("Max size of vector<byte> in VectorIS: %lu\n", vec.max_size());
    }

    bool Read(byte& value) override {
        if (pos < vec.size()) {
            value = vec[pos++];
            return true;
        }
        return false;
    }
    void Write(byte value) {
        vec.push_back(value);
    }

    std::vector<byte> vec;
    int pos;
};

class VectorOutputStream : public IOutputStream {
public:
    explicit VectorOutputStream() {
        printf("Max size of vector<byte> in VectorOS: %lu\n", vec.max_size());
    };

    void Write(byte value) override {
        vec.push_back(value);
    }

    std::vector<byte> vec;
};

class Huffman;

class BitInput {
public:
    explicit BitInput(VectorInputStream &_input) : stream(_input), buf(0), bit_pos(8) {}

    bool read_bit(byte &bit) {
        if ( bit_pos == BITS_IN_BYTE ) {
            bool res = stream.Read(buf);
            bit_pos = 0;
            if ( !res )
                return false;
        }
        bit = (buf >> (BITS_IN_BYTE - 1 - (bit_pos++))) & 1;
        return true;
    }
    byte read_byte() {
        byte letter = 0;
        byte bit;
        for (size_t i = 0; i < BITS_IN_BYTE; ++i) {
            read_bit(bit);
            if ( bit == 1 ) {
                letter |= 1 << (BITS_IN_BYTE - 1 - i);
            }
        }
        return letter;
    }

    friend Huffman;
private:
    VectorInputStream &stream;
    byte buf;
    int bit_pos;
};

class BitOutput {
public:
    explicit BitOutput(IOutputStream& _stream) : stream(_stream), buf(0), bit_count(0) {}

    void write_bit(byte bit) {
        buf |= (bit & 1) << (BITS_IN_BYTE - 1 - (bit_count++));
        if ( bit_count == BITS_IN_BYTE )
            flush();
    }
    void write_char(byte letter) {
        byte bit;
        for ( int i = BITS_IN_BYTE - 1; i >= 0; --i ) {
            bit = ( (letter & (1 << i)) ? '1' : '0' ); 
            write_bit(bit);
        }
    }
    void flush() {
        stream.Write(buf);
        buf = 0;
        bit_count = 0;
    }
    friend Huffman;
private:
    IOutputStream& stream;
    byte buf;
    int bit_count;
};

class Huffman {
public:
    Huffman(VectorInputStream &_stream_in, IOutputStream& _stream_out)
      : amount_of_symbols(0),
        counter(nullptr),
        unused_bits(0),
        bit_input(_stream_in),
        bit_output(_stream_out)
    {
        printf("Max size of vector<shared_ptr TreeNode> in Huffman: %lu\n", rate_table.max_size());
        printf("Max size of vector<shared_ptr TreeNode> in Huffman: %lu\n", backup_rate_table.max_size());
    }
    ~Huffman() {
        delete(counter);
    }
    struct TreeNode {
        byte letter;
        int frequency;
        std::shared_ptr<TreeNode> left;
        std::shared_ptr<TreeNode> right;

        explicit TreeNode(byte _letter,
                          int _frequency = 0,
                          std::shared_ptr<TreeNode> _left = nullptr,
                          std::shared_ptr<TreeNode> _right = nullptr)
          : letter(_letter),
            frequency(_frequency),
            left(std::move(_left)),
            right(std::move(_right)) {};

        bool operator>(const TreeNode &r) {
            return frequency > r.frequency;
        }
    };

    static bool compare_nodes(const std::shared_ptr<TreeNode> &l, const std::shared_ptr<TreeNode> &r) {
        return l > r;
    }

    void Encode() {
        AnalyzeInput();                     
        BuildTree();                        
        auto node = rate_table[0];
        BuildTable(node);                  
        unused_bits = CountUnusedBits();   
        EncodeTree(node);                  
        WritePayload();
    }
    void AnalyzeInput() {
        
        counter = new size_t[ASCII_LENGTH]();
        byte value;
        while ( bit_input.stream.Read(value) ) {
            if ( counter[value] == 0 ) {
                amount_of_symbols++;
            }
            counter[value]++; 
        }

        for ( size_t i = 0; i < ASCII_LENGTH; i++ ) {
            if ( counter[i] != 0 ) {
                rate_table.push_back(std::make_shared<TreeNode>(
                        i,
                        counter[i],
                        nullptr,
                        nullptr));
            }
        }
    }
    void BuildTree() {
    
        backup_rate_table = rate_table;

        std::sort(rate_table.begin(), rate_table.end(), compare_nodes);

        for ( size_t i = rate_table.size() - 1; rate_table.size() != 1; --i ) {
            auto temp_node = std::make_shared<TreeNode>(
                    0,                                  
                    rate_table[i]->frequency + rate_table[i - 1]->frequency,
                    rate_table[i],                   
                    rate_table[i - 1]);              
            rate_table.pop_back();
            rate_table.pop_back();

            rate_table.push_back(temp_node);

            size_t to_swap = rate_table.size() - 2;
            for ( ; to_swap != -1; --to_swap ) {
                if ( temp_node->frequency > rate_table[to_swap]->frequency ) {
                    rate_table[to_swap + 1].swap(rate_table[to_swap]);
                } else {
                    break;
                }
            }
        }
    }
    void BuildTable(const std::shared_ptr<TreeNode> &node) {
        static std::vector<bool> code;
        if ( node->left ) {
            code.push_back(0);
            BuildTable(node->left);
        }
        if ( node->right ) {
            code.push_back(1);
            BuildTable(node->right);
        }
        if ( !(node->left) && !(node->right) ) {
            new_codes.insert(std::pair<byte, std::vector<bool> >(node->letter, std::vector<bool>(code)));
        }
        if ( !code.empty() )
            code.pop_back();
    }
    int8_t CountUnusedBits() {
        int8_t bits;
        size_t tree_bits = amount_of_symbols * ( BITS_IN_BYTE + 1 ) + ( amount_of_symbols - 1 );

        uint64_t payload_bits = 0;
        for ( size_t i = 0; i < amount_of_symbols; ++i ) {
            payload_bits += (
                    backup_rate_table[i]->frequency
                    *
                    new_codes[backup_rate_table[i]->letter].size() );
        }

        bits = BITS_IN_BYTE - ( tree_bits + payload_bits ) % BITS_IN_BYTE;
        bit_output.stream.Write(bits);

        return bits;
    }
    void EncodeTree(const std::shared_ptr<TreeNode> &node) {
        if ( node->left && node->right ) {
            bit_output.write_bit(0);
            EncodeTree(node->left);
            EncodeTree(node->right);
        } else {
            bit_output.write_bit(1);
            bit_output.write_char(node->letter);
        }
    }
    void WritePayload() {
        bit_input.stream.pos = 0;
        byte value;
        while ( bit_input.stream.Read(value) ) {
            for ( size_t i = 0; i < new_codes[value].size(); ++i ) {
                bit_output.write_bit(int(new_codes[value][i]));
            }
        }
        bit_output.flush();
    }
    void Decode() {
        byte bit = 0;
        bit_input.stream.Read(bit);
        unused_bits = bit;

        auto root = DecodeTree();
        auto tmp = root;

        int16_t tree_bits = amount_of_symbols * ( BITS_IN_BYTE + 1 ) + ( amount_of_symbols - 1 );
        for ( size_t i = BITS_IN_BYTE + tree_bits; i < bit_input.stream.vec.size() * BITS_IN_BYTE - unused_bits; ++i ) {
            if ( !bit_input.read_bit(bit) )
                break;

            tmp = ( bit == 0 ? tmp->left : tmp->right );
            if ( !(tmp->left) && !(tmp->right) ) {
                bit_output.write_char(tmp->letter);
                tmp = root;
            }
        }
    }
    std::shared_ptr<TreeNode> DecodeTree() {
        byte bit;
        bit_input.read_bit(bit);
        if ( int(bit) == 0 ) {
            auto left_child = DecodeTree();
            auto right_child = DecodeTree();
            return std::make_shared<TreeNode>(0, 0, left_child, right_child);
        } else {
            byte letter = bit_input.read_byte();
            ++amount_of_symbols;
            return std::make_shared<TreeNode>(letter, 0, nullptr, nullptr);
        }
    }

private:
    std::map<byte, std::vector<bool> > new_codes;
    byte amount_of_symbols;
    int8_t unused_bits;
    std::vector<std::shared_ptr<TreeNode> > rate_table;
    std::vector<std::shared_ptr<TreeNode> > backup_rate_table;
    size_t* counter;

    BitInput bit_input;
    BitOutput bit_output;
};

static void copyStream(IInputStream &input, VectorInputStream &output) {
    byte value;
    while ( input.Read(value) )
        output.Write(value);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    VectorInputStream original_vector;
    copyStream(original, original_vector);

    Huffman hf(original_vector, compressed);
    hf.Encode();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    VectorInputStream compressed_vector;
    copyStream(compressed, compressed_vector);

    Huffman hf(compressed_vector, original);

    hf.Decode();
}


int main() {

    if ( (access(ORIGINAL_FILE, R_OK) & access(COMPRESSED_FILE, R_OK) & access(DECOMPRESSED_FILE, R_OK)) == -1 ) {
        fprintf(stderr, "Failed to open files!\n");
        return EXIT_FAILURE;
    }


    std::ifstream inf;
    inf.open(ORIGINAL_FILE, std::ifstream::out);
    VectorOutputStream temp_vec_output;

    char temp_char;
    byte temp_byte;
    while ( inf.get(temp_char) ) {
        temp_byte = byte(temp_char);
        temp_vec_output.Write( temp_byte );
    }
    inf.close();

    VectorInputStream vec_input;
    vec_input.vec = temp_vec_output.vec;
    VectorOutputStream vec_output;
    printf("---ENCODING---\n");
    Encode(vec_input, vec_output);

    std::ofstream outf;
    outf.open(COMPRESSED_FILE, std::ofstream::in | std::ofstream::trunc);

    VectorInputStream temp_vec_input;
    temp_vec_input.vec = vec_output.vec;
    while ( temp_vec_input.Read(temp_byte) ) {
        outf << temp_byte;
    }
    outf.close();

    inf.open(COMPRESSED_FILE, std::ifstream::out);
    temp_vec_output.vec.clear();

    while ( inf.get(temp_char) ) {
        temp_byte = byte(temp_char);
        temp_vec_output.Write( temp_byte );
    }
    inf.close();

    vec_input.vec.clear();
    vec_input.vec = temp_vec_output.vec;
    vec_input.pos = 0;
    vec_output.vec.clear();
    printf("\n\n---DECODING---\n");
    Decode(vec_input, vec_output);

    outf.open(DECOMPRESSED_FILE, std::ofstream::in | std::ofstream::trunc | std::ofstream::binary);

    temp_vec_input.vec.clear();
    temp_vec_input.vec = vec_output.vec;
    temp_vec_input.pos = 0;
    while ( temp_vec_input.Read(temp_byte) ) {
        outf << char(temp_byte);
    }
    outf.close();

    std::ifstream inf2;
    inf.open(ORIGINAL_FILE, std::ifstream::out);
    inf2.open(DECOMPRESSED_FILE, std::ifstream::out);

    inf.seekg(0, inf.end);
    inf2.seekg(0, inf2.end);
    int orig_length = inf.tellg();
    int decomp_length = inf2.tellg();
    inf.seekg(0, inf.beg);
    inf2.seekg(0, inf2.beg);

    if ( orig_length != decomp_length ) {
        printf("Files aren't the same length!\n");
    } else {
        printf("Files are the same length\n");
    }
    printf("\tOriginal file: %d\n"
           "\tDecompressed file: %d\n",
           orig_length, decomp_length);

    char orig;
    char decomp;
    while ( inf.get(orig) && inf.get(decomp) ) {
        if ( orig != decomp ) {
            printf("Files aren't identical!\n"
                   "\tORIG: %d - %c\n"
                   "\tDECO: %d - %c\n",
                   int(inf.tellg()), orig, int(inf2.tellg()), decomp);
            break;
        }
    }

    inf.close();
    inf2.close();

}
