#include "ChunkBase.h"


Chunk::Chunk(std::ifstream& ifs, std::streampos pos)
	: m_ifs(ifs), m_pos(pos)
{}

FourCC Chunk::id() const {
	FourCC read_data;
	m_ifs.seekg(m_pos);
	m_ifs.read(read_data.data(), read_data.size());
	return read_data;
}

uint32_t Chunk::size() const {
	uint32_t read_data;
	std::streamoff offset(4);
	m_ifs.seekg(m_pos + offset);
	m_ifs.read(reinterpret_cast<char*>(&read_data), 4);
	return read_data;
}

FourCC Chunk::type() const {
	FourCC read_data;
	std::streamoff offset(8);
	m_ifs.seekg(m_pos + offset);
	m_ifs.read(read_data.data(), read_data.size());
	return read_data;
}

ChunkIt Chunk::begin(bool no_chunk_id) {
	// Offset of 12 Bytes to jump over first chunkId, chunkSize and chunkType (each 4 Bytes)
	std::streamoff offset(no_chunk_id ? 8 : 12);
	return ChunkIt(m_ifs, m_pos + offset);
}

ChunkIt Chunk::end() {
	std::uint32_t sz = size();
	std::streamoff offset(sz + 8);	// +8 comes from the fact that the size is chunk size in bytes - 8
	return ChunkIt(m_ifs, m_pos + offset);
}

std::vector<char> Chunk::data() {
	std::uint32_t sz = size();
	std::vector<char> data_buffer;
	data_buffer.resize(sz);

	std::streamoff offset(8);
	m_ifs.seekg(m_pos + offset);
	m_ifs.read(data_buffer.data(), sz);
	
	return data_buffer;
}

ChunkIt::ChunkIt(std::ifstream& stream, std::streampos pos)
	: m_ifs(stream), m_pos(pos)
{}

Chunk ChunkIt::operator*() {
	return Chunk(m_ifs, m_pos);
}

bool ChunkIt::operator == (const ChunkIt& it) const {
	return (m_pos == it.m_pos);
}

bool ChunkIt::operator != (const ChunkIt& it) const {
	return !(*this == it);
}


ChunkIt& ChunkIt::operator ++ () {
	Chunk ch(m_ifs, m_pos);
	uint32_t sz = ch.size();
	std::streampos offset(sz + 8);

	m_pos += offset;
	return *this;
}

ChunkIt ChunkIt::operator++(int)
{
	ChunkIt it(m_ifs, m_pos);

	Chunk ch(m_ifs, m_pos);
	uint32_t sz = ch.size();
	std::streampos offset(sz + 8);

	m_pos += offset;
	return it;
}
