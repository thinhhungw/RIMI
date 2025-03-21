#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <sstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

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

// Hàm chuyển đổi mẫu đầu vào thành regex
string convertToRegex(const string &pattern) {
    string regexPattern = "^";
    for (char c : pattern) {
        if (c == '-') regexPattern += '.'; // '-' đại diện cho bất kỳ ký tự nào
        else regexPattern += c; // Giữ nguyên chữ cái
    }
    regexPattern += "$";
    return regexPattern;
}

int main() {
    string filename = "C:/Users/bichn/OneDrive/Desktop/rimi.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Không thể mở file!" << endl;
        return 1;
    }
    
    string pattern;
    getline(cin, pattern);
    regex regexPattern(convertToRegex(pattern));
    
    string line;
    vector<string> results;
    
    while (getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos == string::npos) continue; // Bỏ qua nếu không có dấu phẩy
        
        string word = line.substr(commaPos + 1);
        word.erase(0, word.find_first_not_of(" \t")); // Loại bỏ khoảng trắng đầu
        word.erase(word.find_last_not_of(" \t") + 1); // Loại bỏ khoảng trắng cuối
        
        if (regex_match(word, regexPattern)) {
            results.push_back(word);
        }
    }
    file.close();
    
    // In kết quả
    for (const string &word : results) {
        cout << word << endl;
    }
    
    // Sao chép từ đầu tiên nếu có kết quả
    if (!results.empty()) {
        copyToClipboard(".claim " + results[0]);
        cout << "Copied to clipboard: .claim " << results[0] << endl;
    } else {
        cout << "Không tìm thấy từ phù hợp!" << endl;
    }
    
    return 0;
}