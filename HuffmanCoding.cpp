#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <utility>
#include <sstream>
#include <bitset>

struct TreeNode {
	char ch;
	int fre;
	TreeNode* left;
	TreeNode* right;
	TreeNode() :ch('#'), fre(0), left(nullptr), right(nullptr) {}
	TreeNode(char ch1, int fre1) :ch(ch1), fre(fre1), left(nullptr), right(nullptr) {}
};
class Huffman {
public:
	void constructTree(std::string& str) {
		std::cout << "constructing huffman tree..." << std::endl;
		for (int i = 0; i<str.length(); i++)
			charmap[str[i]]++;
		auto mycomp = [&](const TreeNode* lhs, TreeNode* rhs) {
			return lhs->fre > rhs->fre;
		};
		typedef std::priority_queue<TreeNode*, std::vector<TreeNode*>,
			decltype(mycomp)> minheap;
		minheap myheap(mycomp);
		std::cout << "char frequence:\n";
		for (auto &x : charmap) {
			std::cout << x.first << " and " << x.second << std::endl;
			myheap.push(new TreeNode(x.first, x.second));
		}
		//TreeNode* tmp11 = myheap.top();
		//std::cout << tmp11->ch << " and " << tmp11->fre << std::endl;
		if (myheap.size() == 1) {
			root = new TreeNode();
			root->left = myheap.top();
			root->right = new TreeNode();
			root->fre = root->left->fre;
			myheap.pop();
			return;
		}
		while (myheap.size()>1) {
			TreeNode* tmp = new TreeNode();
			tmp->left = myheap.top();
			myheap.pop();
			tmp->right = myheap.top();
			myheap.pop();
			tmp->fre = tmp->left->fre + tmp->right->fre;
			std::cout << "processing " << tmp->left->ch << tmp->left->fre << " and " << tmp->right->ch << tmp->right->fre << std::endl;
			myheap.push(tmp);
		}
		root = myheap.top();
		myheap.pop();
		
	}
	void constructHuffmanTable(TreeNode* root) {
		std::cout << "constructing huffman table..." << std::endl;
		dfsConstructTb(root, hufftb, "");
		std::cout << hufftb.size() << std::endl;
		for (auto x : hufftb)
			std::cout << x.first << " and " << x.second << std::endl;
	}
	std::string zip(std::string& str) {
		constructTree(str);
		constructHuffmanTable(root);
		std::string buff;
		std::bitset<8> buffchar = 0;
		int i = 7;
		for (auto x : str) {
			std::string tmp = hufftb[x];
			for (int j = 0; j<tmp.length(); j++) {
				if (i<0) {
					char tmpchar = buffchar.to_ulong();
					buff.push_back(tmpchar);
					buffchar = 0;
					i = 7;
				}
				if (tmp[j] == '1')
					buffchar[i] = true;
				else
					buffchar[i] = false;
				i--;
			}
		}
		char last;
		buff.push_back((char)buffchar.to_ullong());
		if (i < 0) last = 0;
		else
			last = i + 1;
		buff.push_back(last);
		return buff;
	}
	std::string unzip(std::string& str) {
		std::string buff;
		std::bitset<8> buffchar;
		std::string tmp;
		for (int i = 0; i < str.length() - 1; i++) {
			buffchar = str[i];
			tmp += buffchar.to_string();
		}
		//std::cout << (int)str.back() << std::endl;
		int L = tmp.length() - (int)str.back();
		std::cout << tmp << std::endl;
		TreeNode* curr = root;
		for (int i = 0; i <= L; i++) {
			std::cout << tmp[i];
			if (curr->left == nullptr&&curr->right == nullptr) {
				buff.push_back(curr->ch);
				curr = root;
			}
			if (tmp[i] == '0')
				curr = curr->left;
			else if (tmp[i] == '1')
				curr = curr->right;
		}
		std::cout << std::endl;
		return buff;
	}
	~Huffman() { dfsDelete(root); }
private:
	TreeNode * root;
	std::unordered_map<char, int> charmap;
	std::unordered_map<char, std::string> hufftb;
	void dfsConstructTb(TreeNode* root, std::unordered_map<char, std::string>& hufftb, std::string path) {
		if (root == nullptr) return;
		if (root->left == nullptr&&root->right == nullptr) {
			hufftb[root->ch] = path;
			return;
		}
		dfsConstructTb(root->left, hufftb, path + "0");
		dfsConstructTb(root->right, hufftb, path + "1");
	}
	void dfsDelete(TreeNode* root) {
		if (root == nullptr) return;
		dfsDelete(root->left);
		dfsDelete(root->right);
		delete root;
	}
};

int main() {
	std::string str = "bbbbbegingggggg attttttack tonnnnnnight aaaaaaaand fffffffuck thhhhhhem allllllllllll";
	Huffman huff;
	std::string res = huff.zip(str);
	std::cout << (str.length()- res.length())*1.0/str.length() << std::endl;
	std::cout << huff.unzip(res) << std::endl;
	std::cout << str << std::endl;
	return 0;
}

