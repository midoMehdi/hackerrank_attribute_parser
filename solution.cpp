#include<iostream>
#include<cmath>
#include<vector>
#include<map>
using namespace std;
class abrnode
{
private:
	string m_tag;
	map<string, string> properties;
	vector<abrnode*> m_fils;
	bool is_end;
public:
	abrnode() {
		m_tag = ".";
		is_end = false;
	}

	~abrnode() {
		m_fils.clear();
	}

	friend class abrtree;

};
class abrtree
{
private:
	abrnode* root;

	void addAuxAux(vector<string> vec_line, abrnode*& p) {
		if (p->m_fils.size() == 0 && vec_line[0].substr(0, 2) != "</") {
			p->m_fils.push_back(new abrnode);
			if(vec_line.size() == 1)
				p->m_fils[0]->m_tag = vec_line[0].substr(1, vec_line[0].size() - 2);
			else if (vec_line.size() > 1) {
				p->m_fils[0]->m_tag = vec_line[0].substr(1, vec_line[0].size());
				vec_line.erase(vec_line.begin());
				for (size_t i = 0; i < vec_line.size(); ) {
					if (vec_line[i + 2].at(vec_line[i+ 2].size() - 1) == '>') {
						p->m_fils[p->m_fils.size() - 1]->properties.insert(pair<string, string>(vec_line[i], vec_line[i + 2].substr(1, vec_line[i + 2].size() - 3)));
					}
					else {
						p->m_fils[p->m_fils.size() - 1]->properties.insert(pair<string, string>(vec_line[i], vec_line[i+2].substr(1, vec_line[i+2].size() - 2)));
						//vec_line.erase(vec_line.begin());
					}
					i += 3;
				}
			}
			return;
		}
		if (vec_line[0].substr(0, 2) == "</") {
			for (int i = 0; i < p->m_fils.size(); i++) {
				if (p->m_fils[i]->is_end == false) {
					if (vec_line[0].substr(2, vec_line[0].size() - 3) == p->m_fils[i]->m_tag) {
						p->m_fils[i]->is_end = true;
						return;
					}
					else {
						addAuxAux(vec_line, p->m_fils[i]);
						return;
					}
				}
			}
		}
		
		for (size_t i = 0; i < p->m_fils.size(); i++) {
			if (p->m_fils[i]->is_end == false) {
				addAuxAux(vec_line, p->m_fils[i]);
				return;
			}
		}
		
		p->m_fils.push_back(new abrnode);
		if (vec_line.size() == 1)
			p->m_fils[p->m_fils.size() - 1]->m_tag = vec_line[0].substr(1, vec_line[0].size() - 2);
		else if (vec_line.size() > 1) {
			p->m_fils[p->m_fils.size() - 1]->m_tag = vec_line[0].substr(1, vec_line[0].size());
		}
		vec_line.erase(vec_line.begin());
		for (size_t i = 0; i < vec_line.size(); ) {
			if (vec_line[i + 2].at(vec_line[i + 2].size() - 1) == '>')
				p->m_fils[p->m_fils.size() - 1]->properties.insert(pair<string, string>(vec_line[i], vec_line[i+2].substr(1, vec_line[i+2].size() - 3)));
			else
				p->m_fils[p->m_fils.size() - 1]->properties.insert(pair<string, string>(vec_line[i], vec_line[i+2].substr(1, vec_line[i+2].size() - 2)));
			i += 3;
		}
		return;
	}
	bool isExistAux(std::vector<string> vec_querie, abrnode* p)const {
		if (vec_querie.size() == 0)
			return false;

		for (size_t i = 0; i < p->m_fils.size(); i++) {
			if (p->m_fils[i]->m_tag == vec_querie[0]) {
				vec_querie.erase(vec_querie.begin());
				if (vec_querie.size() == 1) {
					if (p->m_fils[i]->properties.find(vec_querie[0]) != p->m_fils[i]->properties.end()) {
						cout << p->m_fils[i]->properties.find(vec_querie[0])->second << endl;
						return true;
					}
					else return false;
				}
				return isExistAux(vec_querie, p->m_fils[i]);
			}
		}
		return false;

	}
public:
	abrtree() {
		root = new abrnode;
	}
	~abrtree() {
		delete root;
	}

	void add(vector<string> vec_line) {
		abrnode* p = root;
		addAuxAux(vec_line, p);
	}
	bool isExist(vector<string> vec_querie) const {
		abrnode* p = root;
		return isExistAux(vec_querie, p);
	}


};

int main() {

	abrtree tree;
	int n, q;
	cin >> n >> q;
	size_t i = 0;
	while (i < n) {
		vector<string> vec;
		string data;
		while (cin >> data) {
			
			vec.push_back(data);
			if (data.at(data.size() - 1) == '>' && data.substr(0, 2) != "</") {
				i++;
				break;
			}
			if (data.substr(0, 2) == "</") {
				i++;
				break;					
			}
			
		}
		tree.add(vec);
	}

	for (size_t i = 0 ; i < q ; i++) {
		vector<string> vec_querie;
		string querie;
		cin >> querie;
		int isExist_dot;
		while ((isExist_dot = querie.find_first_of('.')) != -1) {
			string first_tag = querie.substr(0, isExist_dot);
			querie = querie.substr(isExist_dot + 1, querie.size());
			vec_querie.push_back(first_tag);
		}
		int isExist_tilde;
		if ((isExist_tilde = querie.find_first_of('~')) != -1) {
			string first_tag = querie.substr(0, isExist_tilde);
			string attribute = querie.substr(isExist_tilde + 1, querie.size());
			vec_querie.push_back(first_tag);
			vec_querie.push_back(attribute);
			bool temp = tree.isExist(vec_querie);
			if (!temp) {
				cout << "Not Found!" << endl;
			}
		}
		else {
			cout << "Not Found!" << endl;
		}
	}
	
	return 0;
}
