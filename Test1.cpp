#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>



class TapeDevice {
public:
	virtual bool Read(int& value) = 0;
	virtual bool Write(int value) = 0;
	virtual bool Shift() = 0;
	virtual bool Seek(int pos) = 0;
	virtual int GetPosition() = 0;
	virtual unsigned long long GetLength() = 0;
};

class FileTapeDevice : public TapeDevice {
public:
	FileTapeDevice(const std::string& filename, int delay = 0) : file_(filename), delay_(delay) {
		file_.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	}

	~FileTapeDevice() {
		file_.close();
	}


	bool Read(int& value) override {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
		return file_.read(reinterpret_cast<char*>(&value), sizeof(int)).gcount() == sizeof(int);
		
	}

	bool Write(int value) override {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
		return file_.write(reinterpret_cast<char*>(&value), sizeof(int)).good();
	}

	bool Seek(int pos) override {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
		return file_.seekg(pos * sizeof(int)).good() && file_.seekp(pos * sizeof(int)).good();
	}

	bool Shift() override {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
		return file_.seekg(0 * sizeof(int), std::ios::beg).good() && file_.seekp(0 * sizeof(int), std::ios::beg).good();
	}
	bool Right() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
		return file_.seekg(1 * sizeof(int), std::ios::cur).good() && file_.seekp(1 * sizeof(int), std::ios::cur).good();
	}


	int GetPosition() override {
		return file_.tellg()/sizeof(int);
	}

    unsigned long long GetLength() override {
        auto pos = file_.tellg();
        file_.seekg(0, std::ios::end);
        auto size = file_.tellg()/sizeof(int);
        file_.seekg(pos);
        return size;
    }

	
protected:
	//std::string fileName;
	int delay_;
	int pos = 0;
	std::fstream file_;

};


class Sort {
public:
	Sort(const std::string& input_filename, const std::string& output_filename, int delay)
		: input_filename_(input_filename), output_filename_(output_filename), delay_(delay) {}

	 bool separetion() {

		int firstElement = 0;
		bool flag = true;
		bool sort = true ;
		bool first_a = true;
		bool first_t = true;

 		for (std::size_t i = 0; i < input_.GetLength(); i++) {
			int secElement=0;
			input_.Read(secElement);
			if (i != input_.GetLength())
				input_.Right();
			if (firstElement > secElement) {
				flag = !flag;
				sort = false;
			}
			bool first = flag ? first_a : first_t;
			if (!(first)) {
				if (flag) file1.Right(); else file2.Right();
			}

			if (flag) file1.Write(secElement); else file2.Write(secElement);
			first = false;
			firstElement = secElement;
		}
		file1.Shift();
		file2.Shift();
		input_.Shift();
		output_.Shift();
		return sort;
	}

	void merge() {

		int element1 = 0, element2 = 0;
		if (file1.GetLength() != 0);
			file1.Read(element1);
		if (file2.GetLength() != 0);
			file2.Read(element1);

			while (file1.GetPosition()!=file1.GetLength() || file2.GetPosition() != file2.GetLength())
			{
				if ((element1 > element2) && file1.GetPosition() != file1.GetLength() || file2.GetPosition() == file2.GetLength()) {
					output_.Write(element1);
					if (file1.GetPosition() != file1.GetLength())
						file1.Read(element1);
				}
				else {
					output_.Write(element2);
					if (file2.GetPosition() != file2.GetLength())
						file2.Read(element2);
				}

			}
			file1.Shift();
			file2.Shift();
			input_.Shift();
			output_.Shift();
	}

	void sort() {
		while (true) {
			bool sort = separetion();
			if (sort) break;
			merge();
		 }
	}

	void printFile() {
		int element = 0;
		while (output_.GetPosition() != output_.GetLength()) {
			output_.Read(element);
			std::cout << element << " ";
		}

	}
protected:

	std::string input_filename_;
	std::string output_filename_;
	int delay_;
	std::string file1_ = "tmp_file1.txt";
	std::string file2_ = "tmp_file2.txt";
	FileTapeDevice input_ { input_filename_, delay_ };
	FileTapeDevice output_ { output_filename_, delay_ };
	FileTapeDevice file1 { file1_, delay_ };
	FileTapeDevice file2 { file2_, delay_ };
	
};

void main() {
	std::cout << "Inter input file name: ";
	std::string inputFileName;
	std::cin >> inputFileName;
	std::cout << std::endl;
	std::cout << "Inter output file name: ";
	std::string outputFileName;
	std::cin >> outputFileName;

	Sort Sortrovka(inputFileName, outputFileName, 1);
	Sortrovka.sort();
	Sortrovka.printFile();

	system("pause");
}

