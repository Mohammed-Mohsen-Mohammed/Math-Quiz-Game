#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

enum enQuestionsLevel { Easy = 1, Med = 2, Hard = 3, Mix = 4 };

enum enOperationType { Add = 1, Sub = 2, Mul = 3, Div = 4, MixOp = 5 };

struct stQuestion
{
	int Number1 = 0;
	int Number2 = 0;
	enQuestionsLevel QuestionLevel;
	enOperationType OperationType;
	int CorrectAnswer = 0;
	int PlayerAnswer = 0;
	bool AnswerResult = false;
};

struct stQuiz
{
	stQuestion QuestionList[100];
	short NumberOfQuestions;
	enQuestionsLevel QuestionsLevel;
	enOperationType OperationType;
	int NumberOfRightAnswers = 0;
	int NumberOfWrongAnswers = 0;
	bool IsPass = false;
};

enQuestionsLevel ReadQuestionsLevel() {
	short QuestionLevel;

	do {
		cout << "\nEnter question level [ (1) Easy, (2) Med, (3) Hard, (4) Mix ]? ";
		cin >> QuestionLevel;
	} while (QuestionLevel < 1 || QuestionLevel > 4);

	return enQuestionsLevel(QuestionLevel);
}

enOperationType ReadOperationType() {
	short OperationType;

	do {
		cout << "\nEnter operation Type [ (1) Add, (2) Sub, (3) Mul, (4) Div, (5) Mix ]? ";
		cin >> OperationType;
	} while (OperationType < 1 || OperationType > 5);

	return enOperationType(OperationType);
}

int RandomNumber(int From, int To) {

	return rand() % (To - From + 1) + From;
}

int SimpleCalculator(int Number1, int Number2, enOperationType OpType) {
	switch (OpType)
	{
	case enOperationType::Add:
		return Number1 + Number2;
	case enOperationType::Sub:
		return Number1 - Number2;
	case enOperationType::Mul:
		return Number1 * Number2;
	case enOperationType::Div:
		return (Number2 != 0) ? (Number1 / Number2) : 0;
	default:
		return Number1 + Number2;
	}
}

stQuestion GenerateQuestion(enQuestionsLevel QuestionLevel, enOperationType OpType) {
	stQuestion Question;

	if (QuestionLevel == enQuestionsLevel::Mix)
		QuestionLevel = enQuestionsLevel(RandomNumber(1, 3));

	if (OpType == enOperationType::MixOp)
		OpType = enOperationType(RandomNumber(1, 4));

	Question.OperationType = OpType;
	Question.QuestionLevel = QuestionLevel;

	switch (QuestionLevel)
	{
	case enQuestionsLevel::Easy:
		Question.Number1 = RandomNumber(1, 10);
		Question.Number2 = RandomNumber(1, 10);
		break;
	case enQuestionsLevel::Med:
		Question.Number1 = RandomNumber(10, 100);
		Question.Number2 = RandomNumber(10, 100);
		break;
	case enQuestionsLevel::Hard:
		Question.Number1 = RandomNumber(100, 1000);
		Question.Number2 = RandomNumber(100, 1000);
	}
	Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
	return Question;
}

void GenerateQuizQuestions(stQuiz& Quiz) {
	for (short i = 0; i < Quiz.NumberOfQuestions; i++)
	{
		Quiz.QuestionList[i] = GenerateQuestion(Quiz.QuestionsLevel, Quiz.OperationType);
	}
}

string GetQuestionLevelText(enQuestionsLevel QuestionLevel) {
	string arrQuestionLevelText[] = { "Easy","Med","Hard","Mix" };
	return arrQuestionLevelText[QuestionLevel - 1];
}

string GetOpTypeSymbol(enOperationType OpType) {
	string arrOpTypeSymbol[] = { "+","-","*","/","Mix" };
	return arrOpTypeSymbol[OpType - 1];
}

void PrintQuestion(const stQuiz& Quiz, short QuestionNumber) {
	cout << "\nQuestion [" << QuestionNumber + 1 << "/" << Quiz.NumberOfQuestions << "]\n\n";
	cout << Quiz.QuestionList[QuestionNumber].Number1 << " ";
	cout << GetOpTypeSymbol(Quiz.QuestionList[QuestionNumber].OperationType) << " ";
	cout << Quiz.QuestionList[QuestionNumber].Number2 << endl;
	cout << "----------------\n";
}

int ReadPlayerAnswer() {
	int Answer;
	cin >> Answer;
	return Answer;
}

void SetScreenColor(bool Right) {
	if (Right)
		system("color 2F");
	else
	{
		system("color 4F");
		cout << "\a";
	}
}

void CorrectTheQuestionAnswer(stQuiz& Quiz, short QuestionNumber) {
	if (Quiz.QuestionList[QuestionNumber].PlayerAnswer == Quiz.QuestionList[QuestionNumber].CorrectAnswer) {
		
		Quiz.QuestionList[QuestionNumber].AnswerResult = true;
		Quiz.NumberOfRightAnswers++;

		cout << "Right Answer :-)\n";
	}
	else
	{
		Quiz.QuestionList[QuestionNumber].AnswerResult = false;
		Quiz.NumberOfWrongAnswers++;

		cout << "Wrong Answer :-(\n";
		cout << "The right answer is: " << Quiz.QuestionList[QuestionNumber].CorrectAnswer << endl;
	}

	SetScreenColor(Quiz.QuestionList[QuestionNumber].AnswerResult);
}

void AskAndCorrectQuestionListAnswer(stQuiz& Quiz) {
	for (short i = 0; i < Quiz.NumberOfQuestions; i++)
	{
		PrintQuestion(Quiz, i);

		Quiz.QuestionList[i].PlayerAnswer = ReadPlayerAnswer();

		CorrectTheQuestionAnswer(Quiz, i);
	}

	Quiz.IsPass = (Quiz.NumberOfRightAnswers >= Quiz.NumberOfWrongAnswers);
}

int ReadHowManyQuestions() {
	short NumberOfQuestions;

	do {
		cout << "How many questions do you want to answer? ";
		cin >> NumberOfQuestions;
	} while (NumberOfQuestions < 1 || NumberOfQuestions>100);

	return NumberOfQuestions;
}

string GetFinalResultsText(bool Pass) {
	return Pass ? "PASS :-)" : "Fail :-(";
}

void PrintQuizResults(const stQuiz& Quiz) {
	cout << "\n----------------------------\n";
	cout << "Final Results is " << GetFinalResultsText(Quiz.IsPass);
	cout << "\n----------------------------\n\n";

	cout << "Number of Questions    : " << Quiz.NumberOfQuestions << endl;
	cout << "Questions Level        : " << GetQuestionLevelText(Quiz.QuestionsLevel) << endl;
	cout << "OpType                 :[" << GetOpTypeSymbol(Quiz.OperationType) << "]" << endl;
	cout << "Number of Right Answers: " << Quiz.NumberOfRightAnswers << endl;
	cout << "Number of Wrong Answers: " << Quiz.NumberOfWrongAnswers << endl;
	cout << "----------------------------\n";

}

void PlayMathGame() {
	stQuiz Quiz;

	Quiz.NumberOfQuestions = ReadHowManyQuestions();
	Quiz.QuestionsLevel = ReadQuestionsLevel();
	Quiz.OperationType = ReadOperationType();

	GenerateQuizQuestions(Quiz);
	AskAndCorrectQuestionListAnswer(Quiz);
	SetScreenColor(Quiz.IsPass);
	PrintQuizResults(Quiz);
}

void ResetScreen() {
	system("cls");
	system("color 0F");
}

void StartGame() {
	char playAgain;

	do {

		ResetScreen();
		PlayMathGame();
		
		cout << "\nDo you want to play again? [Y/N] ";
		cin >> playAgain;
	} while (playAgain == 'y' || playAgain == 'Y');

}

int main() {
	srand(unsigned(time(nullptr)));

	StartGame();

	return 0;
}