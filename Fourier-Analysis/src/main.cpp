#include <iostream>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
#include <array>

#include "ChunkBase.h"

#define M_PI 3.14159265358979323846


//struct WAVE_RIFF {
//	struct RIFF {
//		char chunkID[4];
//		uint32_t chunkSize;
//		char riffType[4];
//	};
//
//	// Kennung "fmt"
//	struct FORMAT {
//		uint16_t wChannels;
//		uint32_t dwSamplesPerSec;
//		uint32_t dwAvgBytesPerSec;
//		uint16_t wBlockAlign;
//		uint16_t wBitsPerSample;
//
//		//union 
//	};
//
//	// Kennung "data"
//	struct DATA {
//
//	};
//
//	struct RIFF _RIFF;
//	struct FORMAT _FORMAT;
//	struct DATA _DATA;
//};


//template <typename T> class Signal 
//{
//public:
//	Signal():
//		num_samples(0)
//	{}
//
//	Signal(const int& samples)
//		: num_samples(samples)
//	{
//		data.reserve(num_samples);
//	}
//
//	Signal(const std::vector<T>& sig)
//		: data(sig)
//	{
//		num_samples = data.size();
//	}
//
//	int getNumberOfSamples() {
//		return num_samples;
//	}
//
//	const std::vector<T>& getSignal() {
//		return data;
//	}
//
//	const std::vector<T>& getSignal() const {
//		return data;
//	}
//
//	template <typename T> friend std::ostream& operator<<(std::ostream& out, const Signal<T>& signal);
//
//protected:
//	int num_samples;
//	std::vector<T> data;
//};
//
//template <typename T> std::ostream& operator<<(std::ostream& out, const Signal<T>& signal) {
//	for (T val : signal.data) {
//		out << val << " ";
//	}
//	return out;
//}
//
//
//template <typename T> class PeriodSignal
//	: public Signal<T> 
//{
//protected:
//	int m_freq;
//public:
//	PeriodSignal()
//		: Signal<T>(), m_freq(0)
//	{}
//
//	PeriodSignal(int samples, int frequency)
//		: Signal<T>(samples), m_freq(frequency)
//	{
//		initializeSignal();
//	}
//
//	void initializeSignal() {
//		for (int i = 0; i < this->num_samples; i++) {
//			this->data.push_back(std::sin(i * (2 * M_PI / (m_freq * this->num_samples))));
//		}
//	}
//};


/*
class FourierUtil {
	template <class T> static int analyseSignal(Signal<T> signal) {

	}

	static int getSpectrum(int dummy) {
		int dummy_ = 0;
		return dummy;
	}
};

double getFrequency(int time) {
	return 0.0f;
}
*/

std::vector<std::byte> load_file(const std::string& filepath) {
	std::ifstream ifs(filepath, std::ios::binary | std::ios::ate);

	//if (!ifs) {
	//	throw std::runtime_error(filepath + ": " + std::strerror(errno));
	//}

	auto end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	auto size = std::size_t(end - ifs.tellg());

	if (size == 0) {
		return {};
	}

	std::vector<std::byte> buffer(size);

	if (!ifs.read((char*)buffer.data(), buffer.size())) {
		//throw std::runtime_error(filepath + ": " + std::strerror(errno));
	}

	return buffer;
}



int main() {
	std::string filepath = "C:/Dev/StarWars3.wav";
	std::ifstream readFromFile("C:/Dev/StarWars3.wav", std::ios::binary);

	Chunk ch(readFromFile, std::ios::beg);

	auto id = ch.id();
	auto size = ch.size();
	auto type = ch.type();
	std::cout << "Chunk id: " << std::string(id.data(), 4) << std::endl;
	std::cout << "Chunk size: " << size << std::endl;;
	std::cout << "Chunk type: " << std::string(type.data(), 4) << std::endl;

	std::vector<char> data;
	uint16_t data_format;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t bytesPerSecond;
	uint16_t block_align;
	uint16_t bitsPerSample;

	if (type == std::array<char, 4>{'W', 'A', 'V', 'E'}) {
		for (auto sch : ch) {
			auto sub_id = sch.id();
			auto sub_size = sch.size();
			std::cout << "Chunk id: " << std::string(sub_id.data(), 4) << std::endl;
			std::cout << "Chunk size: " << sub_size << std::endl;
			if (sub_id == std::array<char, 4>{'d', 'a', 't', 'a'}) {
				data = sch.data();
			}
			else if (sub_id == std::array<char, 4>{'f', 'm', 't', ' '}) {
				//std::fpos pos = sch
				std::streampos offset(8);
				sch.m_ifs.seekg(sch.m_pos + offset);
				sch.m_ifs.read(reinterpret_cast<char*>(&data_format), sizeof(data_format));
				sch.m_ifs.read(reinterpret_cast<char*>(&channels), sizeof(channels));
				sch.m_ifs.read(reinterpret_cast<char*>(&sample_rate), sizeof(sample_rate));
				sch.m_ifs.read(reinterpret_cast<char*>(&bytesPerSecond), sizeof(bytesPerSecond));
				sch.m_ifs.read(reinterpret_cast<char*>(&block_align), sizeof(block_align));
				sch.m_ifs.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));
			}
		}
	}

	std::cout << "data_format: " << data_format << "\n";
	std::cout << "channels: " << channels << "\n";
	std::cout << "sample_rate: " << sample_rate << "\n";
	std::cout << "bytes/second: " << bytesPerSecond << "\n";
	std::cout << "block_align: " << block_align << "\n";
	std::cout << "bits/sample: " << bitsPerSample << "\n";

	std::ofstream writeToFile("C:/Dev/test_file", std::ios::out | std::ios::binary);
	writeToFile.write((const char*)data.data(), data.size());

	auto result = load_file(filepath);

	std::cin.get();
}