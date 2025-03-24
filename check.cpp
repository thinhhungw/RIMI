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

// Hàm xóa ký tự ẩn (zero-width, khoảng trắng không mong muốn)
string cleanString(const string &s) {
    string result;
    for (char c : s) {
        if (!isspace(c) && c > 31) { // Loại bỏ ký tự điều khiển và khoảng trắng không mong muốn
            result += c;
        }
    }
    return result;
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
    pattern = cleanString(pattern); // Xóa ký tự ẩn trước khi dùng regex

    // Debug: In mã ASCII của input để kiểm tra ký tự lạ
    cout << "Pattern nhập vào: " << pattern << endl;
    cout << "Mã ASCII của từng ký tự: ";
    for (char c : pattern) {
        cout << int(c) << " ";
    }
    cout << endl;

    regex regexPattern(convertToRegex(pattern));
    
    string line;
    vector<string> results;
    
    while (getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos == string::npos) continue; // Bỏ qua nếu không có dấu phẩy
        
        string word = line.substr(commaPos + 1);
        word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end()); // Xóa khoảng trắng dư thừa
        word = cleanString(word); // Xóa ký tự ẩn

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
