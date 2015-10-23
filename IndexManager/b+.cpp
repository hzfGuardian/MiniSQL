/*
3.2.5	Index Manager
Index Manager负责B+树索引的实现，实现B+树的创建和删除（由索引的定义与删除引起）、等值查找、插入键值、删除键值等操作，并对外提供相应的接口。
B+树中节点大小应与缓冲区的块大小相同，B+树的叉数由节点大小与索引键大小计算得到。*/
#define N 6
#include <iostream>
#include <string>
using namespace std;
class node{
protected:
	string value[N-1];
public:
	node ();
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual void display();
	
	virtual node* getadree(int i);
	virtual void setadree(node *p,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void clear();
	virtual void setNext(node *p);
	virtual node* getNext();
	virtual int whoareyou();
};
node::node()
{
	for(int i=0;i<N-1;i++)
	{
		value[i]="";
	}
}
int node::Haveson()
{
	return 0;
}
string node::getvalue(int i){
	return value[i];
}
void node::setvalue(string k,int i){
	value[i]=k;
}
int node::getOffset(int i){ 
	return -100;
}
void node::setOffset(int Off,int i){
}
node* node::getadree(int i){
	return NULL;
}
void node::setadree(node *p,int i){
}
void node::display(){
}
void node::clear(){
}
void node::setNext(node *p){
}
node* node::getNext(){
	return NULL;
}
int node::whoareyou(){
	return 0;
}

class normalnode :public node{
private:
	node* P[N]; //子节点
public:
	normalnode();
	virtual int Haveson();
	virtual node* getadree(int i);
	virtual void setadree(node *p,int i);
	virtual void display();
	void clear();
	virtual int whoareyou();
};
void normalnode::clear(){
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			value[i]="";
		P[i]=NULL;
	}
}
int normalnode::Haveson(){
	if(P[0]==NULL) return 0;
	else return 1;
}
normalnode::normalnode():node(){
	for(int i=0;i<N;i++)
	{
		P[i]=NULL;
	}
}
node* normalnode::getadree(int i){
	return P[i];
}
void normalnode::setadree(node *p,int i){
	P[i]=p;
}
void normalnode::display()
{
	int i=0;
	for(i=0;i<N-1;i++)
	{
		cout<<"in normal "<<i<<" "<<this->getvalue(i)<<endl;
		if(this->getadree(i)!=NULL)
			(this->getadree(i))->display();
	}
	if(this->getadree(i)!=NULL)
			(this->getadree(i))->display();
	cout<<endl;
}
int normalnode::whoareyou(){
	return 1;
}


class leafnode :public node{
protected:
	int Offset[N]; //偏移量
private:
	node* Next;
public:
	leafnode();
	virtual int Haveson();
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void display();
	virtual void clear();
	virtual void setNext(node *p);
	virtual node* getNext();
	virtual int whoareyou();
};
int leafnode::Haveson(){
	return 0;
}
leafnode::leafnode():node(){
	Next=NULL;
	for(int i=0;i<N;i++)
	{
		Offset[i]=0;
	}
}
int leafnode::getOffset(int i){
	return Offset[i];
}
void leafnode::setOffset(int Off,int i){
	Offset[i]=Off;
}
void leafnode::display(){
	node *p=this;
	//do{
		for(int i=0;i<N-1;i++)
			cout<<i<<" "<<p->getOffset(i)<<" "<<p->getvalue(i)<<endl;
		cout<<endl;
	//	p=p->getNext();
	//}
	//while(p!=NULL);
}
void leafnode::clear(){
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			this->setvalue("",i);
		Offset[i]=0;
	}
}
void leafnode::setNext(node *p){
	Next=p;
}
node* leafnode::getNext(){
	return Next;
}
int leafnode::whoareyou(){
	return 2;
}

class tempnode :public leafnode{
private:
	string value2[N];
public:
	tempnode();
	tempnode(node *p);
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void display();
};
tempnode::tempnode(){
	for(int i=0;i<N;i++)
	{
		value2[i]="";
		Offset[i]=0;
	}
}
tempnode::tempnode(node *p){
	for(int i=0;i<N-1;i++)
	{
		//cout<<"1"<<endl;
		this->value2[i]=p->getvalue(i);
		//cout<<"2"<<endl;
		this->Offset[i]=p->getOffset(i);
	}
}
int tempnode::Haveson(){
	return 0;
}
int tempnode::getOffset(int i){
	return Offset[i];
}
void tempnode::setOffset(int Off,int i){
	Offset[i]=Off;
}
string tempnode::getvalue(int i){
	return value2[i];
}
void tempnode::setvalue(string k,int i){
	value2[i]=k;
}
void tempnode::display(){
	for(int i=0;i<N;i++)
		cout<<i<<" "<<getOffset(i)<<" "<<getvalue(i)<<endl;
}
class neicun{
public:
	string value[N];
	node* p[N+1];
	neicun();
	neicun(node *n);
	//void insert(node *n,string k,int j);
};
neicun::neicun(){
	for(int i=0;i<N;i++)
	{
		value[i]="";
		p[i]=NULL;
	}
	p[N]=NULL;
}
neicun::neicun(node *n){
	for(int i=0;i<N;i++)
	{
		if(i!=N-1)
			value[i]=n->getvalue(i);
		p[i]=n->getadree(i);
	}
}




int find(node *tree,string s)
{
	int i=-1;
	while(tree->Haveson())
	{
		for(i=0;i<N-1;i++)//遍历该节点
		{
			if(tree->getvalue(i).empty())//空了
				break;
			if(s<= tree->getvalue(i))//存在value需要去的地方
				break;
		}
		if(i==N-1) //该节点不符合
			tree=tree->getadree(i);
		else//在该节点中
		{
			if(s== tree->getvalue(i))
				tree=tree->getadree(i+1);
			else
				tree=tree->getadree(i);
		}
	}
	for(i=0;i<N-1;i++)
	{
		if(tree->getvalue(i) ==s)
			break;
	}
	if(i==N-1) return -1;
	return tree->getOffset(i);
}
node* parent(node *p,node *root)
{
	int j,i;
	string k=p->getvalue(0);
	if(root==p) return NULL;
	while(root!=p)
	{
		for(j=0;j<N;j++)
		{
			if(p == root->getadree(j))//存在value需要去的地方
				break;
		}
		if(j<N) break;//找到了该节点的父节点

		for(i=0;i<N-1;i++)//遍历该节点
		{
			if(root->getvalue(i).empty())//空了
				break;
			if(k<= root->getvalue(i))//存在value需要去的地方
				break;
		}
		if(i==N-1) //该节点不符合
			root=root->getadree(i);
		else//在该节点中
		{
			if(k== root->getvalue(i))
				root=root->getadree(i+1);
			else
				root=root->getadree(i);
		}
	}
	return root;
}
node* insertleaf(string k,int off,node* p)
{
	int i=0;
	while(k>p->getvalue(i) && !p->getvalue(i).empty()) 
		i++;
	if(k==p->getvalue(i)) //重复
		return p;
	if(p->getvalue(i).empty())//在最后一个
	{
		p->setvalue(k,i);
		p->setOffset(off,i);
		return p;
	}
	else//在中间
	{
		int c=i;
		while(!p->getvalue(i).empty())
			i++;
		for(;i>c;i--)
		{
			p->setvalue(p->getvalue(i-1),i);
			p->setOffset(p->getOffset(i-1),i);
		}
		p->setvalue(k,i);
		p->setOffset(off,i);
		return p;
	}
}
//void insertpar(newtree->getvalue(0),newtree,p,t);
node* insertpar(string k,node *newnode,node *oldnode,node * root)
{
	node * root2=root;
	if(oldnode==root)//根结点
	{
		node *newroot=new normalnode;
		newroot->setadree(oldnode,0);
		newroot->setvalue(k,0); 
		newroot->setadree(newnode,1);
		return newroot;
	}
	else
	{
		int i=0,j=0;
		root=parent(oldnode,root);
		for(j=0;j<N;j++)
		{
			if(oldnode == root->getadree(j))//存在value需要去的地方
				break;
		}

		if(root->getvalue(N-2).empty())//父节点还有空位
		{
			for(i=N-1;i>j+1;i--)
			{
				root->setadree(root->getadree(i-1),i);
				root->setvalue(root->getvalue(i-2),i-1);
			}
			root->setadree(newnode,j+1);
			root->setvalue(k,j);
			return root2;
		}
		else//没空位了，分裂父节点
		{
			neicun *nei=new neicun(root);
			for(i=N;i>j+1;i--)
			{
				nei->p[i]=nei->p[i-1];
				nei->value[i-1]=nei->value[i-2];
			}
			nei->p[j+1]=newnode;
			nei->value[j]=k;
			root->clear();
			node* second=new normalnode;
			int m=0,q=0;
			for(q=0;q<N/2;q++)
			{
				root->setvalue(nei->value[q],q);
				root->setadree(nei->p[q],q);
			}
			root->setadree(nei->p[q],q);
			q++;
			for(;q<N;q++)
			{
				second->setvalue(nei->value[q],m);
				second->setadree(nei->p[q],m);
				m++;
			} 
			second->setadree(nei->p[q],m);
			node *temp=second;
			while(second->Haveson())
			{
				second=second->getadree(0);
			}
			//second->getvalue(0)有问题
			//cout<<second->getvalue(0)<<" "<<p->getvalue(1)<<" "<<t->getvalue(1)<<endl;
			node *t2=insertpar(second->getvalue(0),temp,root,root2);
			return t2;
		}
	}


}
node* insert(string k,int off,node* p)
{
	node *t=p;
	if(!p->Haveson() && p->getvalue(N-2).empty())//p是没满的叶节点 或 空树
	{
		if(p->getvalue(0).empty()) //空树 
		{
			node* tree2=new leafnode;
			tree2->setvalue(k,0);
			tree2->setOffset(off,0);
			return tree2;
		}
		else //没满的叶节点 
		{
			return insertleaf(k,off,p);
		}
	}
	else //1.叶节点，但是满了 2.非叶节点
	{
		if(p->Haveson())//2.非叶节点
		{
			int i;
			while(p->Haveson())//找到K所在叶节点
			{
				for(i=0;i<N-1;i++)
				{
					if(p->getvalue(i).empty())//空了
						break;
					if(k <= p->getvalue(i))
						break;
				}
				if(i==N-1)
					p=p->getadree(i);
				else{
					if(k == p->getvalue(i))
						p=p->getadree(i+1);
					else
						p=p->getadree(i);
				}
			}
		}
		//满叶节点,或者找到的可能没满的叶节点
		if(p->getvalue(N-2).empty()) //没满  t or p
		{
			p=insertleaf(k,off,p);
			return t;
		}
		else //满了 （1.一开始的满叶节点 2.找到所在的叶节点满了）
		{
			for(int q=0;q<N-2;q++)//如果重复了
			{
				if(k==p->getvalue(q)) //重复
				return t;
			}
			node *newtree=new leafnode;
			node* temp=new tempnode(p);
			insertleaf(k,off,temp);
			p->clear(); //删除
			newtree->setNext(p->getNext());//next指针交换
			p->setNext(newtree);
			int j,m=0;
			for(j=0;j<N/2;j++)
			{
				p->setvalue(temp->getvalue(j),j);
				p->setOffset(temp->getOffset(j),j);
			}
			for(;j<N;j++)
			{
				newtree->setvalue(temp->getvalue(j),m);
				newtree->setOffset(temp->getOffset(j),m);
				m++;
			}
			//cout<<newtree->getvalue(1)<<" "<<p->getvalue(1)<<" "<<t->getvalue(1)<<endl;
			node* ot=insertpar(newtree->getvalue(0),newtree,p,t);	
			return ot;
		}
	}
}
//changepar(p,k,root,kk)
void changepar(node* p,string k,node* root,string k2){
	
	if(p==root) {
		int i=0;
		while(!(p->getvalue(i)==k) && i<N-1)
			i++;
		if(i<N-1)
		{
			p->setvalue(k2,i);
		}
		return;
	}
	node *p_parent=parent(p,root);//如果p不是根节点,否则p_parent=NULL；
	int i=0;
	while(!(p_parent->getvalue(i)==k) && i<N-1)
		i++;
	if(i<N-1)
	{
		p_parent->setvalue(k2,i);
		return;
	}
	else changepar(p_parent,k,root,k2);
		
}
node* deletereall(string k,node* p,node* root){
	int i=0; //在p结点中删除off和value 注意：要判断是leafnode还是normalnode！！！！！
	int where=0;//记录哪里被删掉了 
	string sss1="";
	if(p->whoareyou()==2)//leafnode
	{
		for(i=0;i<N-1;i++)
		{
			if(k==p->getvalue(i))
				break;
		}
		where=i; 
		//if(i==N-1) exit(-1);
		for(;i<N-2;i++)
		{
			if(p->getvalue(i+1).empty()) break;
			p->setOffset(p->getOffset(i+1),i);
			p->setvalue(p->getvalue(i+1),i);
		}
		p->setOffset(0,i);
		p->setvalue("",i);
	}
	else//normalnode
	{
		for(i=0;i<N-1;i++)
		{
			if(k==p->getvalue(i))
				break;
		}
		where=i;
		//if(i==N-1) exit(-1);
		for(;i<N-2;i++)
		{
			if(p->getvalue(i+1).empty()) break;
			p->setadree(p->getadree(i+2),i+1);//注意：预定了normalnode的第一个指针无法修改
			p->setvalue(p->getvalue(i+1),i);
		}
		p->setadree(NULL,i+1);
		p->setvalue("",i);
	}
	//p->setvalue("",i);  i空了
	if(p==root)//p是根节点 1.刚建树开始的叶节点 2.normalnode的结点
	{
		if(p->whoareyou()==2) //1.刚建树开始的叶节点
			return root;
		else //2.normalnode
		{
			if(i==0)//根节点只有一个子节点了
			{
				return p->getadree(0);
			}
			else return root;
		}
	}
	else//p不是根节点  1.有根的叶节点 2.normalnode的结点(树中间) 
	{
		if(p->whoareyou()==1 && i+1>=N/2 || p->whoareyou()==2 && i>=N/2) //内容不少  (修改)/////////////
		{
			if(where==0 && p->whoareyou()==2) //要修改父节点 
			{
				string kk=p->getvalue(0);
				changepar(p,k,root,kk);
			}
			return root; 
		}
		else//结点内容太少，1.合并 2.重新分布   是否考虑i=0？ 
		{
			int j=-1,num=0;
			string k1;
			node *p_parent=parent(p,root);//如果p不是根节点,否则p_parent=NULL；			
			node *p_bro=NULL;
			if(p_parent!=NULL)
			{
				for(j=0;j<N;j++)
				{
					if(p == p_parent->getadree(j))//存在value需要去的地方
						break;
				}
			}
			else //错误 
			{
				cout<<"error(no parent.)"<<endl;
				//exit(-1);
			}
			//p_parent->getadree(j)=p;
			if(j!=0) 
			{
				p_bro=p_parent->getadree(j-1);//左边兄弟 
				k1=p_parent->getvalue(j-1);
			}
			else 
			{
				p_bro=p_parent->getadree(j+1); //右边兄弟 
				k1=p_parent->getvalue(j);
			}
			while(!p_bro->getvalue(num).empty())//计算兄弟结点的key数  num个value 
			{
				num++;
				if(num==N-1) break;
			}
			if(p->whoareyou()==2 && i+num<=N-1 || p->whoareyou()==1 && i+num<=N-2)
			//可以合并  leafnode和normal可以合并的要求不一样 
			{
				if(p->whoareyou()==2)//叶节点 
				{
					if(j!=0)//和左边兄弟合并 
					{
						int m=0,q=num;
						while(!p->getvalue(m).empty())
						{
							p_bro->setvalue(p->getvalue(m),q);
							p_bro->setOffset(p->getOffset(m),q);
							q++;m++;
						}
						p_bro->setNext(p->getNext());
						//node* deletereall(string k,node* p,node* root)
						delete(p);	
						return deletereall(k1,p_parent,root); //deletp在哪？	
					} 
					else//和youbian兄弟合并 
					{
						int m=0,q=i;
						for(;m<num;q++,m++)
						{
							p->setvalue(p_bro->getvalue(m),q);
							p->setOffset(p_bro->getOffset(m),q);
						}
						p->setNext(p_bro->getNext());
						delete(p_bro);
						return deletereall(k1,p_parent,root); //deletp在哪？	
					}
				} 
				else //normalnode
				{
					if(j!=0)//和左边兄弟合并 
					{
						int m=0,q=num;
						p_bro->setvalue(k1,q);
						p_bro->setadree(p->getadree(0),q+1);
						q++;
						while(!p->getvalue(m).empty())
						{
							p_bro->setvalue(p->getvalue(m),q);
							p_bro->setadree(p->getadree(m+1),q+1);
							q++;
							m++;
						}
						delete(p);	
						return deletereall(k1,p_parent,root); //deletp在哪？	
					}
					else//和youbian兄弟合并 
					{
						int m=0,q=i;
						p->setvalue(k1,q);
						p->setadree(p_bro->getadree(0),q+1);
						q++;
						for(;m<num;q++,m++)
						{
							p->setvalue(p_bro->getvalue(m),q);
							p->setadree(p_bro->getadree(m+1),q+1);
						}
						delete(p_bro);
						return deletereall(k1,p_parent,root); //deletp在哪？	
					}
				} 
			}
			else//重新分配  记得i有可能等于0 
			{
				if(j!=0)//和左边兄弟合并 
				{
					if(p->whoareyou()==2)//叶节点 
					{
						int op=p_bro->getOffset(num-1);
						string s=p_bro->getvalue(num-1);
						p_bro->setOffset(0,num-1);
						p_bro->setvalue("",num-1);
						p=insertleaf(s,op,p);
						changepar(p,k1,root,s);
						return root;
					}
					else//normalnode
					{
						node* temp=p_bro->getadree(num);
						string s=p_bro->getvalue(num-1);
						p_bro->setadree(NULL,num);
						p_bro->setvalue("",num-1);
						for(int m=N-1;m>=2;m--)
						{
							p->setadree(p->getadree(m-1),m);
							p->setvalue(p->getvalue(m-2),m-1);
						}
						p->setadree(p->getadree(0),1);
						p->setadree(temp,0);
						p->setvalue(k1,0);
						changepar(p,k1,root,s);
						return root;	
					}
				}
				else//和you边兄弟合并 
				{
					if(p->whoareyou()==2)//叶节点 
					{					
						int m=0;
						int op=p_bro->getOffset(0);
						string s=p_bro->getvalue(1);
						string s1=p->getvalue(0);
						p->setOffset(p_bro->getOffset(0),i);
						p->setvalue(p_bro->getvalue(0),i);
						for(m=0;m<N-2;m++)
						{
							p_bro->setOffset(p_bro->getOffset(m+1),m);
							p_bro->setvalue(p_bro->getvalue(m+1),m);
						}
						p_bro->setOffset(0,m);
						p_bro->setvalue("",m);
						if(where==0) changepar(p,k,root,s1);
						changepar(p,k1,root,s);
						
						return root;
					}
					else//normalnode
					{
						int m=0;
						node* temp=p_bro->getadree(0);
						string s=p_bro->getvalue(0);
						p->setadree(temp,i+1);
						p->setvalue(k1,i);
						for(m=0;m<N-2;m++)
						{
							p_bro->setadree(p_bro->getadree(m+1),m);
							p_bro->setvalue(p_bro->getvalue(m+1),m);
						}
						p_bro->setadree(p_bro->getadree(m+1),m);
						p_bro->setadree(NULL,N-1);
						p_bro->setvalue("",N-2);
						changepar(p,k1,root,s);
						return root;	
					}
				}
			}
		}
	}
}

node* deletenode(string k,node* p){
	node* root=p;
	int i=-1; //找到包含K的结点
	while(p->Haveson())
	{
		for(i=0;i<N-1;i++)//遍历该节点
		{
			if(p->getvalue(i).empty())//空了
				break;
			if(k<= p->getvalue(i))//存在value需要去的地方
				break;
		}
		if(i==N-1) //该节点不符合
			p=p->getadree(i);
		else//在该节点中
		{
			if(k== p->getvalue(i))
				p=p->getadree(i+1);
			else
				p=p->getadree(i);
		}
	}
	for(i=0;i<N-1;i++)
	{
		if(p->getvalue(i) ==k)
			break;
	}
	if(i==N-1) return root; //没有该值
	return deletereall(k,p,root);
}

void display1(node *p,int q)
{
	if(p->whoareyou()==2)
	{
		for(int i=0;i<N-1;i++)
		{
			for(int j=0;j<q;j++)
				cout<<"--";
			cout<<i<<" "<<p->getvalue(i)<<endl;
		}
	}
	else
	{
		for(int i=0;i<N;i++)
		{
			if(p->getadree(i)!=NULL)
			{
				if(i!=N-1)
				{
					for(int j=0;j<q;j++)
						cout<<"--";
					cout<<"In "<<i<<" "<<p->getvalue(i)<<endl;		
				}
				else
				{
					for(int j=0;j<q;j++)
						cout<<"--";
					cout<<"In "<<i<<" NULL"<<endl;	
				}
				display1(p->getadree(i),q+1);
			}
		}
	}
}
int main()
{
	node* tree=new node;

	int answer=0;
	string s1="ahao";
	string s2="hao";
	string s3="czao";

tree=insert("0",1,tree);
tree=insert("1",1,tree);
tree=insert("2",1,tree);
tree=insert("3",1,tree);
tree=insert("4",1,tree);
tree=insert("5",1,tree);
tree=insert("6",1,tree);
tree=insert("7",1,tree);
tree=insert("8",1,tree);
tree=insert("9",1,tree);
tree=insert("10",1,tree);
tree=insert("11",1,tree);
tree=insert("12",1,tree);
tree=insert("13",1,tree);
tree=insert("14",1,tree);
tree=insert("15",1,tree);
tree=insert("16",1,tree);
tree=insert("17",1,tree);
tree=insert("18",1,tree);
tree=insert("19",1,tree);
tree=insert("20",1,tree);
tree=insert("21",1,tree);
tree=insert("22",1,tree);
tree=insert("23",1,tree);
tree=insert("24",1,tree);
tree=insert("25",1,tree);
tree=insert("26",1,tree);
tree=insert("27",1,tree);
tree=insert("28",1,tree);
tree=insert("29",1,tree);
tree=insert("30",1,tree);
tree=insert("31",1,tree);
tree=insert("32",1,tree);
tree=insert("33",1,tree);
tree=insert("34",1,tree);
tree=insert("35",1,tree);
tree=insert("36",1,tree);
tree=insert("37",1,tree);
tree=insert("38",1,tree);
tree=insert("39",1,tree);
tree=insert("40",1,tree);
tree=insert("a",1,tree);
tree=insert("b",2,tree);
tree=insert("c",-2,tree);
tree=insert("d",-2,tree);
tree=insert("e",3,tree);
tree=insert("f",-2,tree);
tree=insert("g",-2,tree);
tree=insert("h",-2,tree);
tree=insert("i",-2,tree);
tree=insert("j",-2,tree);
tree=insert("k",-2,tree);
tree=insert("l",1,tree);
tree=insert("m",1,tree);
tree=insert("n",1,tree);
tree=insert("o",1,tree);
tree=insert("p",1,tree);
tree=insert("q",1,tree);
tree=insert("r",1,tree);
tree=insert("7s",1,tree);
tree=insert("s",1,tree);
tree=insert("t",1,tree);
tree=insert("u",1,tree);
tree=insert("v",1,tree);
tree=insert("w",1,tree);
tree=insert("x",1,tree);
tree=insert("j",-2,tree);
tree=insert("cqq",-2,tree);
tree=insert("h3",-2,tree);
tree=insert("zzqt",-2,tree);
tree=insert("zzaqt",-2,tree);
tree=insert("zkop",-2,tree);
tree=insert("zetqewt1",-2,tree);
tree=insert("zaoqt",-2,tree);
tree=insert("c10vftn1",-2,tree);
tree=insert("c102",-2,tree);
tree=insert("qweqrwr",-2,tree);
tree=insert("14h",-2,tree);
tree=insert("q2wr",-2,tree);
tree=insert("vc1qe",-2,tree);
tree=insert("c2qwer",-2,tree);
tree=insert("cq",-2,tree);
tree=insert("h3",-2,tree);
tree=insert("vvzz",-2,tree);
tree=insert("zza",-2,tree);
tree=insert("zkwerop",-2,tree);
tree=insert("z1",-2,tree);
tree=insert("zao",-2,tree);
tree=insert("c101",-2,tree);
tree=insert("c103",-2,tree);
tree=insert("40",-2,tree);
tree=insert("99",1,tree);
tree=insert("0",1,tree);
tree=insert("1",1,tree);
tree=insert("2",1,tree);
tree=insert("3",1,tree);
tree=insert("4",1,tree);
tree=insert("5",1,tree);
tree=insert("6",1,tree);
tree=insert("7",1,tree);
tree=insert("8",1,tree);
tree=insert("9",1,tree);
tree=insert("10",1,tree);
tree=insert("11",1,tree);
tree=insert("12",1,tree);
tree=insert("13",1,tree);
tree=insert("14",1,tree);
tree=insert("15",1,tree);
tree=insert("16",1,tree);
tree=insert("17",1,tree);
tree=insert("18",1,tree);
tree=insert("19",1,tree);
tree=insert("20",1,tree);
tree=insert("21",1,tree);
tree=insert("22",1,tree);
tree=insert("41",1,tree);
tree=insert("42",1,tree);
tree=insert("43",1,tree);
tree=insert("44",1,tree);
tree=insert("45",1,tree);
tree=insert("46",1,tree);
tree=insert("47",1,tree);
tree=insert("48",1,tree);
tree=insert("49",1,tree);
tree=insert("50",1,tree);
tree=insert("51",1,tree);
tree=insert("52",1,tree);
tree=insert("53",1,tree);
tree=insert("54",1,tree);
tree=insert("55",1,tree);
tree=insert("56",1,tree);
tree=insert("57",1,tree);
tree=insert("58",1,tree);
tree=insert("59",1,tree);
tree=insert("60",1,tree);
tree=insert("61",1,tree);
tree=insert("62",1,tree);
tree=insert("63",1,tree);
tree=insert("64",1,tree);
tree=insert("65",1,tree);
tree=insert("66",1,tree);
tree=insert("67",1,tree);
tree=insert("68",1,tree);
tree=insert("69",1,tree);
tree=insert("70",1,tree);
tree=insert("71",1,tree);
tree=insert("72",1,tree);
tree=insert("73",1,tree);
tree=insert("74",1,tree);
tree=insert("75",1,tree);
tree=insert("76",1,tree);
tree=insert("77",1,tree);
tree=insert("78",1,tree);
tree=insert("79",1,tree);
tree=insert("80",1,tree);
tree=insert("81",1,tree);
tree=insert("82",1,tree);
tree=insert("83",1,tree);
tree=insert("84",1,tree);
tree=insert("85",1,tree);
tree=insert("86",1,tree);
tree=insert("87",1,tree);
tree=insert("88",1,tree);
tree=insert("89",1,tree);
tree=insert("90",1,tree);
tree=insert("91",1,tree);
tree=insert("92",1,tree);
tree=insert("93",1,tree);
tree=insert("94",1,tree);
tree=insert("95",1,tree);
tree=insert("96",1,tree);
tree=insert("97",1,tree);
tree=insert("98",1,tree);
tree=insert("99",1,tree);
tree=insert("96",1,tree);
tree=insert("97",1,tree);
tree=insert("98",1,tree);
tree=insert("99",1,tree);

tree=deletenode("0",tree);
tree=deletenode("1",tree);
tree=deletenode("2",tree);
tree=deletenode("3",tree);
tree=deletenode("4",tree);
tree=deletenode("0",tree);
tree=deletenode("1",tree);
tree=deletenode("2",tree);
tree=deletenode("3",tree);
tree=deletenode("4",tree);
tree=deletenode("5",tree);
tree=deletenode("6",tree);
tree=deletenode("7",tree);
tree=deletenode("8",tree);
tree=deletenode("9",tree);
tree=deletenode("10",tree);
tree=deletenode("11",tree);
tree=deletenode("12",tree);
tree=deletenode("13",tree);
tree=deletenode("14",tree);
tree=deletenode("15",tree);
tree=deletenode("16",tree);
tree=deletenode("17",tree);
tree=deletenode("18",tree);
tree=deletenode("19",tree);
tree=deletenode("20",tree);
tree=deletenode("21",tree);
tree=deletenode("22",tree);
tree=deletenode("23",tree);
tree=deletenode("24",tree);
tree=deletenode("25",tree);
tree=deletenode("26",tree);
tree=deletenode("27",tree);
tree=deletenode("28",tree);
tree=deletenode("29",tree);
tree=deletenode("30",tree);
tree=deletenode("31",tree);
tree=deletenode("32",tree);
tree=deletenode("33",tree);
tree=deletenode("34",tree);
tree=deletenode("35",tree);
tree=deletenode("36",tree);
tree=deletenode("37",tree);
tree=deletenode("38",tree);
tree=deletenode("39",tree);
tree=deletenode("40",tree);
tree=deletenode("a",tree);
tree=deletenode("b",tree);
tree=deletenode("c",tree);
tree=deletenode("d",tree);
tree=deletenode("e",tree);
tree=deletenode("f",tree);
tree=deletenode("g",tree);
tree=deletenode("h",tree);
tree=deletenode("i",tree);
tree=deletenode("j",tree);
tree=deletenode("k",tree);
tree=deletenode("l",tree);
tree=deletenode("m",tree);
tree=deletenode("n",tree);
tree=deletenode("o",tree);
tree=deletenode("p",tree);
tree=deletenode("q",tree);
tree=deletenode("r",tree);
tree=deletenode("7s",tree);
tree=deletenode("s",tree);
tree=deletenode("t",tree);
tree=deletenode("u",tree);
tree=deletenode("v",tree);
tree=deletenode("w",tree);
tree=deletenode("x",tree);
tree=deletenode("j",tree);
tree=deletenode("cqq",tree);
tree=deletenode("h3",tree);
tree=deletenode("zzqt",tree);
tree=deletenode("zzaqt",tree);
tree=deletenode("zkop",tree);
tree=deletenode("zetqewt1",tree);
tree=deletenode("zaoqt",tree);
tree=deletenode("c10vftn1",tree);
tree=deletenode("c102",tree);
tree=deletenode("qweqrwr",tree);
tree=deletenode("14h",tree);
tree=deletenode("q2wr",tree);
tree=deletenode("vc1qe",tree);
tree=deletenode("c2qwer",tree);
tree=deletenode("cq",tree);
tree=deletenode("h3",tree);
tree=deletenode("vvzz",tree);
tree=deletenode("zza",tree);
tree=deletenode("zkwerop",tree);
tree=deletenode("z1",tree);
tree=deletenode("zao",tree);
tree=deletenode("c101",tree);
tree=deletenode("c103",tree);
tree=deletenode("40",tree);
tree=deletenode("99",tree);
tree=deletenode("0",tree);
tree=deletenode("1",tree);
tree=deletenode("2",tree);
tree=deletenode("3",tree);
tree=deletenode("4",tree);
tree=deletenode("5",tree);
tree=deletenode("6",tree);
tree=deletenode("7",tree);
tree=deletenode("8",tree);
tree=deletenode("9",tree);
tree=deletenode("10",tree);
tree=deletenode("11",tree);
tree=deletenode("12",tree);
tree=deletenode("13",tree);
tree=deletenode("14",tree);
tree=deletenode("15",tree);
tree=deletenode("16",tree);
tree=deletenode("17",tree);
tree=deletenode("18",tree);
tree=deletenode("19",tree);
tree=deletenode("20",tree);
tree=deletenode("21",tree);
tree=deletenode("22",tree);
tree=deletenode("41",tree);
tree=deletenode("42",tree);
tree=deletenode("43",tree);
tree=deletenode("44",tree);
tree=deletenode("45",tree);
tree=deletenode("46",tree);
tree=deletenode("47",tree);
tree=deletenode("48",tree);
tree=deletenode("49",tree);
tree=deletenode("50",tree);
tree=deletenode("51",tree);
tree=deletenode("52",tree);
tree=deletenode("53",tree);
tree=deletenode("54",tree);
tree=deletenode("55",tree);
tree=deletenode("56",tree);
tree=deletenode("57",tree);
tree=deletenode("58",tree);
tree=deletenode("59",tree);
tree=deletenode("60",tree);
tree=deletenode("61",tree);
tree=deletenode("62",tree);
tree=deletenode("63",tree);
tree=deletenode("64",tree);
tree=deletenode("65",tree);
tree=deletenode("66",tree);
tree=deletenode("67",tree);
tree=deletenode("68",tree);
tree=deletenode("69",tree);
tree=deletenode("70",tree);
tree=deletenode("71",tree);
tree=deletenode("72",tree);
tree=deletenode("73",tree);
tree=deletenode("74",tree);
tree=deletenode("75",tree);
tree=deletenode("76",tree);
tree=deletenode("77",tree);
tree=deletenode("78",tree);
tree=deletenode("79",tree);
tree=deletenode("80",tree);
tree=deletenode("81",tree);
tree=deletenode("82",tree);
tree=deletenode("83",tree);
tree=deletenode("84",tree);
tree=deletenode("85",tree);
tree=deletenode("86",tree);
tree=deletenode("87",tree);
tree=deletenode("88",tree);
tree=deletenode("89",tree);
tree=deletenode("90",tree);
tree=deletenode("91",tree);
tree=deletenode("92",tree);
tree=deletenode("93",tree);
tree=deletenode("94",tree);
tree=deletenode("95",tree);
tree=deletenode("96",tree);
tree=deletenode("97",tree);
tree=deletenode("98",tree);
display1(tree,0);
cout<<"--------------"<<endl;
tree=deletenode("99",tree);



display1(tree,0);
cout<<"--------------"<<endl;

	



	cin>>s1;
	return 0;
}
