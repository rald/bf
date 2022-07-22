#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int TAB_SIZE;

int	MEM_MAX;

int *m;
char *c;

int mp,cp,sp;

int f,n,i,d,ch;
char b;

int ln,cl;
int scp;



void error(int code,int scp,char *msg) {

	ln=0; cl=0; i=0;

	while(i<scp) {

		if(c[i]=='\t') cl=cl+TAB_SIZE;
		else if(c[i]=='\n') { ln++; cl=0; } 
		else cl++;

		i++;				
	}

	ln++; cl++;

	printf("ERR: LN %d CL %d: %s\n",ln,cl,msg);

	exit(code);
	
}



void check() {

	int *s;

	s=malloc(sizeof(int)*n);
	sp=n;

	i=0;

	while(i<n) {

		if(c[i]=='[') {
			if(sp==0) error(12,i,"ERR: STACK OVERFLOW"); 
			s[--sp]=i;
		} else if(c[i]==']') { 
			if(sp==n) error(10,i,"ERR: UNBALANCED ]"); 
			sp++;
		}

		i++;
	}	
	if(sp<n) error(11,s[sp],"ERR: UNBALANCED ["); 

	free(s);
	s=0;

}



int main(int argc,char **argv) {

	TAB_SIZE = 2;

	MEM_MAX = 65536;



	if(argc<2) {
		printf("usage: %s filename\n",argv[0]);
		return 1;
	}
        
	if((f=open(argv[1],0))==-1) {
		printf("error opening file %s\n",argv[1]);
		return 2;
	} 

	n=0;
  while(read(f,&b,1)>0) {
  	n++;
  }

	close(f);

	if((c=malloc(sizeof(char)*n))==0) {
		printf("error allocating memory\n");
		return 3;
	}

	if((f=open(argv[1],0))==-1) {
		printf("error opening file %s\n",argv[1]);
		return 4;
	} 	

	if(read(f,c,n)!=n) {
		printf("error reading file\n");
		return 5;
	}
	c[n]=0;	

	close(f);



	check();



	m=malloc(sizeof(int)*MEM_MAX);

	i=0;
	while(i<MEM_MAX) {
		m[i]=0;
		i++;
	}
        
	mp=0;
	cp=0;
	
	while(cp<n) {

		if(c[cp]=='+') { 
			m[mp]++; 
			if(m[mp]>255) m[mp]=0; 
		} else if(c[cp]=='-') { 
			m[mp]--; 
			if(m[mp]<0) m[mp]=255; 
		} else if(c[cp]=='<') { 
			mp--; 
			if(mp<0) error(6,cp,"ERR: <");
		} else if(c[cp]=='>') { 
			mp++; 
			if(mp>=MEM_MAX) error(7,cp,"ERR: >");
		} else if(c[cp]=='.') {
			putchar(m[mp]);
		} else if(c[cp]==',') { 
			ch=getchar(); 
			if(ch==-1) m[mp]=0; else m[mp]=ch & 0xFF; 
		} else if(c[cp]=='[') {
			scp=cp;
			if(m[mp]==0) {
				d=1;
				while(d) {
					cp++;
					if(cp>=n) error(8,scp,"ERR: [");
					if(c[cp]=='[') d++; else if(c[cp]==']') d--; 
				}
			}
		} else if(c[cp]==']') {
			scp=cp;
			if(m[mp]) {
				d=1;
				while(d) {
					cp--;
					if(cp<0) error(9,scp,"ERR: ]");
					if(c[cp]=='[') d--; else if(c[cp]==']') d++; 
				}
			}
		}		

		cp++;		

	}

	free(m);
	m=0;

	free(c);
	c=0;
		
	return 0;
}
