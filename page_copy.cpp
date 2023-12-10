#include <iostream>
#include <string>
#include <fstream>
using namespace std;
const string keyword_tag_start = "<Tag TargetID=\"@OpenLink\">";
const string keyword_tag_start_2 = "<Name>";
const string keyword_text_start = "<Text><body><p>";
const string keyword_page_start_1 = "</Height>";
const string keyword_page_start_2 = keyword_tag_start_2;
const string keyword_page_number = "<Number>";
const string ID_start = " ID=\"";

const string keyword_tag_end = "</Name>";
const string keyword_text_end = "</p></body></Text>";
const string ID_end = "\" Compo";
const string keyword_page_end = keyword_tag_end;
const string keyword_page_number_end = "</Number>";


const int offset_tag_start = keyword_tag_start_2.length();
const int offset_text_start = keyword_text_start.length();
const int offset_tag_end = keyword_tag_end.length();
const int offset_text_end  = keyword_text_end.length();
const int offset_ID_start = ID_start.length();

int cont = 0;
int cont_id = 0;
int ID = 200;
const int n = 100;
int n_elementi;
string vettore_iniziale[n];
string vettore_sostituzione[n][n];
int n_title = 1;
int n_page = 10;
int n_sostituzioni = 0;

void setup()
{

	fstream fin;
	fin.open("setup.txt",ios::in);

	for(int i = 0;i<n;i++)
	{
		string temp;
		getline(fin,temp,'\n');
		if(temp=="SOSTITUTIVO")
			n_sostituzioni++;	
	}
	
	fin.close();
	fin.open("setup.txt",ios::in);
	
	for(int i = 0;i<n;i++)
	{
		string temp;
		getline(fin,temp,'\n');
		//cout<<"\nRiapro : "<<temp;
		if(temp!="SOSTITUTIVO" )
		{
			if(temp!="ORIGINALE" and !temp.empty())
			vettore_iniziale[n_elementi++] = temp;
		}
		else
		i = n+1;
	}
	
	for(int i = 0;i<n_sostituzioni;i++)
	{
		for(int j = 0;j<n_elementi;j++)
		{
			string temp;
			getline(fin,temp,'\n');
			if(temp == "FINE")
				break;
			if(temp!="SOSTITUTIVO" and !temp.empty())
			vettore_sostituzione[i][j] = temp;
			if(temp=="SOSTITUTIVO")
				j = -1;
			//cout<<"\nTemp : "<<temp<<" j = "<<j<<" / "<<n_elementi;
		}
	}
cout<<"\n";
}


int main(){
	
	
	setup();
	for(int i = 0;i<n_sostituzioni;i++)	
	{
		for(int j = 0;j<n_elementi;j++)	
		cout<<"\nOriginale : "<<vettore_iniziale[j]<< " da sostituire con : "<<vettore_sostituzione[i][j];
		//cout<<"\n"<<vettore_sostituzione[i][j];
	}
		
	cout<<"\nNumero sostituzioni : "<<n_sostituzioni<<endl;
	
	for(int k = 0;k<n_sostituzioni;k++)
	{
		
		fstream fin;
		fin.open("file_input.xml");
	
		fstream fout;
		string nome_file_out ="file_output";
		nome_file_out+= "_" + to_string(k+1);
		fout.open(nome_file_out +".xml",ios::out);
		string input;
		while(!fin.eof())
		{
			getline(fin,input,'\n');
			//PAGE TITLE
			int start_subst = input.find(keyword_page_start_1);
			int end_subst = 0;
			if(start_subst!=-1)
			{
				fout<<input<<endl;
				getline(fin,input,'\n');
				start_subst = input.find(keyword_page_start_2);
				end_subst = input.find(keyword_tag_end);
				
				start_subst+=offset_tag_start;
				string titolo_old = input.substr(start_subst,end_subst-start_subst);
				string titolo_new = titolo_old + "_"+ to_string(n_title++); 
				cout<<"\nNuovo titolo : "<<titolo_new<<endl;
				cout<<"\nBefore : "<<input;
					input = input.replace(start_subst,end_subst-start_subst,titolo_new); 
					cout<<"\nAfter : "<<input;cout<<endl;
					cont++;
	
			}
			//PAGE NUMBER
			start_subst = input.find(keyword_page_number);
			end_subst = input.find(keyword_page_number_end);
			if(start_subst!=-1)
			{
				start_subst+=offset_tag_start+2; 
				cout<<"\nBefore : "<<input;
					input = input.replace(start_subst,end_subst-start_subst,to_string(n_page++)); 
					cout<<"\nAfter : "<<input;cout<<endl;
					cont++;
			}
			//TEXT
			start_subst = input.find(keyword_text_start);
			end_subst = input.find(keyword_text_end);
			if(start_subst!=-1)
			{
				start_subst+=offset_text_start;
				for(int i= 0;i<n_elementi;i++)
				{
					if(input.substr(start_subst,vettore_iniziale[i].size()) == vettore_iniziale[i])
						{
							cout<<"\nBefore : "<<input;
							input = input.replace(start_subst,vettore_iniziale[i].size(),vettore_sostituzione[k][i]);
							cont++;
							i = 200;
							cout<<"\nAfter : "<<input;cout<<endl;
						}
					
				}
			}
			//TAG
			start_subst = input.find(keyword_tag_start);
			if(start_subst!=-1)
			{
				fout<<input<<endl;
				getline(fin,input,'\n');
				start_subst = input.find(keyword_tag_start_2);
				end_subst = input.find(keyword_tag_end);
				
				start_subst+=offset_tag_start;
				for(int i= 0;i<n_elementi;i++)
				{
					if(input.substr(start_subst,vettore_iniziale[i].size()) == vettore_iniziale[i])
						{
						cout<<"\nBefore : "<<input;
						input = input.replace(start_subst,vettore_iniziale[i].size(),vettore_sostituzione[k][i]); 
						cout<<"\nAfter : "<<input;cout<<endl;
						cont++;
						i = 200;
						}
				}
			}
			
			//ID
			start_subst = input.find(ID_start);
			end_subst = input.find(ID_end);
			if(start_subst!=-1)
			{
				start_subst+=offset_ID_start;
				cout<<"\nBefore : "<<input;
				input = input.replace(start_subst,to_string(ID++).length(),to_string(ID++)+"\" ");
				cout<<"\nAfter : "<<input;cout<<endl;
				cont_id++;
			}
					
			fout<<input<<endl;
		}
		
		fin.close();
		fout.close();
	}
	cout<<"\nFinale : "<<cont<< " ID : "<<cont_id;

}
