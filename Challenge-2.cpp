#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int ALPHABET_SIZE = 26;

struct TrieNode
{
	TrieNode* children[ALPHABET_SIZE];
	bool isEndOfWord;

};

//Ham tao new node
TrieNode* createNode()
{
	TrieNode* newNode = new TrieNode;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		newNode->children[i] = NULL;
	}

	newNode->isEndOfWord = false;

	return newNode;
}

//Ham chen chuoi key vao cay tien to
void insertNode(TrieNode* root, string key)
{
	TrieNode* nodeTemp = root;

	for (int i = 0; i < key.length(); i++)
	{
		int next = key[i] - 97;

		if (!nodeTemp->children[next])
		{
			nodeTemp->children[next] = createNode();
		}

		nodeTemp = nodeTemp->children[next];
	}

	nodeTemp->isEndOfWord = true;
}

//Ham tao cay tien to
void createTrie(TrieNode*& root, string filename)
{
	ifstream fin(filename, ios::in);

	if (!fin.is_open())
	{
		return;
	}

	string line;
	while (getline(fin, line))
	{
		insertNode(root, line);
	}

	fin.close();
}

//Ham tim kiem tu trong cay tien to
bool searchKey(TrieNode* root, string key)
{
	TrieNode* nodeTemp = root;

	for (int i = 0; i < key.length(); i++)
	{
		int next = key[i] - 97;

		if (!nodeTemp->children[next])
			return false;

		nodeTemp = nodeTemp->children[next];
	}
	
	return nodeTemp->isEndOfWord;
}

//Ham tach khoang trang
string split(string line)
{
	string key="";
	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] != 32)
		{
			key = key + line[i];
		}
	}
	return key;
}

//Ham xuat ra ket qua
void outPutWordValid(vector<string> listWord)
{
	listWord.push_back("0");
	int count = 0;
	for (int i = 0; i < listWord.size() - 1; i++)
		if (listWord[i] != listWord[i+1])
				count++;
	cout << count << endl;
	for (int i = 0; i < listWord.size() - 1; i++)
		if (listWord[i] != listWord[i+1])
			cout << listWord[i] << endl;
}
//Tim tat ca sau con hoan vi cua chuoi co trong root
// Đệ quy quay lui
void findSubString(TrieNode* root, TrieNode* temp, string& s, string& key, vector<string>& listWord)
{
	// string s là chuỗi để xét
	// string key là kết quả chuỗi con
	// string stemp là chuỗi tạm để lưu
    string stemp;
	TrieNode* prevTemp; // node để duyệt vị trí trên cây
    for (int i = 0; i < s.length(); i++)
    {
		if (temp->children[s[i] - 97] != NULL)
		{
			prevTemp = temp; // lưu lại địa chỉ node hiện tại
			stemp = s;  // lưu lại s ban đầu để thay đổi cấu trúc s
			temp = temp->children[s[i]-97]; //đi tới node tiếp theo để tìm các trường hợp có thể xảy ra
			key += s[i]; // cộng kí tự vào chuỗi con        

			// nếu từ có độ dài lớn hơn 2 có trong từ điển thì thêm vào vector
			if (temp->isEndOfWord && key.length() > 2) listWord.push_back(key); 

			s.erase(i,1); // Xóa kí tự s[i]
			findSubString(root, temp, s, key, listWord); //gọi lại hàm để tìm chuỗi con của các kí tự còn lại
			s = stemp; // reset lại chuỗi s để xét kí tự tiếp theo
			temp = prevTemp; // trở về node hiện tại
			key.pop_back(); // xóa kí tự cuối của key
		}
    }
}

//Nhap danh sach cac ki tu
void inputWord(TrieNode* root)
{
	vector<string> listWordValid; //Danh sách chuỗi con hoán vị của input
	string key = ""; //biến để chứa chuỗi con
	string line; //biến để đọc input
	getline(cin, line);
	line = split(line);
	findSubString(root, root, line, key, listWordValid);
	sort(listWordValid.begin(), listWordValid.end()); //Sắp xếp danh sách để kiểm tra chuỗi trùng lặp
	outPutWordValid(listWordValid); //Xuất danh sách
}
int main()
{
	TrieNode* root = createNode();
	createTrie(root,"Dic.txt");
	inputWord(root);
}