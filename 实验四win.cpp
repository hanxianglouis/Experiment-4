#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

struct node1{
    string data;
    node1* right;
    node1* left;
    node1* middle;

    node1(string data):data(data),right(nullptr),middle(nullptr),left(nullptr){}
};

class tree1 {
    public: 
        node1* head;

        tree1():head(nullptr){
            head=new node1("DataRUC");
            //第二层
            head->left=new node1("A");
            head->middle=new node1("H");
            head->right=new node1("G");
            node1* l=head->left,*r=head->right,*m=head->middle;
            //第三层
            l->left=new node1("2022");
            l->middle=new node1("2023");//保证2023，2024都对应中，方便查找
            m->middle=new node1("2024");
            r->left=new node1("2021");
            r->middle=new node1("2024");//保证2023，2024都对应中，方便查找
            //第四层
            l->left->middle=new node1("Intelligent device for air purification in a home environment");
            l->middle->middle=new node1("Pharmaceutical composition for the treatment of chronic diseases");
            m->middle->middle=new node1("Distributed electricity trading method based on blockchain");
            r->left->left=new node1("Image processing method in augmented reality devices");
            r->left->right=new node1("Autonomous vehicle navigation system based on machine learning");
            r->middle->middle=new node1("High-precision radio signal receiving device");
        }

        ~tree1(){}

        void traversal(node1 *tree){
            if(tree==nullptr){
                return;
            }
            else{
                cout<<tree->data<<endl;
                traversal(tree->left);
                traversal(tree->middle);
                traversal(tree->right);
            }
            return;
        }

        void look_up(){
            int judge=0;
            string field,year;
            do{
                cout<<"Please enter the patent's field(A/H/G)"<<endl;
                fflush(stdin);
                getline(cin,field);
                if(field!="A" && field!="H" && field!="G"){
                    cout<<"Invalid input! Enter again!"<<endl;
                }
            }while(field!="A" && field!="H" && field!="G");
            do{
                cout<<"Please enter the patent's year"<<endl;
                getline(cin,year);
                if(year!="2021" && year!="2022" && year!="2023" && year!="2024"){
                    cout<<"Invalid input! Enter again!"<<endl;
                }
            }while(year!="2021" && year!="2022" && year!="2023" && year!="2024");
            //第二层
            node1* result;
            if(field=="A"){
                result=head->left;
            }
            else if(field=="G"){
                result=head->middle;
            }
            else{
                result=head->right;
            }
            //第三层
            if((year=="2021" && result->left->data=="2021(") ||(year=="2022" && result->left->data=="2022")){
                result=result->left;
            }
            else if((year=="2023" && result->left->data=="2023(") ||(year=="2024" && result->middle->data=="2024")){
                result=result->middle;
            }
            else {
                judge=1;
            }
            //输出
            if(judge==0){
                cout<<"The result is:"<<endl;
                if(result->left!=nullptr){
                    cout<<result->left->data<<endl;
                }
                if(result->middle!=nullptr){
                    cout<<result->middle->data<<endl;
                }
                if(result->right!=nullptr){
                    cout<<result->right->data<<endl;
                }
            }
            else{
                cout<<"The patent doesn't exit!"<<endl;
            }
            return;
        }
};

struct node2{
    int firmid;
    double expenditures;
    node2* right;
    node2* left;
    int ltag,rtag;//0表示指向孩子，1表示指向前驱或后继

    node2(int firmid,double expenditures):firmid(firmid),expenditures(expenditures),left(nullptr),right(nullptr),ltag(-1),rtag(-1){}
};

class tree2 {
    public:
        node2* head;

        tree2():head(nullptr){
            int firmid;
            double expenditures;
            vector<node2*> table;//先用线性结构，在用线性结构转换成完全二叉树
            ifstream file("rde.csv");
            string line;
            getline(file,line);//略过标题行
            while(!file.eof() && getline(file,line)){
                stringstream ss(line);
                string temp;
                getline(ss,temp,',');
                firmid=stoi(temp);
                getline(ss,temp,',');
                expenditures=stod(temp);
                node2* new_node=new node2(firmid,expenditures);
                table.push_back(new_node);
            }
            file.close();
            head=table[0];
            for(int i=0;i<table.size();i++){
                if((2*i+1)<table.size()){
                    table[i]->left=table[2*i+1];
                    table[i]->ltag=0;
                }
                else{
                    table[i]->left=nullptr;
                }
                if((2*i+2)<table.size()){
                    table[i]->right=table[2*i+2];
                    table[i]->rtag=0;
                }
                else{
                    table[i]->right=nullptr;
                }
            }
        }

        ~tree2(){}

        void preorder(node2* tree){
            if(tree==nullptr){
                return;
            }
            else{
                cout<<tree->firmid<<' '<<tree->expenditures<<endl;
                preorder(tree->left);
                preorder(tree->right);
            }
            return;
        }

        void midorder(node2* tree){
            if(tree==nullptr){
                return;
            }
            else{
                midorder(tree->left);
                cout<<tree->firmid<<' '<<tree->expenditures<<endl;
                midorder(tree->right);
            }
            return;
        }

        void postorder(node2* tree){
            if(tree==nullptr){
                return;
            }
            else{
                postorder(tree->left);
                postorder(tree->right);
                cout<<tree->firmid<<' '<<tree->expenditures<<endl;
            }
            return;
        }

        void midrder_for_clue(node2* tree,vector<node2*> &clue){
            if(tree==nullptr){
                return;
            }
            else{
                midrder_for_clue(tree->left,clue);
                clue.push_back(tree);
                midrder_for_clue(tree->right,clue);
            }
            return;
        }

        void clue(){
            vector<node2*> clue;
            midrder_for_clue(head,clue);
            for(int i=0;i<clue.size();i++){
                if(clue[i]->ltag==-1 && i!=0){
                    clue[i]->left=clue[i-1];
                    clue[i]->ltag=1;
                }
                if(clue[i]->rtag==-1 && i!=clue.size()-1){
                    clue[i]->right=clue[i+1];
                    clue[i]->rtag=1;
                }
            }
            cout<<"The firm pointed by will be showed if the pointer is a clue."<<endl;
            cout<<"The result is:"<<endl;
            for(int i=0;i<clue.size();i++){
                cout<<clue[i]->firmid<<' ';
                if(clue[i]->ltag==1){
                    cout<<"left:"<<clue[i]->left->firmid<<' ';
                }
                if(clue[i]->rtag==1){
                    cout<<"right:"<<clue[i]->right->firmid<<' ';
                }
                cout<<endl;
            }
            return;
        }
};

//一下为第三问Huffman Tree相关内容，与前部分内容几乎无关
struct node{
    char ch;
    int freq;
    node* left;
    node* right;
    string code;

    node(char ch,int freq):ch(ch),freq(freq),left(nullptr),right(nullptr),code(""){}
};

struct codepoint{
    char ch;
    string code;

    codepoint(char ch, string code):ch(ch),code(code){}
};

struct compare{//定义比较函数
    bool operator()(node* l,node* r){
        return l->freq > r->freq;//返回值为true，则第二个的优先级更高
    }
};

class huffmantree {
    private:
        node* head;

    public:
        huffmantree(priority_queue<node*,vector<node*>,compare> pq):head(nullptr){
            while(pq.size()>1){
                node* l=pq.top();
                pq.pop();
                node* r=pq.top();
                pq.pop();
                node* newnode=new node('#',l->freq+r->freq);
                newnode->left=l;
                newnode->right=r;
                pq.push(newnode);
            }
            head=pq.top();
        }

        huffmantree(fstream &file):head(nullptr){//从文件中读取哈夫曼树
            head=new node('#',0);
            string line;
            getline(file,line);
            while(getline(file,line)){
                stringstream ss(line);
                string temp,code;
                getline(ss,temp,',');
                char ch=temp[0];
                getline(ss,code,',');
                node* nownode=head;
                for(int i=0;i<code.size()-1;i++){
                    if(code[i]=='0' && nownode->left==nullptr){
                        nownode->left=new node('#',0);
                    }
                    if(code[i]=='1' && nownode->right==nullptr){
                        nownode->right=new node('#',0);
                    }
                    if(code[i]=='0'){
                        nownode=nownode->left;
                    }
                    if(code[i]=='1'){
                        nownode=nownode->right;
                    }
                }
                if(code[code.size()-1]=='0'){
                    nownode->left=new node(ch,0);
                }
                if(code[code.size()-1]=='1'){
                    nownode->right=new node(ch,0);
                }
            }
        }

        ~huffmantree(){};

        node* gethead(){
            return head;
        }

        void coding(node* tree, string s){
            if(tree==nullptr){
                return;
            }
            else {
                coding(tree->left,s+"0");
                tree->code=s;
                coding(tree->right,s+"1");
                return;
            }
        }

        void gene_codebook(node* tree, vector<codepoint> &codebook){
            if(tree==nullptr){
                return;
            }
            else{
                if(tree->ch!='#'){
                    codebook.push_back(codepoint(tree->ch,tree->code));
                }
                gene_codebook(tree->left,codebook);
                gene_codebook(tree->right,codebook);
                return;
            }
        }

        void decode(string code){
            string words;
            node* nownode=head;
            int i=0;
            while(i<=code.size()){//保证最后一个字符也被加入words
                if(nownode->ch=='#'){
                    if(i==code.size() && nownode!=head){
                        cout<<"Wrong code!"<<endl;//如果最后一个走完指的不是有效字符，则肯定编码有误
                        return;
                    }
                    if(code[i]=='0'){
                        nownode=nownode->left;
                    }
                    if(code[i]=='1'){
                        nownode=nownode->right;
                    }
                    i++;
                }
                else {
                    words.push_back(nownode->ch);
                    nownode=head;
                }
            }
            cout<<"The code refers to: "<<words<<endl;
            return;
        }
};

string char_to_code(char a,vector<codepoint> codebook){
    int i=0;
    for (i=0;i<codebook.size();i++){
        if(codebook[i].ch==a){
            return codebook[i].code;
        }
    }
    if(i==codebook.size()){
        cout<<"Invalid character!"<<endl;
    }
    return "";
}

string encode(const string &words,const vector<codepoint> &codebook){
    string codes="";
    for (int i=0;i<words.size();i++){
        codes.append(char_to_code(words[i],codebook));
    }
    return codes;
}

void send(){
    string words;
    priority_queue<node*,vector<node*>,compare> pq;
    cout<<"Please enter the words you want to encode:"<<endl;
    fflush(stdin);
    getline(cin,words);
    string temp=words;
    while(!temp.empty()){//一种自创的自我感觉比较好的方法，每次都数第一个字符的个数然后顺手删掉，直至字符串为空
        int count=0;
        char ch=temp[0];
        for(int i=0;i<temp.size();i++){
            if(temp[i]==ch){
                count++;
                temp.erase(i,1);
                i--;
            }
        }
        pq.push(new node(ch,count));
    }
    huffmantree tree(pq);
    string s="";
    tree.coding(tree.gethead(),s);
    vector<codepoint> codebook;
    tree.gene_codebook(tree.gethead(),codebook);
    string codes=encode(words,codebook);
    fstream file("code.txt",ios::out);
    file<<codes;
    fstream file2("codebook.txt",ios::out);
    file2<<"ch,code"<<endl;
    for(int i=0;i<codebook.size();i++){
        file2<<codebook[i].ch<<','<<codebook[i].code<<endl;
        pq.pop();
    }
    file.close();
    file2.close();
}

void receive(){
    fstream file("code.txt",ios::in);
    string code;
    getline(file,code);
    file.close();
    fstream file2("codebook.txt",ios::in);
    huffmantree tree(file2);
    file2.close();
    tree.decode(code);
    return;
}






int main(){
    tree1 patent_tree;
    tree2 expenditures_tree;
    int judge = 0; // 判断是否退出
    while (judge == 0)
    {
        int choice;
        cout<<"1.Show the path of the query."<<endl;
        cout<<"2.Look up patent."<<endl;
        cout<<"3.Show the expenditures tree."<<endl;
        cout<<"4.Clues the expenditures tree.(The firm pointed by will be showed if the pointer is a clue.)"<<endl;
        cout<<"5.Encode the words."<<endl;
        cout<<"6.Decode the code."<<endl;
        cout<<"0.exit"<<endl;
        cout<<"----------------------"<<endl;
        cout<<"Please choose a function:";
        cin>>choice;
        switch (choice)
        {
        case 1:{
            patent_tree.traversal(patent_tree.head);
            getchar();
            getchar();
            system("cls");
            break;
        }
        case 2:
            patent_tree.look_up();
            getchar();
            system("cls");
            break;
        case 3:
            cout<<"Preorder:"<<endl;
            expenditures_tree.preorder(expenditures_tree.head);
            cout<<endl;
            cout<<"Midorder:"<<endl;
            expenditures_tree.midorder(expenditures_tree.head);
            cout<<endl;
            cout<<"Postorder:"<<endl;
            expenditures_tree.postorder(expenditures_tree.head);
            getchar();
            getchar();
            system("cls");
            break;
        case 4:
            expenditures_tree.clue();
            getchar();
            getchar();
            system("cls");
            break;
        case 5:
            send();
            cout<<"The information has been saved in \"codebook.txt\" and \"code.txt\"."<<endl;
            getchar();
            system("cls");
            break;
        case 6:
            receive();
            getchar();
            getchar();
            system("cls");
            break;
        case 0:
            judge = 1;
            break;
        default:
            cout<<"Invalid input! Please choose again!"<<endl;
        }
    }
    return 0;
}