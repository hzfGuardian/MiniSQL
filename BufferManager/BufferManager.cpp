#include "Buffermanager.h" 
Block* MBuffer::GBlock()
{
	Block* Tmp;
	Tmp = new Block;
	Tmp->record = new char[4048];
	memset(Tmp->record,0,4048);
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

Block* MBuffer::GetBlock(string tablename, int offset)
{
	Block* Target;
	int i;
	for(i=0;i<64;i++)
	{
		Target = Buffer[i];
		if(Target->tablename == tablename && Target->offset == offset)
			return Target;
	}
	//cout<<tablename<<endl;
	if(Sche(tablename,offset))
	{
		for(i=0;i<64;i++)
		{
			Target = Buffer[i];
			if(Target->tablename == tablename && Target->offset == offset)
				return Target;
		}
	}
	//cout<<"ok"<<endl;
	return NULL;
}

bool MBuffer::Sche(string tablename, int offset)
{
	int i = 0;
	Block* Target;
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

	Buffer[i] = Exchange(tablename, offset, Target);
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
{	cout<<"ok"<<endl;
	FILE* fp;
	int i;
	char Filename[64];
	memset(Filename,0,sizeof(Filename));
	int length;
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
			return NULL;
		} 
		fseek(fp,4048*Replaced->offset,SEEK_SET);
		fwrite(Replaced->record,4048,1,fp);
		fclose(fp);
		delete Replaced->record;
		delete Replaced;
	}
	memset(Filename,0,sizeof(Filename));
	if(offset!=-1)
	{
		length = tablename.size();
		for(i=0;i<length;i++)
		{
			Filename[i] = tablename.at(i);
		}
		printf("%s",Filename);
		if((fp = fopen(Filename,"r"))==NULL)
		{
			cout<<"Reading data error!"<<endl;
			return NULL;
		} 
		Block* Target = GBlock(); 
		fseek(fp,4048*offset,SEEK_SET);
		fread(Target->record,4048,1,fp);
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
			if(Target->written == true)
				Exchange(Target->tablename,-1,Target);
			delete Target->record;
			delete Target;
		}
	}
}
void MBuffer::test(){
	int i;
	for(i=0;i<64;i++)
	{
		printf("%s\n",Buffer[i]->record);
	}
	
}
