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
				//cout<<"ok"<<endl;
				Target->accessed=true; 
				return Target;
			}
	}
	
	if(flag == 0)
	{ 
		if(Sche(tablename,offset))
		{
			for(i=0;i<64;i++)
			{
				Target = Buffer[i];
				if(Target->tablename == tablename && Target->offset == offset)
					{
						Target->accessed=true; 
						return Target;
					}
			}
		}
	} 
	else
	{
		Target = GBlock();
		Target->tablename = tablename;
		Target->accessed = true;
		Target->written = true;
		Target->offset = offset;
		Sche(Target);
		return Target;
	} 
	return NULL;
}

bool MBuffer::Sche(string tablename, int offset)
{
	int i = 0;
	Block* Target;
	for(i=0;i<64;i++)
	{
		if(Buffer[i]->tablename == "")
		{
			Target = Buffer[i];
			break;		
		}
	}
	if(i==64)
	{
		i=0;
		while(1)
		{
			Target = Buffer[i];
			if(Target->accessed == false)
				break;
			else
				Buffer[i]->accessed = false;
			i++;
			if(i == 64)
				i = 0;	
		}
	}
	Buffer[i] = Exchange(tablename, offset, Target);
	if(Buffer[i] != NULL)
		return true;
	else 
		return false;
}

bool MBuffer::Sche(Block* newblock)
{
	int i = 0;
	Block* Target;
	for(i=0;i<64;i++)
	{
		if(Buffer[i]->tablename == "")
		{
			Target = Buffer[i];
			break;		
		}
	}
	if(i==64)
	{
		i=0;
		while(1)
		{
			Target = Buffer[i];
			if(Target->accessed == false)
				break;
			else
				Buffer[i]->accessed = false;
			i++;
			if(i == 64)
				i = 0;	
		}
	}
	if(Buffer[i]->tablename!="")
		Exchange("",-1,Buffer[i]);
	Buffer[i] = newblock;
	Exchange("",-1,newblock);
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

Block*  MBuffer::Exchange(string tablename, int offset, Block* Replaced)
{	
	FILE* fp;
	int i;
	char Filename[64];
	memset(Filename,0,sizeof(Filename));
	int length = 0;
	length = Replaced->tablename.size();
	//cout<<length<<endl;
	if(length!=0)
	{
		//cout<<"ok"<<endl;
		for(i=0;i<length;i++)
		{
			Filename[i] = Replaced->tablename.at(i);
		}
		if((fp = fopen(Filename,"rb+"))==NULL)
		{
			cout<<"Writing data error!"<<endl;
			return NULL;
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
			return NULL;
		} 
		Block* Target = GBlock(); 
		fseek(fp,BlockSize*offset,SEEK_SET);
		fread(Target->record,BlockSize,1,fp);		
		fclose(fp);
		Target->offset = offset;
		Target->accessed = true;
		Target->tablename = tablename;
		return Target;
	}
	else
		return NULL;
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
			//cout<<"w"<<endl;
			if(Target->tablename != "")
				{//cout<<Target->tablename<<endl;
				Exchange("",-1,Target);}
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
	//cout<<ftell(fp)<<endl;
    num = ftell(fp)/BlockSize;  
    fclose(fp);
    return num;
}
MBuffer m1;
