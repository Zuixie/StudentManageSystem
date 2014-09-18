
//可以设置公共课程，然后是个人选修课
//私有课程设置放在修改信息中...
//无文件是的操作情况
//文件为空的时候操作情况
#include<iostream>
#include<string>
#include<fstream>						//文件的输出输出流操作
#include<iomanip>						//对齐输出
using namespace std;
const char* filename="Student.txt";			//学生信息记录的文件名称
const char* filename_lesson="Lesson.txt";	//公共课程信息的记录名称
const int MAX=10;							//最多课程数量
struct Lesson_Node							//课程的结构体设置
{
	string L_name;							//课程名称
	int grade;								//课程成绩
};
class Lesson								//课程设置类
{
private:
	Lesson_Node lesson[MAX];				//课程具体信息，最多10项
	int n;									//当前的课程数量
	int Mount_s;							//成绩总和
	int Averg_s;							//平均成绩
public:
	Lesson()								//课程初始化，读取文件中公共课程的信息设置,如果没有文件就建立一个文件
	{
		ifstream ifs(filename_lesson);
		if(!ifs)
		{	
			string na[4]={"语文","数学","英语","体育"};
			n=4;
			for(int i=0;i<n;++i)
			{
				lesson[i].L_name=na[i];
				lesson[i].grade=0;
			}
			SetA_M();
			Filein_PublicL();				//如果打开失败，就写一个文件
		}
		else
		{
			ifs.close();
			Fileout_PbulicL();				//读取成功就读取当前文件中的课程设置
			
		}
	}
	~Lesson()								//析构
	{
		n=0;
		Mount_s=0;
		Averg_s=0;
	}
	Lesson(const Lesson&rhs)				//复制构造函数
	{
		n=rhs.n;
		for(int i=0;i<n;i++)
		{
			lesson[i].L_name=rhs.lesson[i].L_name;
			lesson[i].grade=rhs.lesson[i].grade;
			SetA_M();
		}
	}
	Lesson& operator=(const Lesson&rhs)
	{
		n=rhs.n;
		for(int i=0;i<n;i++)
		{
			lesson[i].L_name=rhs.lesson[i].L_name;
			lesson[i].grade=rhs.lesson[i].grade;
			SetA_M();
		}
		return *this;
	}
	int ReturnMount_s()						//返回总分
	{
		return Mount_s;
	}
	int ReturnAverg_s()						//返回平均分
	{
		return Averg_s;
	}
	void Dislesson(int x=1)					//输出当前的课程的信息
	{
		if(x==1)							//由于需要，在没有输入参数的情况下默认是输出文件中的课程信息，在参数x不等于1则输出当前内存中修改后的课程程信息
			Fileout_PbulicL();
		int i;
		for(i=0;i<n;i++)
		{
			cout<<i+1<<lesson[i].L_name<<"  ";
		}
		cout<<endl;
	}
	void Addlesson(int i=1)					//增加课程 参数1是公共课程的添加 2是私人课程的添加
	{
		
		char t='1';
		string na;
		while(t=='1')
		{
			if(n==MAX)
			{
				cout<<"以达到课程上限，无法继续添加"<<endl;
				return ;
			}
			cout<<"输入要添加的课程名称：";
			cin>>na;
			lesson[n].L_name=na;
			if(i==1)
			lesson[n].grade=0;
			else
			{
				cout<<"输入成绩";
				cin>>na;
				lesson[n].grade=atoi(na.c_str());
			}
			n++;							//课程数量加1
			
			cout<<"是否继续添加课程：（1.继续 0.不了）";
			cin>>t;
		}
		if(i==1)
		Filein_PublicL();//将修改后的课程信息写入文件
	}	
	void Dellesson(int i=1)					//删除课程
	{
		int x;string tmp;char t='1';
		while(t=='1')
		{
			cout<<"请输入要删除的课程编号";
			cin>>tmp;x=atoi(tmp.c_str());
			if(x>0&&x<=n)
			{
				for(;x<n;x++)
				{
					lesson[x-1].L_name=lesson[x].L_name;
					lesson[x-1].grade=lesson[x].grade;
				}
				n--;
				if(i==1)
				Dislesson(0);				//显示当前内存中的课程信息，避免一直循环删除要一直读取写入文件，所以一次性将修改好的课程写入文件
			}
			else
			{
				cout<<"输入错误~~~，或已经不存在课程信息了"<<endl;
			}
			cout<<"是否继续删除课程信息（1.是的，0.不了） ";
			cin>>t;
		}
		if(i==1)
		Filein_PublicL();					//写入文件
	}
	void SetA_M()							//计算平均分和总分
	{
		Mount_s=0;
		for(int i=0;i<n;i++)
		{
			Mount_s=Mount_s+lesson[i].grade;
		}
		Averg_s=Mount_s/n;
	}
	/*******************************************     个人选修课程设置     *********************************************/
	void SetPersonLesson()
	{
		string tmp;
		char x='0';
		int i;
		Dislesson(2);  cout<<endl;                             //输出当前学生信息中的课程信息
		cout<<"a.增加课程 b.删除课程";
		cin>>x;
		switch (x)
		{
		case 'a':Addlesson(2);break;							//增加单人课程
		case 'b':Dellesson(2);break;							//删除单人课程
		default:
			i=x-'0';
			if(i>0&&i<=n)
			{
				cout<<"请输入"<<lesson[i-1].L_name<<"的成绩：";
				cin>>tmp;
				lesson[i-1].grade=atoi(tmp.c_str());//成绩从字符转整形
				
			}
			break;
		}SetA_M();
	}
	/*******************************************     公共课程设置部分     **********************************************/
	
	void Filein_PublicL()				//向文件中输入公共课程的设置
	{
		ofstream ofs(filename_lesson);
		if(!ofs)
		{
			cout<<"文件打开失败"<<endl;
			exit(0);
		}
		ofs<<n<<'\t';
		for(int i=0;i<n;i++)
		{
			ofs<<lesson[i].L_name<<'\t';
		}
		ofs.close();

	}
	void Fileout_PbulicL()				//从文件中读取公共课程设置信息，建立课程
	{
		int i=0;
		ifstream ifs(filename_lesson);
		if(!ifs)
		{
			cout<<"文件打开失败"<<endl;
			exit(0);
		}
		ifs>>n>>ws;
		while(i<n&&!ifs.eof())			//判断是否读取到文件末尾了
		{
			ifs>>lesson[i].L_name>>ws;
			i++;
		}
		ifs.close();
	}
	void SetPbulicLesson()				//设置当前的公共课程的信息
	{
		cout<<"当前公共课程信息："<<endl;//显示当前的课程信息便于用来
		Dislesson();
		cout<<"1.添加课程  2.删除课程  0.返回 ";
		string tmp;char t;
		cin>>t;
		switch(t)
		{
		case '1':
			Addlesson();
			break;
		case '2':
			Dellesson();
			break;
		case '0':break;
		}
	}
	friend istream& operator>>(istream& is,Lesson &rhs)//输入成绩
	{
		string tmp;
		for(int i=0;i<rhs.n;i++)
		{
			cout<<"请输入"<<rhs.lesson[i].L_name<<"的成绩：";
			is>>tmp;
			rhs.lesson[i].grade=atoi(tmp.c_str());//成绩从字符转整形
		}
		return is;
	}
	friend ostream& operator<<(ostream& os,Lesson &rhs)//输出成绩
	{
		for(int i=0;i<rhs.n;i++)
		{
			cout<<"|"<<rhs.lesson[i].L_name<<"|\t   "<<rhs.lesson[i].grade<<"\t";
		}
		cout<<endl;
		return os;
	}
	friend class student_Node;
	friend class student_List;
};
/*****************************************************************************************************************************/
Lesson L;//***********************************************************************************公共课程变量
class student_Node		//学生的信息类，存储学生的信息，并修改信息
{
private:
	string Name;		//姓名
	long Number;		//学号
	string Sex;			//性别
	string Academy;		//学院
	string Class;		//班级
	Lesson Less;        //课程
	student_Node *next;
public:
	student_Node(string na="空",long num=0,string sex="男",string aca="空",string cla="空")		//对成绩和姓名所以成员都进行初始化??需要优化?
		:Less(L)
	{
		SetName(na);
		SetNumber(num);
		SetSex(sex);
		SetAcademy(aca);
		SetClass(cla);
		next=NULL;
	}
	~student_Node()					//析构
	{
		Name="";Number=0;Sex="";Academy="";Class="";next=NULL;
	}
	student_Node& operator =(const student_Node&rhs)
	{
		Name=rhs.Name;
		Number=rhs.Number;
		Sex=rhs.Sex;
		Academy=rhs.Academy;
		Class=rhs.Class;
		Less=rhs.Less;//这里又调用课程类的赋值函数
		return *this;
	}
	void SetName(string name)		//设置名字
	{
		Name=name;
	}
	void SetNumber(long num )		//设置学号
	{
		Number=num;
	}
	void SetSex(string sex)			//设置性别
	{
		Sex=sex;
	}
	void SetAcademy(string acad)	//设置学院
	{
		Academy=acad;
	}
	void SetClass(string cla )		//设置班级
	{
		Class=cla;
	}
	void SetLesson()				//单人课程设置
	{
		Less.SetPersonLesson();
	}
	friend istream& operator>>(istream&is,student_Node &rhs)//输入数据
	{
		string tmp;
		long num;
		cout<<"请输入姓名：";		//输入名字，到临时变量中，然后通过设置函数设置好成员中的名字，下面的都相同
		is>>tmp;
		rhs.SetName(tmp);
		cout<<"请输入学号：";
		is>>tmp;
		num=atol(tmp.c_str());		//字符型转化为长整形（解决了输入字符时会出现的bug）
		rhs.SetNumber(num);
		cout<<"请输入性别：";
		is>>tmp;
		rhs.SetSex(tmp);
		cout<<"请输入学院：";
		is>>tmp;
		rhs.SetAcademy(tmp);
		cout<<"请输入班级：";
		is>>tmp;
		rhs.SetClass(tmp);
		is>>rhs.Less;				//输入课程
		return is;
	}
	friend ostream& operator<<(ostream&os,student_Node &rhs)//输出数据
	{
		cout<<left<<setw(8)<<rhs.Name<<left<<setw(15)<<rhs.Number<<left<<setw(8)<<rhs.Sex<<left<<setw(10)<<rhs.Academy<<left<<setw(10)<<rhs.Class<<left<<setw(8)<<rhs.Less.ReturnMount_s()<<left<<setw(8)<<rhs.Less.ReturnAverg_s()<<endl;

		cout<<rhs.Less<<endl;		//输出课程
		return os;
	}
	friend class  student_List;		//友元类设置
};
/*******************************************************************************************************************/
class student_List					//学生的信息链表类，主要进行链表的操作，查找输出
{
private:
	student_Node *head;
public:
	student_List()					//学生链表的初始化，建立头结点,并从文件中读取信息建立链表
	{
		head=new student_Node;
		head->next=NULL;
		ifstream ifs(filename);
		if(!ifs)					//如果文件不存在
		{
			Filewrite();			//建立文件
		}
	}
	~student_List()
	{
		student_Node *p=head->next,*q;
		while(p!=NULL)
		{
			q=p;
			p=p->next;
			delete q;
		}
		delete head;
	}
	void Fileread()					//读取文件中的信息建立链表
	{
		ifstream ifs(filename);
		if(!ifs)
		{
			cout<<"文件打开失败"<<endl;
			exit(0);
		}
		student_Node *q=head;
		while(ifs.peek()!=EOF)		//判断是否读取到文件末尾了 采用这个peek函数而不采用ifs.eof()是防止文件为空时还进入循环
		{
			student_Node *p=new student_Node;
			ifs>>p->Name
				>>ws
				>>p->Number
				>>ws
				>>p->Sex
				>>ws
				>>p->Academy
				>>ws
				>>p->Class
				>>ws;
			ifs>>p->Less.n>>ws;
			for(int i=0;i<p->Less.n;i++)
				ifs>>p->Less.lesson[i].L_name>>ws>>p->Less.lesson[i].grade>>ws;
			p->Less.SetA_M();
			q->next=p;
			p->next=NULL;
			q=p;
		}
	}
	void Filewrite()				//通过文件的读写将链表中的内容写到文件中
	{
		student_Node *p=head->next;
		ofstream ofs(filename);
		if(!ofs)
		{
			cout<<"文件打开失败"<<endl;
			exit(0);
		}
		while(p!=NULL)
		{
			ofs<<p->Name<<'\t'
				<<p->Number<<'\t'<<p->Sex<<'\t'<<p->Academy<<'\t'<<p->Class
				<<endl;
			ofs<<p->Less.n;
			for(int i=0;i<p->Less.n;i++)
				ofs<<'\t'<<p->Less.lesson[i].L_name<<'\t'<<p->Less.lesson[i].grade;
			ofs<<endl;
				p=p->next;
		}
		ofs.close();
	}
	/**********************************     操作部分    ***********************************/
	void Putin(int n=1)			//输入,参数的含义是区别与是操作1的输入还好插入函数....默认是输入函数
	{
		char x='0';
		int i=1;				//计数
		int num_0,num_1;//插入位置
		student_Node *q;
		if(n==1)				//这是操作输入数据是需要的，如果插入数据则不需要
		{
			cout<<"是否清空原来数据 0.是的 1.不，添加进去 3.返回 ";
			cin>>x;
			if(x=='1')//添加进去
			{
				Fileread();				//读取文件形成链表
				Putin(0);				//参数的改变,就是插入函数的调用
				return ;
			}
		}
		if(x=='0')
		{
			while(x=='0')
			{
				cout<<i++<<endl;
				student_Node *p=new student_Node;
				cin>>*p;
				if(n!=3)//节点的插法
				{
					p->next=head->next;
					head->next=p;
				}
				else//节点的任意位置插入
				{
					Display(2);
					cout<<"----------------------------------------------------------------"<<endl;
					cout<<"请选择插入位置：";
					cin>>x;
					num_0=x-'0';
					q=head;
					num_1=0;//寻找位置
					while (q->next!=NULL)
					{
						num_1++;
						if(num_0==num_1)
							break;
						q=q->next;
					}
					p->next=q->next;
					q->next=p;
					Display(2);
				}
				cout<<"请问是要继续么？（0.是 1.不是） ";
				cin>>x;
				cout<<"----------------------------------------------------------------"<<endl;
			}
			Filewrite();
		}
	}
	void Display(int x=1)				//显示输出cout<<left<<setw(40)<<"靠左输出"<<left<<setw(35)<<"靠左输出"<<endl;
	{
		if(x==1)
			Fileread();
		cout<<left<<setw(5)<<"·序号·"<<left<<setw(8)<<" ·姓名·"<<left<<setw(15)<<" ·学号·"<<left<<setw(8)<<"·性别·"<<left<<setw(10)<<"·学院·"<<left<<setw(10)<<"·班级·"<<left<<setw(8)<<"|总分|"<<left<<setw(8)<<"|平均分|"<<endl<<endl;
		student_Node *p=head->next;
		int i=1;
		while(p!=NULL)
		{
			cout<<left<<setw(3)<<" "<<left<<setw(3)<<i++<<"    "<<*p;
			p=p->next;
		}
		cin.get();
		cin.get();
	}
	void Insert()				//插入(添加)
	{
		Fileread();				//读取文件形成链表
		Putin(3);				//参数的改变,就是插入函数的调用

	}
	void Delete()				//删除
	{
		Find(2);
		Filewrite();		//写入文件
	}
	void Find(int i=1)			//查找
	{
		Fileread();//读取文件形成链表
		char x='0';
		while(x=='0')
		{
			cout<<"选择查找方式：1.姓名  2.学号  3.学院   0.返回";
			cin>>x;
			switch(x)
			{
			case '1':
				Find_01name(i);
				break;
			case '2':
				Find_02number(i);
				break;
			case '3':
				Find_03academy(i);
				break;
			case '0':return;
			default:
				cout<<"选择错误！！！";
			}
			cout<<"是否继续查找 0.继续 1.不了  ";
			cin>>x;
		}
	}
	void Find_01name(int x=1)	//查找名字 参数 1.查找 2.删除 3.修改
	{
		int i=0;
		string na;
		char x_0='1';
		student_Node *p=head->next,*q=head;
		cout<<"请输入要查找的姓名：";
		cin>>na;
		cout<<left<<setw(5)<<"·序号·"<<left<<setw(8)<<" ·姓名·"<<left<<setw(15)<<" ·学号·"<<left<<setw(8)<<"·性别·"<<left<<setw(10)<<"·学院·"<<left<<setw(10)<<"·班级·"<<left<<setw(8)<<"|总分|"<<left<<setw(8)<<"|平均分|"<<endl<<endl;
		while(p!=NULL)
		{
			if(p->Name==na)
			{
				cout<<left<<setw(3)<<" "<<left<<setw(3)<<i++<<"    "<<*p;
				if(x==2)//删除数据
				{
					cout<<"是否删除这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						q->next=p->next;
						delete p;
						p=q;
					}
				}
				if(x==3)//修改数据
				{
					cout<<"是否修该这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						Modify_small(p);
					}
				}
			}
			
			q=p;
			p=p->next;
		}cout<<"-------------------------------------------------------"<<endl;
		if(i==0)
			cout<<"你能认真点么，没有需要的数据"<<endl;
	}
	void Find_02number(int x=1)//学号查找
	{
		int i=0;
		long num;
		string tmp;
		char x_0='1';
		student_Node *p=head->next,*q=head;
		cout<<"请输入要查找的学号：";
		cin>>tmp;
		num=atol(tmp.c_str());
		cout<<left<<setw(5)<<"·序号·"<<left<<setw(8)<<" ·姓名·"<<left<<setw(15)<<" ·学号·"<<left<<setw(8)<<"·性别·"<<left<<setw(10)<<"·学院·"<<left<<setw(10)<<"·班级·"<<left<<setw(8)<<"|总分|"<<left<<setw(8)<<"|平均分|"<<endl<<endl;
		while(p!=NULL)
		{
			if(p->Number==num)
			{
				cout<<left<<setw(3)<<" "<<left<<setw(3)<<i++<<"    "<<*p;
				if(x==2)//删除数据
				{
					cout<<"是否删除这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						q->next=p->next;
						delete p;
						p=q;
					}
				}
				if(x==3)//修改数据
				{
					cout<<"是否修该这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						Modify_small(p);
					}
				}
			}
			q=p;
			p=p->next;
		}
		cout<<"-------------------------------------------------------"<<endl;
		if(i==0)
			cout<<"你能认真点么，没有需要的数据"<<endl;
	}
	void Find_03academy(int x=1)//学院查找
	{
		int i=0;
		string tmp;
		char x_0='1';
		student_Node *p=head->next,*q=head;
		cout<<"请输入要查找的学院：";
		cin>>tmp;
		cout<<left<<setw(5)<<"·序号·"<<left<<setw(8)<<" ·姓名·"<<left<<setw(15)<<" ·学号·"<<left<<setw(8)<<"·性别·"<<left<<setw(10)<<"·学院·"<<left<<setw(10)<<"·班级·"<<left<<setw(8)<<"|总分|"<<left<<setw(8)<<"|平均分|"<<endl<<endl;
		while(p!=NULL)
		{
			if(p->Academy==tmp)
			{
				cout<<left<<setw(3)<<" "<<left<<setw(3)<<++i<<"    "<<*p;
				if(x==2)
				{
					cout<<"是否删除这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						q->next=p->next;
						delete p;
						p=q;
					}
				}
				if(x==3)//修改数据
				{
					cout<<"是否修该这条数据 0.是 1.否";
					cin>>x_0;
					if(x_0=='0')
					{
						Modify_small(p);
					}
				}
			}
			q=p;
			p=p->next;
		}
		cout<<"-------------------------------------------------------"<<endl;
		if(i==0)
			cout<<"你能认真点么，没有需要的数据"<<endl;
	}
	void Modify()		//修改             界面里加上一些划线增加美观度
	{
		Find(3);
		Filewrite();		//写入文件
	}
	void Modify_small(student_Node *p)
	{
		char a='0';
		string tmp;
		long num;
		while(a=='0')
		{
			cout<<"-------------------------------------------------------"<<endl;
			cout<<"请选择修改选项   1.姓名 2.学号 3.性别 4.学院 5.班级 6.成绩";
            cin>>a;
			cout<<"-------------------------------------------------------"<<endl;
			switch(a)
			{
			case '1':
				cout<<"输入名字：";
				cin>>tmp;
				p->SetName(tmp);
				break;
			case '2':
				cout<<"输入学号：";
				cin>>tmp;
				num=atol(tmp.c_str());
				p->SetNumber(num);
				break;
			case '3':
				cout<<"输入性别：";
				cin>>tmp;
				p->SetSex(tmp);
				break;
			case '4':
				cout<<"输入学院：";
				cin>>tmp;
				p->SetAcademy(tmp);
				break;
			case '5':
				cout<<"请输入班级：";
				cin>>tmp;
				p->SetClass(tmp);
				break;
			case '6':
				p->SetLesson();//修改课程
				break;
			default:
				cout<<"你可爱哦，输错啦，亲~"<<endl;
			}
			cout<<*p;
			cout<<"-------------------------------------------------------"<<endl;//美化视觉效果
			cout<<"是否继续修改其他选项 0.继续 1.不了";
			cin>>a;
		}
	}
	void Sort()			//排序
	{
		Fileread();	
		char t='0';
		while (t=='0')
		{
			cout<<"------------------------------------------------------------"<<endl;
			cout<<"1.学号 2.单科成绩 3.总成绩 0.返回";
			cin>>t;
			switch (t)
			{
			case '1':
				Sort_small(1);break;//学号排序
			case '2':
				Sort_small(2);break;//单科成绩排序
			case '3':
				Sort_small(3);break;//总成绩排序
			default:
				Filewrite();//
				return ;
			}
			Display(0);
			cout<<"是否继续排序 0.继续 1.不了,并保存结果";
			cin>>t;
		}
		Filewrite();		//写入文件
	}
	void Sort_small(int i=1)//排序函数分支，根据参数的不同进行不同选项的排序1.默认学号 2.单科成绩 3.总成绩  
	{
		char t;int x;
		if(i==2)//单科成绩排序时输出
		{
			L.Dislesson();//显示公共课程
			cin>>t;
			x=t-'0';//课程的下标
			if(x>L.n||x<0)
			{
				cout<<"选择错误"<<endl;
				return ;
			}
		}
		cout<<"1.递增 2.递减";
		student_Node *p1,*p2;
		student_Node tmp;
		int flag=1;
		cin>>t;
			while(1)//冒泡排序
			{
				flag=1;
				p1=head->next;p2=p1->next;
				while(p2!=NULL)
				{
					switch (t)
					{
					case '1':
						if(i==3)//总分排序
							if(p1->Less.Mount_s>p2->Less.Mount_s)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							}
						if(i==2)//成绩排序
							if(p1->Less.lesson[x-1].grade>p2->Less.lesson[x-1].grade)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							}
						if(i==1)//学号排序
							if(p1->Number>p2->Number)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							};break;
					case '2':
						if(i==3)//总分排序
							if(p1->Less.Mount_s<p2->Less.Mount_s)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							}
						if(i==2)//成绩排序
							if(p1->Less.lesson[x-1].grade<p2->Less.lesson[x-1].grade)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							}
						if(i==1)//学号排序
							if(p1->Number<p2->Number)
							{
								tmp=*p1;
								*p1=*p2;
								*p2=tmp;
								flag=0;
							};break;
					default:
						break;
					}
					
					p1=p1->next;
					p2=p2->next;
				}
				if(flag==1)
					break;
			}
	}
	void Cal()			//计算各个学科总成绩和平均成绩   需要强大其功能
	{
		Fileread();	
		cout<<"-------------------------------------------------------------------"<<endl;
		cout<<left<<setw(10)<<"科目"<<left<<setw(10)<<"总分"<<left<<setw(10)<<"平均分"<<left<<setw(10)<<"通过率"<<endl;//三个空格
		for(int i=0;i<L.n;i++)
			Cal_small(L.lesson[i].L_name);
		cin.get();cin.get();
	}
	void Cal_small(string na)
	{
		student_Node *p=head->next;
		int num_1=0,num_n=0,num_p=0;
		while(p!=NULL)
		{
			for(int i=0;i<p->Less.n;++i)
			{
				if(p->Less.lesson[i].L_name==na)
				{
					num_n++;//总人数
					num_1=num_1+p->Less.lesson[i].grade;
					if(p->Less.lesson[i].grade>=60)
						num_p++;//通过人数
					break;
				}
			}
			p=p->next;
		}
		cout<<left<<setw(10)<<na<<left<<setw(10)<<num_1<<left<<setw(10)<<num_1/num_n<<left<<setw(3)<<(float)num_p/(float)num_n*100<<"%"<<endl;
	}
	float Grade_fail(string name ,int grade)		//计算成绩打败全校的百分比
	{
		int num_1=0,num_2=0;
		float num;
		student_Node *p=head->next;
		while (p!=NULL)
		{
			for(int i=0;i<p->Less.n;i++)
			{
				if(name==p->Less.lesson[i].L_name)//计算选这门课的人
				{
					num_1++;
					if(p->Less.lesson[i].grade<=grade)
						num_2++;//比你分数差的人人数
					break;
				}
			}
			p=p->next;
		}
		num=(float)num_2/(float)num_1*100;//类型转换
		return num;
	}
	void student_pass(string na ,string num )		//学生登陆情况
	{
		Fileread();
		student_Node *p=head->next;
		while(p!=NULL)
		{
			if(p->Name==na&&p->Number==atol(num.c_str()))
				break;
			p=p->next;
		}
		if(p==NULL)
		{
			cout<<"没有这个学生啊！！！！";
			cin.get();cin.get();
			return ;
		}
		int i=0;
		system("cls");
		cout<<endl<<endl<<"我是"<<p->Name<<",我的编号是"<<p->Number<<",性别"<<p->Sex<<",现在在"<<p->Academy<<p->Class<<"班,深藏功与名~"<<endl<<endl;
		cout<<left<<setw(15)<<"学科"<<" 成绩"<<endl;
		cout<<"-------------------------------------------------------------------------"<<endl;
		for(i=0;i<p->Less.n;i++)
		{
			cout<<left<<setw(15)<<p->Less.lesson[i].L_name<<" "<<p->Less.lesson[i].grade<<"          我打败了全校"<<Grade_fail(p->Less.lesson[i].L_name,p->Less.lesson[i].grade)<<"%的人"<<endl;
			cout<<"-------------------------------------------------------------------------"<<endl;
		}
		cin.get();cin.get();
	}
};
const string Pass_word="123456";//教师登陆密码
void Meue_studentpass()//学生登陆
{
	student_List B;
	string na ,num;
	cout<<"\n\n                           请输入学号：";
	cin>>num;
	cout<<"                           请输入名字：";
	cin>>na;
	B.student_pass(na,num);
}
void Meue()				//主菜单界面
{
	while(1)
	{
	system("cls");
	cout<<"\n\n\n\n\n";
	cout<<"             ★★★当我和世界不一样 那就让我不一样吧★★★ "<<endl;
	cout<<"                    ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁"<<endl;
	cout<<"                    ▏                            ▕"<<endl;
	cout<<"                    ▏        1.输入              ▕"<<endl;
	cout<<"                    ▏        2.输出              ▕"<<endl;
	cout<<"                    ▏        3.公共课程设置      ▕"<<endl;
	cout<<"                    ▏        4.添加              ▕"<<endl;//变成课任意插入位置的
	cout<<"                    ▏        5.查找              ▕"<<endl;
	cout<<"                    ▏        6.删除              ▕"<<endl;
	cout<<"                    ▏        7.修改              ▕"<<endl;
	cout<<"                    ▏        8.排序              ▕"<<endl;
	cout<<"                    ▏        9.计算              ▕"<<endl;
	cout<<"                    ▏        0.退出登陆          ▕"<<endl;
	cout<<"                    ▏                            ▕"<<endl;
	cout<<"                    ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔"<<endl;
	student_List A;//建立链表,初始化
	char t;
	cin>>t;
	system("cls");
	switch (t)
	{
	case '1':
		A.Putin();
		break;
	case '2':
		A.Display();
		break;
	case '3':
		L.SetPbulicLesson();
		break;
	case '4':
		A.Insert();
		break;
	case '5':
		A.Find();
		break;
	case '6':
		A.Delete();
		break;
	case '7':
		A.Modify();
		break;
	case '8':
		A.Sort();
		break;
	case '9':
		A.Cal();break;
	case '0':return ;
	default:
		break;
	}
	}
}
void Welcome()			//开始的登陆界面
{
	system("cls");

	cout<<"\n\n\n\n\n\n";
	cout<<"                            ☆学生成绩管理系统☆"<<endl;
	cout<<"                                              @by__大伟"<<endl;
	cout<<"                           ▇▇▇▇▇▇▇▇▇▇▇"<<endl;
	cout<<"                           ★                  ★"<<endl;
	cout<<"                           ★    1.教师登录    ★"<<endl;
	cout<<"                           ★    2.学生登录    ★"<<endl;
	cout<<"                           ★    0.退   出     ★"<<endl;
	cout<<"                           ★                  ★"<<endl;
	cout<<"                           ▇▇▇▇▇▇▇▇▇▇▇";
	char x;
	string tmp;
	cin>>x;
	switch(x)
	{
	case '0':exit(0);
	case '1':
		cout<<"\n\n                           请输入密码：";
		cin>>tmp;
		if(tmp==Pass_word)					//判断是否通过密码
			Meue();
		break;
	case '2':
		Meue_studentpass();
		break;
	}
	//_sleep(2*1000);//延时命令
}
int main()
{
	while(1)
	Welcome();
	return 0;
}