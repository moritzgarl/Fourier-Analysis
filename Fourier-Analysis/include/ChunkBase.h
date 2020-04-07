#pragma once
#include <fstream>
#include <array>
#include <vector>

using FourCC = std::array<char, 4>;

class ChunkIt;

class Chunk {
public:
	std::ifstream& m_ifs;
	std::streampos m_pos;

public:
	Chunk(std::ifstream& ifs, std::streampos pos);

	FourCC id() const;
	std::uint32_t size() const;
	FourCC type() const;
	ChunkIt begin(bool no_chunk_id = false);
	ChunkIt end();
	std::vector<char> data();
};


class ChunkIt {
private:
	std::ifstream& m_ifs;
	std::streampos m_pos;
public:
	ChunkIt(std::ifstream& stream, std::streampos pos);

	Chunk operator*();
	bool operator==(const ChunkIt& it) const;
	bool operator!=(const ChunkIt& it) const;
	ChunkIt& operator ++ ();
	ChunkIt operator ++ (int);
};