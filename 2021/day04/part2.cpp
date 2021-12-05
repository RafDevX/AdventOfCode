#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int mark_number_for_card(vector<vector<vector<int>>> *cards, int num, int cardi)
{
	for (int rowi = 0; rowi < (*cards)[cardi].size(); rowi++) {
		bool checkThisRow = false;
		bool incompleteRow = false;
		for (int i = 0; i < (*cards)[cardi][rowi].size(); i++) {
			if ((*cards)[cardi][rowi][i] == num) {
				(*cards)[cardi][rowi][i] = -1;
				checkThisRow = true;
				int incompleteCol = false;
				for (int j = 0; j < (*cards)[cardi].size(); j++) {
					if ((*cards)[cardi][j][i] >= 0) {
						incompleteCol = true;
						break;
					}
				}
				if (!incompleteCol) {
					return cardi;
				}
			} else if ((*cards)[cardi][rowi][i] >= 0) {
				incompleteRow = true;
			}
		}
		if (checkThisRow && !incompleteRow) { // line
			return cardi;
		}
	}
	return -1;
}

vector<int> mark_number(vector<vector<vector<int>>> *cards, int num)
{
	vector<int> ret;
	for (int cardi = 0; cardi < (*cards).size(); cardi++) {
		int winner = mark_number_for_card(cards, num, cardi);
		if (winner >= 0) {
			ret.push_back(cardi);
		}
	}
	return ret;
}

int sum_unmarked(vector<vector<int>> card)
{
	int sum = 0;
	for (vector<int> row : card) {
		for (int num : row) {
			if (num >= 0) {
				sum += num;
			}
		}
	}
	return sum;
}

void handle_winner(vector<vector<vector<int>>> *cards, int winner, int num)
{
	if ((*cards).size() > 1) {
		(*cards).erase((*cards).begin() + winner);
	} else {
		int unmarked = sum_unmarked((*cards)[winner]);
		cout << "Card: #" << winner + 1 << "\n";
		cout << "Unmarked: " << unmarked << "\n";
		cout << "Number: " << num << "\n";
		cout << "Score: " << unmarked * num << "\n";
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		cout << "Run with text file as argument" << endl;
		return 1;
	}
	fstream file;
	file.open(argv[1], ios::in);
	if (file.is_open()) {
		string line;
		vector<int> nums;
		vector<vector<vector<int>>> cards;
		int row, col;
		bool firstLine = true;
		while (getline(file, line)) {
			stringstream ss(line);
			if (firstLine) {
				for (int i; ss >> i;) {
					nums.push_back(i);
					if (ss.peek() == ',') {
						ss.ignore();
					}
				}
				firstLine = false;
			} else if (line == "") {
				cards.push_back(vector(5, vector(5, 0)));
				row = 0;
				col = 0;
			} else {
				// oh god i hate how wet this whole thing is
				while (ss.peek() == ' ') {
					ss.ignore();
				}
				for (int i; ss >> i;) {
					cards[cards.size() - 1][row][col] = i;
					if (++col >= 5) {
						col = 0;
						row++;
					}
					while (ss.peek() == ' ') {
						ss.ignore();
					}
				}
			}
		}
		for (int num : nums) {
			vector<int> winners = mark_number(&cards, num);
			for (int winner : winners) {
				handle_winner(&cards, winner, num);
			}
			// for (int i = 0; i < cards.size(); i++) {
			// 	bool stop = false;
			// 	for (int j = 0; j < cards[i].size(); j++) {
			// 		for (int k = 0; k < cards[i][j].size(); k++) {
			// 			if (cards[i][j][k] >= 0) {
			// 				stop = true;
			// 				break;
			// 			}
			// 		}
			// 		if (stop)
			// 			break;
			// 	}
			// 	if (!stop) { // no clue why this isn't being caught by handle_winner
			// 		cout << "[" << i << "],";
			// 		cards.erase(cards.begin() + i);
			// 	}
			// }
		}
		cout << "No winner found\n";
		for (auto card : cards) {
			for (auto line : card) {
				for (auto num : line) {
					cout << "|" << num;
				}
				cout << "|\n---\n";
			}
			cout << "\n===\n";
		}
	}
	return 0;
}