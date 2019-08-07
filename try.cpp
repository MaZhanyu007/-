/*
˼�룺
ÿ�δ����ݼ��и��������Ϣ����ѡȡһ����������������ݽ��л��֣�ÿ�λ��ֶ�������һ��������
ʹ������Խ��Խ�٣����������ݼ�����ͬһ�࣬������������������ʱ�����ֽ�����

��Ϣ�أ�  Entropy(D) = -sum( p_i * log_2(p_i) )
DΪ��������
1<= i <= ����D���������������
p_i Ϊ��i�������D����ռ�ı���
��Ϣ���棺Gain(D_all) = Entropy(D_all) - sum( D_i / D_all * Entorpy(D_i) )
D_all Ϊ�����ݼ�
D_i Ϊ��i�������ݼ�
1<= i <= �����ݼ���
*/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <math.h>
using namespace std;

/*set�������ǣ�����Ԫ�ض������Ԫ�صļ�ֵ�Զ�����set��Ԫ�ز���map��������ͬʱӵ��ʵֵ(value)�ͼ�ֵ(key),setԪ�صļ�ֵ����ʵֵ��ʵֵ���Ǽ�ֵ��set����������Ԫ������ͬ�ļ�ֵ*/
/*�㷨��Algorithm��Ϊһ������ľ��岽�裬�����ڼ��㡢���ݴ�����Զ�����C++ �㷨�⣨Algorithms library��Ϊ C++ �����ṩ�˴��������������������������н����㷨�����ĺ�������Щ�������Ϊ��������ģ�壬�󲿷���ͷ�ļ� <algorithm> �ṩ��һС����λ�� <numeric>��<cstdlib> �С�*/
#define N 14
#define feature 4

vector< vector<string> > X;
string x[N][feature + 1] =
{
	{ "Sunny", "Hot", "High", "Weak", "no" },
	{ "Sunny", "Hot", "High", "Strong", "no" },
	{ "Overcast", "Hot", "High", "Weak", "yes" },
	{ "Rain", "Mild", "High", "Weak", "yes" },
	{ "Rain", "Cool", "Normal", "Weak", "yes" },
	{ "Rain", "Cool", "Normal", "Strong", "no" },
	{ "Overcast", "Cool", "Normal", "Strong", "yes" },
	{ "Sunny", "Mild", "High", "Weak", "no" },
	{ "Sunny", "Cool", "Normal", "Weak", "yes" },
	{ "Rain", "Mild", "Normal", "Weak", "yes" },
	{ "Sunny", "Mild", "Normal", "Strong", "yes" },
	{ "Overcast", "Mild", "High", "Strong", "yes" },
	{ "Overcast", "Hot", "Normal", "Weak", "yes" },
	{ "Rain", "Mild", "High", "Strong", "no" },
};

//���Ե����ƣ����� Outlookȡֵ��Sunny, Overcast, Rain
string attribute[] =   {"Outlook","Temperature","Humidity","Wind"};

vector<string> attributes;

/*
*****************************************************************
�������ƣ�createDataset
���ܣ��������ݼ�
��������
����ֵ���ޣ���������Ϊȫ�ֱ�����
*****************************************************************
*/

void createDataset()//�������ݼ�
{
	//�������ݼ�//N*(feature + 1)�Ķ�άvector
	X = vector< vector<string> >(N, vector<string>(feature + 1));
	//��x��ֵ����X��
	int i, j;
	for (i = 0; i<N; i++)
	{
		for (int j = 0; j<feature + 1; j++)
		{
			X[i][j] = x[i][j];
		}
	}
	
	//��������
	for (i = 0; i<feature; i++)//������vector��push_back����,��β����������
		attributes.push_back(attribute[i]);
}

/*
*****************************************************************
�������ƣ�calcShanno
���ܣ�����������ݼ�����ũ��
������string��ά���������ã�
����ֵ���������ݼ�����ũ�أ�double��
*****************************************************************
*/

double calcShanno(vector< vector<string> > &data)//����������ݼ�����ũ��
{
	int n = data.size(); //��ά����һ��������
	map<string, int> classCounts; //���ܼ�ʵ��
	int i;
	int label = data[0].size() - 1; //��ǩ������Ϊÿ��������1������classCounts�±��Ӧ���ķ��࣬��ʵ�������±��Ӧyes no��
	for (i = 0; i<n; i++)  //��ʼΪ0//ÿһ�е�yes no��Ӧ��ʵֵ����ʼ��Ϊ0
	{
		classCounts[data[i][label]] = 0; //�ؼ��ֶ��ǡ�yes����no����ֻ������Ӧ��ʵֵ����0
	}
	for (i = 0; i<data.size(); i++)  //ÿ������һ�Σ�+1
	{
		classCounts[data[i][label]] += 1; //�����ۼ�yes,no�Ĵ���//�����������map����Ӧ��ֵ���Ա��޸ĵ�����
	}

	//������ũ��
	double shanno = 0;
	map<string, int>::iterator it; //��������ָ��ķ�ָ
	for (it = classCounts.begin(); it != classCounts.end(); it++)//����һ����������ֵ��yes-9,no-5(��ʱ���ǵ�һ�ε�ʱ��)
	{
		double prob = (double)(it->second) / (double)n;//��������ռ����//����Ե�second����yes����no������
		shanno -= prob * (log(prob) / log(2));
	}
	return shanno;
}

/*
*****************************************************************
�������ƣ�splitDataSet
���ܣ����ո��������������ݼ������ֺ�����ݼ��в������������������µ����ݼ���ά������һ��
������string��ά����,  axis �������±�(0,1,2,3),  value������ֵ(string:ÿ�������ľ�������)
����ֵ���������ݼ�����ũ�أ�double��
*****************************************************************
*/

vector< vector<string> > splitDataSet(vector< vector<string> > data, int axis, string value)//���ո��������������ݼ������ֺ�����ݼ��в������������������µ����ݼ���ά������һ��
{
	vector< vector<string> > result;
	for (int i = 0; i<data.size(); i++)//data.size=14
	{
		if (data[i][axis] == value)//data[i].size=5
		{
			//������ǰ���������ά��ȥ��
			vector<string> removed(data[i].begin(), data[i].begin() + axis); //��ʼ��Ϊ����������ָ����Χ��Ԫ�صĿ���
			removed.insert(removed.end(), data[i].begin() + axis + 1, data[i].end());  //v.insert(v.end(), a[1], a[3]);//��β������a[1]��a[3]//��Ȼ������ָ����ָ��ĵ�����
			result.push_back(removed);
			//��ʵ���ﲻ��ȥ�����������¶��������ݼ�result��Ȼ��ѳ�ȥ<axis,value>���,ʣ�µĸ��ƽ�ȥ����
		}
	}
	return result;
}

/*
*****************************************************************
�������ƣ�createFeatureList
���ܣ����������б�
������string��ά���������ã�,  axis �������±�(0,1,2,3)
����ֵ������������ȡֵ��string��
*****************************************************************
*/

vector<string> createFeatureList(vector< vector<string> > &data, int axis)//���������б�
{
	int n = data.size();         //=14
	vector<string>featureList;   //ĳ�����Ե�����������ȡֵ����outlook��Sunny��Rain��overcast��
	set<string> s;
	for (int j = 0; j<n; j++)    //Ѱ�Ҹ����������п���ȡֵ
		s.insert(data[j][axis]); //�Ѹ���������ȡֵ������s����
	set<string>::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		featureList.push_back(*it);//�ٰ�s��������ֵ�����������б�
	}
	return featureList;
}

/*
*****************************************************************
�������ƣ�chooseBestFeatureToSplit
���ܣ�ѡ����õ����ݼ����ַ�ʽ
������string��ά���������ã�
����ֵ����ǰ������������
*****************************************************************
*/

int chooseBestFeatureToSplit(vector< vector<string> > &data)//ѡ����õ����ݼ����ַ�ʽ
{
	int n = data[0].size() - 1;				//=4
	double bestEntropy = calcShanno(data);  //��ʼ��ũ��
	double bestInfoGain = 0;		        //������Ϣ����
	int bestFeature = 0;					//��õ�����
	for (int i = 0; i<n; i++)				//��������
	{
		double newEntropy = 0;				                      //��ʼ���µ���ũ��
		vector<string> featureList = createFeatureList(data, i);  //�����������п���ȡֵ����0��1��2��3��������������Ӧ��ֵ��//��ʵ�����൱��ĳ����������һ�С�
		for (int j = 0; j<featureList.size(); j++)				  //featureList.size()=14
		{
			vector< vector<string> > subData = splitDataSet(data, i, featureList[j]); //�����subData�����ݼ���ȥfeatureList[j]֮������ݼ�
			double prob = (double)subData.size() / (double)data.size(); //��ȥ��ǰ����֮��ʣ�����ݼ���ռ����
			newEntropy += prob * calcShanno(subData); //һ���ۼӵõ���ȥ������֮�����ũ��
		}
		double infoGain = bestEntropy - newEntropy;  //��Ϣ���棬���صļ��٣�����������ȵļ���
		if (infoGain > bestInfoGain) //������ѡȡ��ǰ����£�������������Ϣ�������
		{
			bestInfoGain = infoGain;
			bestFeature = i;
		}
	}
	return bestFeature; //���ﷵ�ص��Ǳ��0��1��2��3
}

/*
*****************************************************************
�������ƣ�majorityCnt
���ܣ����س��ִ������ķ�������
//������ݼ��Ѵ������������ԣ������ǩ��Ȼ����Ψһ�ģ����ö�������ķ�������Ҷ�ӽڵ�ķ���
������string���������ã�
����ֵ��string
*****************************************************************
*/

string majorityCnt(vector<string> &classList)
{
	int n = classList.size();
	map<string, int> classCount;
	int i;
	for (i = 0; i<n; i++)
	{
		classCount[classList[i]] = 0;
	}
	for (i = 0; i<n; i++)
	{
		classCount[classList[i]] += 1;
	}

	int maxCnt = 0;
	map<string, int>::iterator it;
	string result = "";
	for (it = classCount.begin(); it != classCount.end(); it++)
	{
		if (it->second > maxCnt)
		{
			maxCnt = it->second;
			result = it->first;
		}
	}
	return result;
}

struct Node //��Ҷ�ڵ�
{
	string attribute; //����
	string val; //����ֵ(string:ÿ�������ľ�������)
	bool isLeaf; //�Ƿ��к��ӽ��
	vector<Node*> childs; //���ӽڵ�ָ��

	Node() //���캯��
	{
		val = "";
		attribute = "";
		isLeaf = false;
	}
};

Node *root = NULL; //��̬���

/*
*****************************************************************
�������ƣ�createTree
���ܣ��ݹ鹹��������
������Node*���ڵ�,��ά����string�����ã���string����������
����ֵ��Note*����ַ
*****************************************************************
*/

Node* createTree(Node *root, vector< vector<string> > &data, vector<string> &attribute)
{
	if (root == NULL)
		root = new Node();
	vector<string> classList;
	set<string> classList1; //����������set�ؼ��ֲ����ظ����ص㣬����Ϊ������������Ƿ���yes��no
	int i, j;
	int label = data[0].size() - 1; //=4
	int n = data.size(); //=14
	for (i = 0; i<n; i++) //�ռ���ǰ���ݵ����е�yes no��classList������yes no������ĵ�classList1
	{
		classList.push_back(data[i][label]);
		classList1.insert(data[i][label]);
	}
	if (classList1.size() == 1)  //�������ʵ��������ͬһ�ֹ࣬ͣ���֣�ֻ��yes����no��
	{
		if (classList[0] == "yes")
			root->attribute = "yes";
		else
			root->attribute = "no"; 
		root->isLeaf = true;
		return root;
	}
	if (data[0].size() == 1)  //�������������������س��ִ����������
	{
		root->attribute = majorityCnt(classList);
		return root;
	}

	int bestFeatureIndex = chooseBestFeatureToSplit(data); //����õ���ǰ��������������Է���
	vector<string> featureList = createFeatureList(data, bestFeatureIndex);  //������õ��ĵ�ǰ�����������Է��࣬Ȼ����������Ե����п�������ֵ���ŵ�����
	string bestFeature = attribute[bestFeatureIndex]; //�����������bestFeatureIndex = chooseBestFeatureToSplit(data)��int�͵�

	root->attribute = bestFeature;   //��¼Ҫ���ֵ�������������

	for (i = 0; i<featureList.size(); i++)  //���ڵ�ǰ���Ե�ÿ������ֵ�������µķ�֧
	{
		vector<string> subAttribute;  //�µ������б���������ǰҪ���ֵ���������������һ��������������ά������һ��
		for (j = 0; j<attribute.size(); j++)
		{
			if (bestFeature != attribute[j])
				subAttribute.push_back(attribute[j]); //�ѣ���������ǰҪ���ֵ����ԣ�����������ֵȫ��ѹ������
		}
		Node *newNode = new Node();
		newNode->val = featureList[i];   //��¼���Ե�ȡֵ��ȡ�ĸ������ַ�����
		createTree(newNode, splitDataSet(data, bestFeatureIndex, featureList[i]), subAttribute);//��ʱ�ú���splitDataSet����һ���µ����ݼ�����Ȼ�����ų���bestFeatureIndex��ͬʱ����ʣ������ԷŽ�ȥ
		root->childs.push_back(newNode); //Ȼ��ѽ��ѹ�뺢��ָ������
	}
	return root;
}

/*
*****************************************************************
�������ƣ�print
���ܣ���ӡ
������Node*���ڵ�,�������
����ֵ����
*****************************************************************
*/

void print(Node *root, int depth)
{
	int i;
	for (i = 0; i<depth; i++)
		cout << "\t";

	if (root->val != "")
	{
		cout << root->val << endl;
		for (i = 0; i<depth + 1; i++)
			cout << "\t";
	}
	cout << root->attribute << endl;
	vector<Node*>::iterator it;
	for (it = root->childs.begin(); it != root->childs.end(); it++)
	{
		print(*it, depth + 1);
	}
}

/*
*****************************************************************
�������ƣ�clsssify
���ܣ�Ԥ��
������Node*���ڵ�,�����б�string��������
����ֵ����
*****************************************************************
*/

string classify(Node *root, vector<string> &attribute, string *test)
{
	string firstFeature = root->attribute;
	int firstFeatureIndex;
	int i;
	for (i = 0; i<feature; i++)  //�ҵ����ڵ��ǵڼ�������
	{
		if (firstFeature == attribute[i])
		{
			firstFeatureIndex = i;
			break;
		}
	}
	if (root->isLeaf)  //�����Ҷ�ӽڵ㣬ֱ��������
		return root->attribute;
	for (i = 0; i<root->childs.size(); i++)
	{
		if (test[firstFeatureIndex] == root->childs[i]->val)
		{
			return classify(root->childs[i], attribute, test);
		}
	}
}

//�ͷŽڵ�
void freeNode(Node *root)
{
	if (root == NULL)
		return;
	vector<Node*>::iterator it;
	for (it = root->childs.begin(); it != root->childs.end(); it++)
		freeNode(*it);
	delete root;
}

int main(void)
{
	createDataset();
	root = createTree(root, X, attributes);
	print(root, 0);

	string test[6][4] = 
	{{"Sunny","Hot","Normal","Weak"},
	{"Sunny","Hot","Normal","Strong"},
	{"Rain","Hot","Normal","Weak"},
	{"Sunny","Mild","Normal","Weak"},
	{"Sunny","Cool","High","Strong"},
	{"Sunny","Cool","High","Weak"}};
	int i,j;
	cout << endl << "���ԣ�";
	for (i = 0; i<feature; i++)
		cout << attributes[i] << "\t";
	

	for (j = 0; j < 6; j++)
	{
		cout << endl << "���ӣ�";
		for (i = 0; i < feature; i++)
		{
			cout << test[j][i] << "\t";
		}
		cout << endl << "Ԥ�⣺";
		cout << classify(root, attributes, test[j]) << endl;
	}
	
	freeNode(root);

	return 0;
}


