#include "BufferManager.h" 

Block* MBuffer::GBlock()
{
	Block* Tmp;
	Tmp = new Block;
	Tmp->record = new char[BlockSize];
	memset(Tmp->record,0,BlockSize);
	Tmp->tablename = "";
	Tmp->offset = 0;
	Tmp->accessed = false;
	Tmp->written = false;	
	return Tmp;
}

bool MBuffer::Init()
{
	int i;
	for(i=0;i<64;i++)
	{
		Buffer[i] = GBlock();
		if(Buffer[i] == NULL || Buffer[i]->record == NULL)
			return false;
	}
	return true;
}

Block* MBuffer::GetBlock(string tablename, int offset, int flag)
{
	Block* Target;
	int i;
	for(i=0;i<64;i++)
	{
		Target = Buffer[i];
		if(Target->tablename == tablename && Target->offset == offset)
		{
			Target->accessed = true;
			return Target;
		}
	}
	if(flag == 0)
	{ 	
		if(Sche1(tablename,offset))
		{
			for(i=0;i<64;i++)
			{
				Target = Buffer[i];
				if(Target->tablename == tablename && Target->offset == offset)
				{
					Target->accessed = true;
					return Target;
				}
			}
		}
	} 
	else
	{
		if(Sche2(tablename,offset))
		{
			for(i=0;i<64;i++)
			{
				Target = Buffer[i];
				if(Target->tablename == tablename && Target->offset == offset)
				{
					Target->accessed = true;
					return Target;
				}
			}
		}
	} 
	return NULL;
}

bool MBuffer::Sche1(string tablename, int offset)
{
	int i = 0;
	for(i=0;i<64;i++)
	{
		if(Buffer[i]->tablename == "")
		{
			break;			
		}
	}
	if(i==64)
	{
		i = 0; 
		while(1)
		{
			if(Buffer[i]->accessed == false)
				break;
			else
				Buffer[i]->accessed = false;
			i++;
			if(i == 64)
				i = 0;	
		}
	}
	Exchange(tablename, offset, Buffer[i]);
	if(Buffer[i] != NULL)
		return true;
	else 
		return false;
}

bool MBuffer::Sche2(string tablename, int offset)
{
	int i = 0;
	for(i=0;i<64;i++)
	{
		if(Buffer[i]->tablename == "")
		{
			break;		
		}
	}
	if(i==64)
	{
		i = 0;
		while(1)
		{
			if(Buffer[i]->accessed == false)
				break;
			else
				Buffer[i]->accessed = false;
			i++;
			if(i == 64)
				i = 0;	
		}
	}
	Exchange("",-1,Buffer[i]);
	Buffer[i]->tablename = tablename;
	Buffer[i]->accessed = true;
	Buffer[i]->written = true;
	Buffer[i]->offset = offset;
	memset(Buffer[i]->record,0,BlockSize);
	Exchange("",-1,Buffer[i]);
	if(Buffer[i] != NULL)
		return true;
	else 
		return false;
}

bool MBuffer::Drop(string tablename)
{
	Block* Target;
	int i;
	for(i=0;i<64;i++)
	{
		Target = Buffer[i];
		if(Target->tablename == tablename)
		{
			Buffer[i] = GBlock();
			if(Buffer[i] == NULL || Buffer[i]->record == NULL)
				return false;
			delete Target->record;
			delete Target;
		}
	}
	return true;
}

bool  MBuffer::Exchange(string tablename, int offset, Block* Replaced)
{	
	FILE* fp;
	int i;
	char Filename[64];
	memset(Filename,0,sizeof(Filename));
	int length = 0;
	length = Replaced->tablename.size();
	if(length!=0)
	{
		for(i=0;i<length;i++)
		{
			Filename[i] = Replaced->tablename.at(i);
		}
		if((fp = fopen(Filename,"rb+"))==NULL)
		{
			cout<<"Writing data error!"<<endl;
			return false;
		} 
		fseek(fp,BlockSize*Replaced->offset,SEEK_SET);
		fwrite(Replaced->record,BlockSize,1,fp);
		fclose(fp);
	}
	memset(Filename,0,sizeof(Filename));
	if(offset!=-1)
	{
		length = tablename.size();
		for(i=0;i<length;i++)
		{
			Filename[i] = tablename.at(i);
		}
		if((fp = fopen(Filename,"rb"))==NULL)
		{
			cout<<"Reading data error!"<<endl;
			return false;
		} 
		fseek(fp,BlockSize*offset,SEEK_SET);
		fread(Replaced->record,BlockSize,1,fp);		
		fclose(fp);
		Replaced->offset = offset;
		Replaced->accessed = true;
		Replaced->tablename = tablename;
		return true;
	}
	else
		return true;
}
 
MBuffer::~MBuffer()
{
	Block* Target;
	int i;
	for(i=0;i<64;i++)
	{
		Target=Buffer[i];
		
		if(Target!=NULL)
		{
			if(Target->tablename != "")
				Exchange("",-1,Target);
			delete Target->record;
			delete Target;
		}
	}
}
void MBuffer::test(){
	int i;
//	for(i=0;i<64;i++)
//	{
		printf("%s",Buffer[0]->record);
//	}
//if(Buffer[1]->record[10]==0)
//cout<<"ok"<<endl;
	
}
int Block_num(string file_name)
{
	int num = 0;
	FILE *fp;
	fp = fopen(file_name.c_str(),"r");
	fseek(fp, 0, 2);  
    num = ftell(fp)/BlockSize;  
    fclose(fp);
    return num;
}
