#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

//Take a double pointer. First delete it's children/nodes. Then delete the parent pointer.
void DeallocateMemory(char** matrix, const int& rows)
{
	char** upperBoundAddressRow = matrix + rows;

	for(char** i = matrix; i < upperBoundAddressRow; i++)
	{
		delete *i;
	}

	delete[] matrix;

	return;
}

//Iterate a simple loop that outputs all the arrays stored in the double pointer
void displayTokens(char** charString, int rows)
{
	for(int i = 0; i < rows; i++)
	{
		cout << charString[i];
		cout << endl;
	}

	return;
}

//Iterate a simple loop till the null ('\0') character is found. Return the counter
//Doesn't work if tstring isn't null terminated (cstring)
int sizeOfString(char* str)
{
	int stringLen = 0;
	for (char* count = str; *count != '\0'; count++)
	{
		stringLen++;
	}

	return stringLen;
}

//Takes a char string. Creates a new char pointer of bufferSize + 1 (for null character)
//Deep Copies all elements of Buffer to the newly created pointer. Returns the Pointer
char* GetStringFromBuffer(char* buffer)
{
	int strSize = sizeOfString(buffer);
	char* str = 0;

	if (strSize > 0)
	{
		str = new char[strSize+1];
		char* dataDestination = str;

		for (char* dataSource = buffer; *dataSource != '\0'; dataSource++, dataDestination++)
		{
			*dataDestination = *dataSource;
		}

		*dataDestination = '\0';
	}

	return str;
}

//Takes two char Strings. Create a third char string the size of array1+array2, + 1 space for null character (to prevent garbage printing)
//Copy all elements of array1 to array3. Then copy all elements of array2 to array3
//Assign address of array3 to array1, so we don't have to return anything.
void StringConcatenate(char*& str1, char* str2)
{
	int sizeStr1 = sizeOfString(str1);
	int sizeStr2 = sizeOfString(str2);
	int sizeStr3 = sizeStr1 + sizeStr2 + 1;

	char* str3 = new char[sizeStr3];

	int counter = 0;
	for(; counter < sizeStr1; counter++)
	{
		str3[counter] = str1[counter];
	}

	for(int i = 0; i < sizeStr2; i++)
	{
		str3[counter++] = str2[i];
	}

	str3[counter] = '\0';


	delete[] str1;
	str1 = str3;

	return;
}

//Takes a char String. Iterates a lopp till its length, whenever a space is encountered, increase the word counter by one. Return the word counter integer
//This function assumes that all the words are separated by a space (other punctuations don't work)
int numberOfTokens(char* charString)
{
	int numberOfTokens = 0, strSize = sizeOfString(charString);

	for(int i = 0; i < strSize; i++)
	{
		if(charString[i] == ' ')
		{
			numberOfTokens++;
		}
	}
	numberOfTokens += 1;
	return numberOfTokens;
}

//Takes a char string. Calculates the number of words it has (using the numberOfTokens function).
//Create a double pointer the size of the number of words. Iterate a loop till the end of the charString and store each word into a tempBuffer
//Find the size of that tempBuffer and add a pointer to the double pointer the size of the word + 1 (for null character)
//Copy all elements from tempBuffer to the newly created pointer. Repeat this process for the whole string. Return the double pointer containg all the words.
char** StringTokens(char* charString)
{
	int tokenSize = numberOfTokens(charString);

	int tempCounter = 0, charStringCounter = 0;
	char** strTokens = new char*[tokenSize];
	char tempBuffer[40];

	for(int i = 0; i < tokenSize; i++)
	{
		tempCounter = 0;

		for(; charString[charStringCounter] != ' ' && charString[charStringCounter] != '\0'; charStringCounter++)
		{
			tempBuffer[tempCounter++] = charString[charStringCounter];
		}

		charStringCounter++;

		strTokens[i] = new char[tempCounter + 1];

		strTokens[i][tempCounter] = '\0';

		for(int k = 0; k < tempCounter; k++)
		{
			strTokens[i][k] = tempBuffer[k];
		}

	}

	return strTokens;
}

//Takes a char String. Pass this string to StringToken function that returns all the individual words
//Use a simple loop to reverse the array (replacing current element with the element from the end of the array)
//Return the reversed Tokens array
char** InverseStringTokens(char* charString)
{
	int tokenSize = numberOfTokens(charString);
	char** tokenArray = StringTokens(charString);
	char* temp;

	for(int i = 0, j = tokenSize - 1; i < tokenSize/2; i++, j--)
	{
		temp=tokenArray[i];
		tokenArray[i]=tokenArray[j];
		tokenArray[j]=temp;
	}

	return tokenArray;

}

//Takes a char string, an integer array containing the indices where a space has to be placed, and the length of the sentence (including the spaces)
//Iterate a loop till the stringLength, and compare the iterator with the values of SpaceIndex. Once an index has been found where the space has to be placed:
//1. Move all characters one place to the right
//2. And put a space where it's suppoesd to be (according to the spaceIndex vlaue)
void insertSpacesIntoSentence(char*& charString, int* spaceIndex, int stringLength)
{
	int spaceIndexCounter = 0;
	for(int i = 0; i < stringLength; i++)
	{
		if(i == spaceIndex[spaceIndexCounter])
		{

			for(int j = stringLength; j >= i ; j--)
			{
				charString[j] = charString[j-1];
			}

			charString[spaceIndex[spaceIndexCounter] - 1] = ' ';
			spaceIndexCounter++;
		}

	}
	return;
}

//Takes a string that has to be reversed. Passes it to the InverseStringTokens function so we can have all the words in the reverse order
//Concatinate all the reverse tokens, one by one, with an empty string dynamically allocated on the heap
//Calcuate the size of each word and place it inside an integer array. Pass that array to insertSpacesIntoSentence, so a space can be added between words
char* ReverseSentence(char* charString)
{
	int numberOfWords = numberOfTokens(charString), stringLength = sizeOfString(charString);
	char** reverseTokens = InverseStringTokens(charString);
	char* revSentence = new char[stringLength + 1];
	revSentence[0] = '\0';

	for(int i = 0; i < numberOfWords; i++)
	{
		StringConcatenate(revSentence, reverseTokens[i]);
	}


	int* positionOfSpace = new int[numberOfWords - 1];

	for(int i = 0; i < numberOfWords - 1; i++)
	{
		positionOfSpace[i] = sizeOfString(reverseTokens[i]) + 1;

		if(i != 0)
		{
			positionOfSpace[i] += positionOfSpace[i-1];
		}
	}

	insertSpacesIntoSentence(revSentence, positionOfSpace, stringLength);

	revSentence[stringLength] = '\0';

	return revSentence;
}


int main()
{
	ifstream inputObj("Data.txt");
	if(!inputObj)
	{
		cout << "Data.txt couldn't be loaded, please verify the location of the file\n";
	}
	else
	{

		int tokenNumber;
		char tempBuffer[80];

		cout << "\n\n\t\t\t\tTesting StringConcatenate:\n\n";

		inputObj.getline(tempBuffer, 80);
		cout << "String 1: \"" << tempBuffer << "\"" << endl;
		char* str1 = GetStringFromBuffer(tempBuffer);

		inputObj.getline(tempBuffer, 80);
		cout << "String 2: \"" << tempBuffer << "\"" << endl;
		char* str2 = GetStringFromBuffer(tempBuffer);

		inputObj.close();

		StringConcatenate(str1, str2);

		cout << "\n\nAfter Concatination\n\n";
		cout << "String 1: \"" << str1 << "\"" << endl;
		cout << "String 2: \"" << str2 << "\"" << endl;


		cout << "\n\n------------------------------------------------------------------------------------------------------------\n\n\n\n";

		cout << "\t\t\t\tTesting StringTokens\n\n";
		cout << "Tokens of String 1 are as follows:\n\n\n";
		char** tokenHolder = StringTokens(str1);
		tokenNumber = numberOfTokens(str1);
		displayTokens(tokenHolder, tokenNumber);
		DeallocateMemory(tokenHolder, tokenNumber);


		cout << "\n\n------------------------------------------------------------------------------------------------------------\n\n\n\n";
		cout << "\t\t\t\tTesting InverseStringTokens\n\n";
		cout << "Tokens of the string in reverse order are as follows:\n\n\n";
		char** reverseTokens = InverseStringTokens(str1);
		displayTokens(reverseTokens, tokenNumber);
		DeallocateMemory(reverseTokens, tokenNumber);


		cout << "\n\n------------------------------------------------------------------------------------------------------------\n\n\n\n";
		cout << "\t\t\t\tTesting Reverse Sentence\n\n";
		char* revSentence = ReverseSentence(str1);
		cout << "Reverse Sentence of String 1 is: \"" << revSentence << "\"" <<"\n\n";
		cout << "\n\n------------------------------------------------------------------------------------------------------------\n";
	}


	system("pause>0");
}
