#include "crypt.h"

void WordsInfo::setParametres(char *pointer, int len, int encr) {
	word = (char*)malloc(len * sizeof(char) + 1);
	for (int i = 0; i < len; ++i)
		word[i] = *pointer++;
	word[len] = '\0';
	length = len;
	encrypted = encr;
}

int WordsInfo::getLength(void) {
	return length;
}

int WordsInfo::getEncrypted(void) {
	return encrypted;
}

char* WordsInfo::getWord(void) {
	return word;
}

Cryptogram::Cryptogram() {
	FILE *file;
	int i = 0;
	char symbol;
	file = fopen("cryptogram.txt", "r");
	if (file == NULL) { 
		printf("cryptogram.txt opening error\n"); 
		system("pause");
		exit(-1); 
	}
	fseek(file, 0, SEEK_END);
	cryptogramSize = ftell(file);
	cryptogram = (char*)malloc(cryptogramSize + 1);
	fseek(file, 0, SEEK_SET);
	while (fscanf(file, "%c", &symbol) != EOF)
	{
		cryptogram[i] = symbol;
		++i;
	}
	cryptogramSize = i;
	cryptogram[i - 1] = '\0';
	fclose(file);
}

char* Cryptogram::getCryptogram(void) {
	return cryptogram;
}

int Cryptogram::getCryptogramSize(void) {
	return cryptogramSize;
}

void CryptoanalyseInstrument::showCryptogram(void) {
	printf("%s", myCryptogram.getCryptogram());
}

void Cryptogram::saveCryptogram(void) {
	FILE *ready;
	ready = fopen("ready.txt", "w");
	for (int i = 0; i < cryptogramSize; i++)
		fprintf(ready, "%c", cryptogram[i]);
	fclose(ready);
}

CryptoanalyseInstrument::CryptoanalyseInstrument() {
	maxLength = maxEncrypted = 0;

	int i, j, temp_alph;
	float temp = 0.0, count = 0.0;
	char *tmp_crypt = myCryptogram.getCryptogram();
	for (j = 0; j < myCryptogram.getCryptogramSize(); ++j)
	{
		for (i = 0; i < ALPH_SIZE; i++)
			if (distributionAphabet[i] == tmp_crypt[j])
			{
				frequency[i] += 1.0;
				count++;
				continue;
			}
	}
	for (i = 0; i < ALPH_SIZE; ++i)
	{
		frequency[i] = frequency[i] / count;
	}
	for (i = 1; i < ALPH_SIZE; i++)
	{
		for (j = 0; j < ALPH_SIZE - i; j++)
		{
			if (frequency[j] < frequency[j + 1]) {
				temp = frequency[j];
				frequency[j] = frequency[j + 1];
				frequency[j + 1] = temp;
				temp_alph = distributionAphabet[j];
				distributionAphabet[j] = distributionAphabet[j + 1];
				distributionAphabet[j + 1] = temp_alph;
			}
		}
	}

	mainCicle();
}

void CryptoanalyseInstrument::wordsAnalysis(void) {
	char *letters = NULL;
	int length = 0, encrypted = 0, flag = 0;
	char *tmp_crypt = myCryptogram.getCryptogram();
	if (!wordsInfoList.empty()) deleteWordsInfo();
	for (int i = 0; i < myCryptogram.getCryptogramSize(); ++i)
	{
		if ((tmp_crypt[i] >= 'À' && tmp_crypt[i] <= 'ß') || (tmp_crypt[i] >= 'à' && tmp_crypt[i] <= 'ÿ') || (tmp_crypt[i] >= 'a' && tmp_crypt[i] <= 'z'))
		{
			if (flag == 0) {
				letters = &tmp_crypt[i];
				flag = 1;
			}
			length++;
			for (int j = 0; j < 33; j++)
			{
				if (tmp_crypt[i] == alphabet[j]) { encrypted++; continue; }
			}
		}
		else
		{
			if (length != 0) addNewWordsInfo(letters, length, encrypted);
			if (length > maxLength) maxLength = length;
			if (encrypted > maxEncrypted) maxEncrypted = encrypted;
			length = 0;
			encrypted = 0;
			flag = 0;
			letters = NULL;
		}
	}
}

void CryptoanalyseInstrument::addNewWordsInfo(char *letters, int length, int encrypted) {
	WordsInfo newWI;
	newWI.setParametres(letters, length, encrypted);
	wordsInfoList.push_back(newWI);
}

void CryptoanalyseInstrument::deleteWordsInfo(void) {
	wordsInfoList.clear();
}

void CryptoanalyseInstrument::showWordsLength(void) {
	maxLength = 0;
	wordsAnalysis();
	for (int i = maxLength; i > 0; i--) {
		for (list<WordsInfo>::iterator iter = wordsInfoList.begin();
			iter != wordsInfoList.end();
			++iter) {
			if (iter->getLength() == i)
				printf("%d: %s\n", iter->getLength(), iter->getWord());
		}
	}
}

void CryptoanalyseInstrument::showWordsEncrypted(void) {
	maxEncrypted = 0;
	wordsAnalysis();
	for (int i = maxEncrypted; i > 0; i--) {
		for (list<WordsInfo>::iterator iter = wordsInfoList.begin();
			iter != wordsInfoList.end();
			++iter) {
			if (iter->getEncrypted() == i)
				printf("%d: %s\n", iter->getEncrypted(), iter->getWord());
		}
	}
}

void CryptoanalyseInstrument::showAnalysisResult(void) {
	for (int i = 0; i < ALPH_SIZE; ++i)
	{
		printf("%c - %f - %c\n", distributionAphabet[i], frequency[i], rusLettersDistribution[i]);
	}
}

void CryptoanalyseInstrument::doReplacement(void) {
	char *tmp_crypt = myCryptogram.getCryptogram();
	char from, to;
	printf("Replace from ");
	cin >> from;
	printf("to ");
	cin >> to;
	addNewReplacement(from, to);
	for (int i = 0; i < myCryptogram.getCryptogramSize(); i++) {
		if (tmp_crypt[i] == from) tmp_crypt[i] = to;
	}
}

void CryptoanalyseInstrument::addNewReplacement(char letfirst, char letsecond) {
	Replacement newRep;
	newRep.setParametres(letfirst, letsecond);
	replacementList.push_back(newRep);
}

void Replacement::setParametres(char from, char to) {
	letterFrom = from;
	letterTo = to;
}

char Replacement::getLetFrom(void) {
	return letterFrom;
}

char Replacement::getLetTo(void) {
	return letterTo;
}

void CryptoanalyseInstrument::showReplacementHistory(void) {
	if (replacementList.empty()) {
		cout << "Replacement's history is empty" << endl;
		return;
	}
	for (list<Replacement>::iterator iter = replacementList.begin();
		iter != replacementList.end(); ++iter)
		cout << iter->getLetFrom() << " changed to " << iter->getLetTo() << endl;
}

void CryptoanalyseInstrument::doRollbackFromLastReplacement(void) {
	if (replacementList.empty()) {
		cout << "Rollback is impossible - no replacements!" << endl;
		return;
	}
	char from, to;
	//list<Replacement>::iterator lastReplacement = replacementList.end();
	Replacement lastReplacement = replacementList.back();
	from = lastReplacement.getLetTo();
	to = lastReplacement.getLetFrom();
	replacementList.pop_back();
	char *tmp_crypt = myCryptogram.getCryptogram();
	for (int i = 0; i < myCryptogram.getCryptogramSize(); i++) {
		if (tmp_crypt[i] == from) tmp_crypt[i] = to;
	}
}

void CryptoanalyseInstrument::doAutoReplacement(void) {
	char *tmp_crypt = myCryptogram.getCryptogram();
	for (int i = 0; i < myCryptogram.getCryptogramSize(); i++)
		for (int j = 0; j < ALPH_SIZE; j++)
			if (tmp_crypt[i] == distributionAphabet[j])
				tmp_crypt[i] = rusLettersDistribution[j];
}

int CryptoanalyseInstrument::menu(void) {
	int n = 0;
	printf("\nChoose your action: \n"
		"1. Show result of frequency analysis.\n"
		"2. Show current cryptogram.\n"
		"3. Show list of words grouped by the letters count.\n"
		"4. Show list of words grouped by the encrypted letters count.\n"
		"5. Do letters replacement in cryptogram.\n"
		"6. Show replacements history.\n"
		"7. Do rollback from last replacement.\n"
		"8. Do frequency autoreplacement.\n"
		"9. Save cryptogram.\n"
		"0. Exit.\n"
		"Your action >> ");
	scanf("%d", &n);
	return n;
}

void CryptoanalyseInstrument::mainCicle(void) {
	bool working = true;
	int button = menu();
	while (working)
	{
		switch (button)
		{
		case 1: showAnalysisResult();
			break;
		case 2: showCryptogram();
			break;
		case 3: showWordsLength();
			break;
		case 4: showWordsEncrypted();
			break;
		case 5: doReplacement();
			break;
		case 6: showReplacementHistory();
			break;
		case 7: doRollbackFromLastReplacement();
			break;
		case 8: doAutoReplacement();
			break;
		case 9: myCryptogram.saveCryptogram();
			break;
		case 0: working = false;
			break;
		}
		if (working == true) button = menu();
	}
}