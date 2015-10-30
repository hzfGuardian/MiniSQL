#include "IndexManager.h"
extern MBuffer m1;

//automatically create index on primary key
void Create_index(string index_name, int attr_type){
	Block* b;
	int n=attr_type;
	int firstplace=1;
	b=m1.GetBlock(index_name,0,1);
	memcpy(b->record,&n,4);
	memcpy(b->record+4,&firstplace,4);	
	b=m1.GetBlock(index_name,1,1);
	int who=2;
	memcpy(b->record,&n,4);
	memcpy(b->record+4,&who,4);	
}

//CREATE INDEX NAME ON NAME '(' NAME ')'
void Create_index(vector<a> values, string index_name, int attr_type){
	Create_index(index_name,attr_type);
	for(int i=0;i<values.size();i++)
	{
		Insert_index(index_name, values[i].s, values[i].off);
	}
}

void Insert_index(string index_name, string value, int offset){
	Block* b=m1.GetBlock(index_name,0,0);
	int type=0;
	int root=0;
	memcpy(&type,b->record,4);
	memcpy(&root,b->record+4,4);
	root=insert(value,offset,root,index_name);	
	b=m1.GetBlock(index_name,0,0);
	memcpy(b->record+4,&root,4);
	return;
}

void Delete_index(string index_name, string value){
	Block* b=m1.GetBlock(index_name,0,0);
	int type=0;
	int root=0;
	memcpy(&type,b->record,4);
	memcpy(&root,b->record+4,4);
	root=deletenode(value,root,index_name);	
	b=m1.GetBlock(index_name,0,0);
	memcpy(b->record+4,&root,4); 
	return;
}


vector<int> Find_indices(string index_name,string op, string value){ 
	vector<int> ans;
	Block* b=m1.GetBlock(index_name,0,0);
	int root=0;
	memcpy(&root,b->record+4,4);
	//find1(root,value,index_name);
	if(op=="=")
	{
		int i=-1;
		i=find1(root,value,index_name);
		if(i!=-1)  ans.push_back(i);
	}
	else if(op==">=")
	{
		ans=findbigthan(root,value,index_name,1);	
	} 
	else if(op=="<=")
	{
		ans=findsmallthan(root,value,index_name,1);	
	} 
	else if(op==">")
	{
		ans=findbigthan(root,value,index_name,0);	
	}
	else if(op=="<")
	{
		ans=findsmallthan(root,value,index_name,0);	
	}
	else if(op=="<>")
	{
		ans=findnoeq(root,value,index_name);	
	}
	return ans;
} 
void dis(string filename)
{
	Block* b=m1.GetBlock(filename,0,0);
	int type=0;//?
	int root=0;
	memcpy(&type,b->record,4);
	memcpy(&root,b->record+4,4);
	cout<<"type: "<<type<<endl;
	cout<<"root: "<<root<<endl;
	node *p=getnode(root,filename);
	disre(p,0,filename);
}
void disre(node *p,int q,string filename)
{
	int N=(p->gettype()==2)?15:501;
	if(p->whoareyou()==2)
	{
		for(int i=0;i<N-1;i++)
		{
			for(int j=0;j<q;j++)
				cout<<"-";
			cout<<i<<" "<<p->getvalue(i)<<" "<<p->getOffset(i)<<endl;
		}
	}
	else
	{
		for(int i=0;i<N;i++)
		{
			if(p->getadree(i)!=0)
			{
				if(i!=N-1)
				{
					for(int j=0;j<q;j++)
						cout<<"-";
					cout<<"In "<<i<<" "<<p->getvalue(i)<<endl;		
				}
				else
				{
					for(int j=0;j<q;j++)
						cout<<"-";
					cout<<"In "<<i<<" NULL"<<endl;	
				}
				disre(getnode(p->getadree(i),filename),q+1,filename);
			}
		}
	}
}

int main()
{
	m1.Init();
	a atest;
	atest.off=1;
	atest.s="nihao";
	string filename="index1.txt";
	vector<int> nima;
Create_index(filename,2);

Insert_index(filename,"puvl",3);
Insert_index(filename,"gsam",8);
Insert_index(filename,"xysg",-1);
Insert_index(filename,"ktlw",30);
Insert_index(filename,"mzbd",4);
Insert_index(filename,"qjci",1);
Insert_index(filename,"sgew",-2);
Insert_index(filename,"wtue",444);
Insert_index(filename,"yhmf",7);
Insert_index(filename,"lrgt",18);
Insert_index(filename,"ydgb",-3);
Insert_index(filename,"wprh",9);
Insert_index(filename,"elbc",34);
Insert_index(filename,"spmv",377);

Insert_index(filename,"gnpv",3);

Insert_index(filename,"qsyh",3);

Insert_index(filename,"aclb",3);
Insert_index(filename,"zgvd",3);
Insert_index(filename,"zqpf",3);
Insert_index(filename,"cvbt",3);
Insert_index(filename,"idni",3);
Insert_index(filename,"lxcw",3);
Insert_index(filename,"nvzy",3);
Insert_index(filename,"xucr",3);
Insert_index(filename,"glml",3);
Insert_index(filename,"vdjl",3);
Insert_index(filename,"wiww",3);
Insert_index(filename,"zqwc",3);
Insert_index(filename,"kqoi",3);
Insert_index(filename,"yfrr",3);
Insert_index(filename,"ncth",3);
Insert_index(filename,"iego",3);
Insert_index(filename,"xoii",3);
Insert_index(filename,"xomj",3);
Insert_index(filename,"lsjq",3);
Insert_index(filename,"gmkp",3);
Insert_index(filename,"gued",3);
Insert_index(filename,"rdvc",3);
Insert_index(filename,"dddq",3);
Insert_index(filename,"pmtd",3);
Insert_index(filename,"vkrk",3);
Insert_index(filename,"ehkf",3);
Insert_index(filename,"yidn",3);
Insert_index(filename,"oyou",3);
Insert_index(filename,"zxce",3);
Insert_index(filename,"dvwx",3);
Insert_index(filename,"wfdw",3);
Insert_index(filename,"trfg",3);
Insert_index(filename,"bwdc",3);
Insert_index(filename,"lqre",3);
Insert_index(filename,"usqh",3);
Insert_index(filename,"kkub",3);
Insert_index(filename,"gpon",3);
Insert_index(filename,"orlb",3);
Insert_index(filename,"jmhz",3);
Insert_index(filename,"yqpw",3);
Insert_index(filename,"ofaa",3);
Insert_index(filename,"jhdl",3);
Insert_index(filename,"xdgt",3);
Insert_index(filename,"xpaj",3);
Insert_index(filename,"zsgm",3);
Insert_index(filename,"ppsp",3);
Insert_index(filename,"ngxb",3);
	dis(filename);
	cout<<"--------------------"<<endl;

//	Delete_index(filename,"gsam");
nima=Find_indices(filename,"<", "wtue");
cout<<"an:"<<endl;
for(int i=0;i<nima.size();i++)
cout<<nima[i]<<endl;	
	
	
	
	

Delete_index(filename,"puvl");
Delete_index(filename,"gsam");
Delete_index(filename,"xysg");
Delete_index(filename,"ktlw");
Delete_index(filename,"mzbd");
Delete_index(filename,"qjci");
Delete_index(filename,"sgew");
Delete_index(filename,"wtue");
Delete_index(filename,"yhmf");
Delete_index(filename,"lrgt");
Delete_index(filename,"ydgb");
Delete_index(filename,"wprh");
Delete_index(filename,"elbc");
Delete_index(filename,"spmv");
Delete_index(filename,"gnpv");
Delete_index(filename,"qsyh");
Delete_index(filename,"aclb");
Delete_index(filename,"zgvd");
Delete_index(filename,"zqpf");
Delete_index(filename,"cvbt");
Delete_index(filename,"idni");
Delete_index(filename,"lxcw");
Delete_index(filename,"nvzy");
Delete_index(filename,"xucr");
Delete_index(filename,"glml");
Delete_index(filename,"vdjl");
Delete_index(filename,"wiww");
Delete_index(filename,"zqwc");
Delete_index(filename,"kqoi");
Delete_index(filename,"yfrr");
Delete_index(filename,"ncth");
Delete_index(filename,"iego");
Delete_index(filename,"xoii");
Delete_index(filename,"xomj");
Delete_index(filename,"lsjq");
Delete_index(filename,"gmkp");
Delete_index(filename,"gued");
Delete_index(filename,"rdvc");
Delete_index(filename,"dddq");
Delete_index(filename,"pmtd");
Delete_index(filename,"vkrk");
Delete_index(filename,"ehkf");
Delete_index(filename,"yidn");
Delete_index(filename,"oyou");
Delete_index(filename,"zxce");
Delete_index(filename,"dvwx");
Delete_index(filename,"wfdw");
Delete_index(filename,"trfg");
Delete_index(filename,"bwdc");
Delete_index(filename,"lqre");
Delete_index(filename,"usqh");
Delete_index(filename,"kkub");
Delete_index(filename,"gpon");
Delete_index(filename,"orlb");
Delete_index(filename,"jmhz");
Delete_index(filename,"yqpw");
Delete_index(filename,"ofaa");
Delete_index(filename,"jhdl");
Delete_index(filename,"xdgt");
Delete_index(filename,"xpaj");
Delete_index(filename,"zsgm");
Delete_index(filename,"ppsp");
Delete_index(filename,"ngxb");


		dis(filename);
	cout<<"--------------------"<<endl;

//	Delete_index(filename,"gsam");
nima=Find_indices(filename,"<", "wtue");
cout<<"an:"<<endl;
for(int i=0;i<nima.size();i++)
cout<<nima[i]<<endl;	
/*	Block* b=m1.GetBlock(filename,0,0);
	int type=-1;//?
	int root=0;
	memcpy(&type,b->record,4);
	memcpy(&root,b->record+4,4);

	//insertleaf("QFGEWqw",10,1,filename);
	/*
	b=m1.GetBlock(filename,2,1);
	memcpy(b->record,&n,4);
	memcpy(b->record+4,&f,4);
	
	insertleaf("nihao",2,2,filename);
	insertleaf("q",-10,2,filename);
	 //cout<<Block_num(filename);*/
	return 0;
}




