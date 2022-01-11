

#ifndef __DATA_INPUT_H__
#define __DATA_INPUT_H__

#include <fstream>

using namespace std;

class dataInput
{
public:
	dataInput();
	~dataInput();
public:
	void reset();
	bool openLabelFile(const char* url,int i);
	bool openImageFile(const char* url,int i);
    bool readLabel(char label[]);
	bool readImage(char imageBuf[]);
    bool read(char label[], char imageBuf[]);


private:
	int ImageStartPos;
	int LableStartPos;
	fstream LabelFile;
	fstream ImageFile;
};

#endif // !__DATA_INPUT_H__

