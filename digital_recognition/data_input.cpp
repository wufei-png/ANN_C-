
#include <iostream>

#include "data_input.h"
#include <typeinfo>

dataInput::dataInput()
:
	ImageStartPos(16),
	LableStartPos(8)
{
}

dataInput::~dataInput()
{
	LabelFile.close();
	ImageFile.close();
}


bool dataInput::openLabelFile(const char* url,int i) {
    LabelFile.open(url, ios::binary | ios::in);
    LabelFile.seekg(LableStartPos);
    return true;
}
bool dataInput::openImageFile(const char* url,int i){
    ImageFile.open(url, ios::binary | ios::in);
    ImageFile.seekg(ImageStartPos);
    return true;
}

bool dataInput::readLabel(char label[])
{
	if (LabelFile.is_open() && !LabelFile.eof())
	{
		LabelFile.read(label, 1);
        return true;
	}
	return false;
}

bool dataInput::readImage(char Buf[])
{
	if (ImageFile.is_open() && !ImageFile.eof())
	{
		ImageFile.read(Buf, 28*28);
		return true;
	}
	return false;
}

bool dataInput::read(char label[], char Buf[])
{
    return readLabel(label)&&readImage(Buf);
}

void dataInput::reset()
{
	if (ImageFile.is_open())
	{
		ImageFile.clear();
		ImageFile.seekg(ImageStartPos);
	}

	if (LabelFile.is_open())
	{
		LabelFile.clear();
		LabelFile.seekg(LableStartPos);
	}
}