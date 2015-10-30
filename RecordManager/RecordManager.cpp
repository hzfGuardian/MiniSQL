#include "RecordManager.h"
 
MBuffer mybuffer;

char* Convertvalue(Tuple &tuple)
{
	string value = "";
	char *p = new char[tuple.length()];
	memset(p, 0, 11);
	char *v = p;
	char *tmp = new char[4];
	int ivalue;
	float fvalue;
	for(int i=0;i<tuple.attr_count;i++)
	{
		if(tuple.attrs[i].attr_type == CHAR)
		{
			strcpy(v,tuple.attr_values[i].data());
			v += tuple.attr_values[i].size();
		}
		if(tuple.attrs[i].attr_type == INT)
		{
			ivalue = atoi(tuple.attr_values[i].c_str());
			*((int*) tmp) = ivalue;			
			memcpy(v,tmp,4);
			v += 4;
		}
		if(tuple.attrs[i].attr_type == FLOAT)
		{	
			fvalue = (float) atof(tuple.attr_values[i].c_str());
			*((float*) tmp) = fvalue;			
			memcpy(v,tmp,4);
			v += 4;
		}			
	}
	return p;
}

bool Insert(Tuple &tuple)
{
	int i;
	mybuffer.Init();
	char *values = new char[tuple.length()+2];
	values = Convertvalue(tuple);
	values[tuple.length()] = '1';
	values[tuple.length()+1]=0;
	string file_name = tuple.table_name + "_table.rec";	
	int offset = Block_num(file_name)-1;
	if(offset<0)
	{
		Block* newblock = mybuffer.GetBlock(file_name,0,1);
		memcpy(newblock->record,values,(tuple.length()+1));
		int testi = 0;
		float testf = 0;
		memcpy(&testi, mybuffer.Buffer[0]->record, 4);
		memcpy(&testf, mybuffer.Buffer[0]->record+4, 4);
		cout<<"1:"<<testi<<" "<<testf<<endl;
		return true;
	}
	else
	{
		Block* Target = mybuffer.GetBlock(file_name,offset,0);
		for(i=tuple.length();i<BlockSize;i+=tuple.length()+1)
		{
			if(Target->record[i] == 0)
				break;
		}
		cout<<Target->record[8]<<i<<endl;
		if(i<BlockSize)
		{
			memcpy(Target->record+i-tuple.length(),values,tuple.length()+1);
			cout<<Target->record[20]<<endl;
			Target->accessed = true;
			Target->written = true;
			return true; 
		} 
		else
		{
			cout<<"**************"<<endl;
			Block* newblock = mybuffer.GetBlock(file_name,offset+1,1);
			memcpy(newblock->record,values,tuple.length()+1);
			return true;
		}
	}
	return false;
}

/*bool Delete_tuple(vector<int> offsetlist, string table_name, Conditionlist list)
{
	bool flag;
	if(offsetlist[0]==-1)
	{
		int offset=0;
		int Block_num;
		Block_num = Block_num(table_name);
		if(Block_num!=0)
		{
			Block * Target;
			while(offset<Block_num)
			{
				Target = mybuffer.GetBlock(table_name,offset,0);
				
			}
		}
	}
} */
