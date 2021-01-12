#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <list>
#define ALPH_SIZE 32

using namespace std;

class Replacement {
private:
	char letterFrom;
	char letterTo;

public:
	void setParametres(char from, char to);
	char getLetFrom(void);
	char getLetTo(void);
};

class WordsInfo {
private:
	char *word;
	int length;
	int encrypted;

public:
	void setParametres(char *pointer, int len, int encr);
	char* getWord();
	int getLength(void);
	int getEncrypted(void);
};

class Cryptogram {
private:
	char *cryptogram;
	int cryptogramSize;
	
public:
	Cryptogram();
	char* getCryptogram(void);
	int getCryptogramSize(void);
	void saveCryptogram(void);
};

class CryptoanalyseInstrument {
private:
	const string alphabet = "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
	const string rusLettersDistribution = "îåàèíòñğâëêìäïóÿûüãçá÷éõæøşöùıôú";
	string distributionAphabet = "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
	float frequency[ALPH_SIZE] = { 0 };
	int maxLength;
	int maxEncrypted;
	list<Replacement> replacementList;
	list<WordsInfo> wordsInfoList;
	Cryptogram myCryptogram;

	int menu(void);
	void wordsAnalysis(void);
	void addNewWordsInfo(char *letters, int length, int encrypted);
	void deleteWordsInfo(void);
	void addNewReplacement(char letfirst, char letsecond);

public:
	CryptoanalyseInstrument();
	void mainCicle(void);
	void showCryptogram(void);
	void showAnalysisResult(void);
	void showWordsLength(void);
	void showWordsEncrypted(void);
	void doReplacement(void);
	void showReplacementHistory(void);
	void doRollbackFromLastReplacement(void);
	void doAutoReplacement(void);
};
