#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;

// Hàm tính khoảng cách Levenshtein giữa hai chuỗi
int levenshteinDistance(const string& a, const string& b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
        }
    }

    return dp[m][n];
}

// Hàm sao chép nội dung vào clipboard
void copyToClipboard(const string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hMem) {
            memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
    }
}

int main() {
    string input;
    getline(cin, input); // Nhập chuỗi tìm kiếm

    vector<int> spacePositions;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            spacePositions.push_back(i);
        }
    }

    string uppercaseInput;
    getline(cin, uppercaseInput);
    stringstream ss(uppercaseInput);
    vector<char> uppercaseLetters;
    char letter;
    while (ss >> letter) {
        uppercaseLetters.push_back(letter);
    }

    if (!uppercaseLetters.empty()) {
        input[0] = uppercaseLetters[0];
    }

    for (size_t i = 1; i < uppercaseLetters.size() && i - 1 < spacePositions.size(); i++) {
        int pos = spacePositions[i - 1] + 1;
        if (pos < input.size()) {
            input[pos] = uppercaseLetters[i];
        }
    }

    cout << "Chuỗi: " << input << endl;

    ifstream file("C:\\Users\\bichn\\OneDrive\\Desktop\\rimi.csv");
    if (!file.is_open()) {
        cerr << "Không thể mở tệp." << endl;
        return 1;
    }

    string line;
    vector<pair<string, int>> matches;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ',')) {
            int distance = levenshteinDistance(input, cell);
            matches.push_back({cell, distance});
        }
    }

    file.close();

    auto bestMatch = min_element(matches.begin(), matches.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second < b.second;
    });

    cout << "Chuỗi: " + bestMatch->first + "\nLevenshtein: " + to_string(bestMatch->second);

	string result = ".claim " + bestMatch->first;
    // Lưu vào clipboard
    copyToClipboard(result);
    cout << "\nClipboard!" << endl;

    return 0;
}
