/*
 * roboshell.c
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#include "roboshell.h"


/** GLOBAL **/

CLI_Command_Callback_List_Item_t *command_list_head=NULL;
char *margvp[16];
char margvbuf[64];


/** FUNCTION **/

int CLI_register(CLI_Command_Callback_Definition_t *cd)
{
	CLI_Command_Callback_List_Item_t *n, *p=command_list_head;

	// Insert new element
	n=(CLI_Command_Callback_List_Item_t *)pvPortMalloc(sizeof(CLI_Command_Callback_List_Item_t *));
	n->command=cd;
	n->next=NULL;

	// Find last element in list and insert n
	if(command_list_head!=NULL)
	{
		for(p=command_list_head; p->next!=NULL; p=p->next);
		p->next=n;
	}
	else  // List is empty
	{
		command_list_head=n;
	}

	return 0;
}

int CLI_parse(char *s, char **argv, int *argc, char *buffer)
{
	char *b, *e;
	int c=0, n=0, a=0;

	b=e=s;
	memset(argv, 0, sizeof(argv));
	memset(buffer, 0, sizeof(buffer));

	while((*e!='\0') && (*e!='\r') && (*e!='\n'))
	{
		e++;  n++;
		if(((*e==' ') || (*e=='\0') || (*e=='\r') || (*e=='\n')) && (b!=e))
		{
			strncpy(buffer+a, b, n);
			buffer[a+n]='\0';
			argv[c]=buffer+a;
			//printf("%s#\n", argv[c]);
			c++;  a+=n+1;
			while(*e==' ')
				e++;
			b=e;  e--;  n=-1;
		}
	}

	*argc=c;
	return c;
}


int CLI_process(int argc, char **argv)
{
	CLI_Command_Callback_List_Item_t *p;
	int r;
	char out[16]="";

	if(command_list_head!=NULL)
	{
		for(p=command_list_head; p!=NULL; p=p->next)
		{
			if(strcmp(p->command->command_str, argv[0])==0)
			{
				//printf("Found: %s\r\n", p->command->command_str);
				while((r=p->command->command_func(argc, argv, out, sizeof(out) ))>0)
				{
					printf("%s", out);
				}
				printf("%s", out);
				break ;
			}
		}
	}

	return 0;
}

int CMD_read_adc(int argc, char *argv[], char outs[], size_t outl)
{
	unsigned int v=0, c=0;

	if(argc!=2)
		return -1;

	c = atoi(argv[1]);
	ADCValue[c] = ADCRead( c );
	while ( !ADCIntDone );
	ADCIntDone = 0;
	v = ADCValue[c];

	//TODO outs bounds check using outl
	sprintf(outs, "adc=%d\r\n\0", v);
	outl=outl;

	//one line to print
	return 0;
}


int CMD_set_gpio(int argc, char *argv[], char outs[], size_t outl)
{
	unsigned int c=0, p=-1;

	outl=outl;
	outs=outs;

	if(argc!=3)
		return -1;

	p = atoi(argv[1]);
	c = atoi(argv[2]);

	if(c==1)
		gpio_set(p);
	else
		gpio_clear(p);

	return 0;
}


int CMD_select_gpio(int argc, char *argv[], char outs[], size_t outl)
{
	unsigned int v=0;

	outl=outl;
	outs=outs;

	if(argc!=2)
		return -1;

	v = atoi(argv[1]);

	gpio_select(v);

	return 0;
}
