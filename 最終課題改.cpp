#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
using namespace std;

// Wordクラス
class Word {
private:
    string word;
    string meaning;

public:
    Word(string word, string meaning) : word(word), meaning(meaning) {}

    string getWord() const {
        return word;
    }

    string getMeaning() const {
        return meaning;
    }
};

// WordListクラス：英単語のリストを管理するクラス
class WordList {
private:
    vector<Word> words;
    string filename; // ファイル名

public:
    // コンストラクタ
    WordList(string filename) :filename(filename) {
        // ファイルから単語リストを読み込む
        bool success = loadFromFile();
        if (!success) {
            cout << "ファイルを開けませんでした： " << filename << "\n";
        }
    }

    // デストラクタ
    ~WordList() {
        // プログラム終了時に単語リストをファイルに保存する
        saveToFile();
    }

    // 単語をリストに追加する関数
    bool addWord(Word word) {
        words.push_back(word);
        return true;
    }

    // 指定された単語をリストから削除する関数
    bool removeWord(string word) {
        for (auto it = words.begin(); it != words.end(); ++it) {
            if (it->getWord() == word) {
                words.erase(it);
                return true;
            }
        }
        return false;
    }

    // 単語リストを表示する関数
    void displayWordList() const {
        cout << "単語リスト：\n";
        for (auto word : words) {
            cout << word.getWord() << " - " << word.getMeaning() << "\n";
        }
        cout << "\n";
    }

    // ランダムな単語を出題する関数
    void quizRandomWord() const {
        if (words.empty()) {
            cout << "単語リストが空です。Quizを始める前に単語を追加してください。\n";
            return;
        }

        // 乱数生成
        random_device engine;
        mt19937 gen(engine());
        uniform_int_distribution<> dist(0, words.size() - 1);
        int randomIndex = dist(gen);

        const Word& randomWord = words[randomIndex];

        string userAnswer;
        cout << "Quiz：\"" << randomWord.getWord() << "\"の意味は何ですか？ ";
        cin.ignore(); 
        getline(cin, userAnswer);

        if (userAnswer == randomWord.getMeaning()) {
            cout << "正解！素晴らしいです！\n";
        }
        else {
            cout << "不正解です。正解は「" << randomWord.getMeaning() << "」です。\n";
        }
    }

    // ファイルから単語リストを読み込む関数
    bool loadFromFile() {
        ifstream file(filename);
        if (!file) {
            return false;
        }

        string word, meaning;
        while (file >> word >> meaning) {
            addWord(Word(word, meaning));
        }

        file.close();
        return true;
    }

    // 単語リストをファイルに保存する関数
    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cout << "ファイルを開けませんでした： " << filename << "\n";
            return;
        }

        for (const auto& word : words) {
            file << word.getWord() << " " << word.getMeaning() << "\n";
        }

        file.close();
    }

    // イテレータを取得する
    vector<Word>::iterator begin() {
        return words.begin();
    }

    vector<Word>::iterator end() {
        return words.end();
    }
};

int main() {
    cout << "こんにちは！英単語学修ツール Vocaです！\n" << "\n";

    // ファイル名を指定してWordListを作成
    WordList wordList("wordlist.txt"); 
    int choice;

    do {
        cout << "メニュー：\n";
        cout << "1. 単語を追加する\n";
        cout << "2. 単語を削除する\n";
        cout << "3. 単語リストを表示する\n";
        cout << "4. Quiz - ランダムに出題される単語の意味を当てる！\n";
        cout << "5. 終了\n";
        cout << "数字を選択してください: ";

        // 入力エラー処理
        if (!(cin >> choice)) {
            cout << "無効な入力です。全角になっていないでしょうか？数字を半角で入力してください。\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            string word, meaning;
            cout << "単語を入力してください： ";
            cin.ignore();
            getline(cin, word);
            cout << "意味を入力してください： ";
            getline(cin, meaning);

            if (wordList.addWord(Word(word, meaning))) {
                cout << "単語が追加されました。\n";
            }
            else {
                cout << "単語の追加に失敗しました。\n";
            }
            break;
        }
        case 2: {
            string word;
            cout << "削除する単語を入力してください： ";
            cin.ignore();
            getline(cin, word);

            if (wordList.removeWord(word)) {
                cout << "単語が削除されました。\n";
            }
            else {
                cout << "単語の削除に失敗しました。\n";
            }
            break;
        }
        case 3:
            wordList.displayWordList();
            break;
        case 4:
            wordList.quizRandomWord();
            break;
        case 5:
            cout << "プログラムを終了します。\n";
            break;
        default:
            cout << "無効な選択肢です。もう一度選んでください。\n";
        }

        cout << "\n";
    } while (choice != 5);

    return 0;
}
