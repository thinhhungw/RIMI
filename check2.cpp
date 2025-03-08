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

// Hàm kiểm tra chuỗi có đúng định dạng viết hoa không
bool isUppercaseFormatCorrect(const string& input, const string& match) {
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != ' ' && ((isupper(input[i]) && !isupper(match[i])) || (islower(input[i]) && !islower(match[i])))) {
            return false;
        }
    }
    return true;
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

    cout << "Chuỗi sau thay thế: " << input << endl;

    ifstream file("C:\\Users\\bichn\\OneDrive\\Desktop\\rimi.csv");
    if (!file.is_open()) {
        cerr << "Không thể mở tệp." << endl;
        return 1;
    }

    string line;
    vector<pair<string, int>> matches;
    int minDistance = INT_MAX;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ',')) {
            int distance = levenshteinDistance(input, cell);
            matches.push_back({cell, distance});
            minDistance = min(minDistance, distance);
        }
    }

    file.close();

    cout << "Các chuỗi có khoảng cách Levenshtein nhỏ nhất (" << minDistance << "):" << endl;

    vector<string> bestMatches;
    for (const auto& match : matches) {
        if (match.second == minDistance && isUppercaseFormatCorrect(input, match.first)) {
            bestMatches.push_back(match.first);
        }
    }

    // Sắp xếp theo số lượng ký tự giảm dần
    sort(bestMatches.begin(), bestMatches.end(), [](const string& a, const string& b) {
        return a.size() > b.size();
    });

    for (const string& match : bestMatches) {
        cout << match << endl;
    }

    // Ghép các chuỗi đã sắp xếp thành một chuỗi dán vào clipboard
    if (!bestMatches.empty()) {
        string clipboardText = ".claim " + bestMatches[0];
        for (size_t i = 1; i < bestMatches.size(); i++) {
            clipboardText += "\n.claim " + bestMatches[i];
        }
        copyToClipboard(clipboardText);
        cout << "\nClipboard!" << endl;
    }

    return 0;
}
