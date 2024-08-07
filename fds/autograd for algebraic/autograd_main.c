#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

typedef struct tree* Tree;
struct tree{         //define struct tree
    char c[11];         //store operand
    Tree left;          //point to left tree
    Tree right;         //point to right tree
};

Tree backT = NULL;    //the curent root of tree

void preprocess(char *in, char* out);     //process the input for easier changing
void postorder(char* in, char* po, char (*operand)[11]);  //get postorder of input
Tree CreateTree(char* po, char (*operand)[11]);   //create tree based on the postorder of input
void sort_operand(char (*operand)[11], char (*variable)[11]);    //sort the variables in lexicographical order
char* autograd(Tree t, char *variable, char* out);   //get the autograd expression stored in out
char *inor(Tree t, char* out);   //store the result of inorder traversal of tree in out
void simplify(char* out, char *out1);    //simplify the result of autograd
int check0(Tree t, Tree parent, int lor);    //remove the part including 0 in tree
int check1(Tree t, Tree parent, int lor);    //remove the part including *1 in tree
char *inor_sim(Tree t, char* out, Tree parent);    //get a shorter inorder traversal result

int main(){

    char *input = NULL, *post = NULL, *inorder, *out = NULL, *out1 = NULL;
    char *no;     //a not significant point
    int i = 0;    //number of variable
    static char operand[100][11];   //store the operands
    input = (char*)malloc(sizeof(char)*101);    //the string inputted
    inorder = (char*)malloc(sizeof(char)*200);    //the string after preprocess
    post = (char*)malloc(sizeof(char)*300);    //the postorder of input 
    out = (char*)malloc(sizeof(char)*500);     //the result of autograd
    out1 = (char*)malloc(sizeof(char)*300);    //the result after simplify
   
    printf("please enter an expression no longer than 100:");
    gets(input);  //require the input

    preprocess(input, inorder);   //preprocess the input

    postorder(inorder, post, operand);  //get postorder of input
    
    Tree t = NULL;
    t = CreateTree(post, operand);   //create tree based on postorder

    static char variable[100][11];
    sort_operand(operand, variable);  //sort the variable
    printf("after simplify\n");
    //printf("before simplify\n");
    while(variable[i][0]){  //loop to handle all the variable in order
        no = autograd(t, variable[i], out);   //get the autograd
        *no = '\0';  //end the result
        //printf("%s:%s\n", variable[i], out);   //before simplify
        simplify(out, out1);  //simplify the result
        printf("%s:%s\n", variable[i], out1);  //output the result after simplify
        i++;
    }

    free(t);   //free the space
    free(input);
    free(inorder);
    free(post);
    free(out);
    free(out1);
    
    system("pause");

    return;
}

void simplify(char* out, char *out1){       //simplify the result

    char *po, *afterpre;          //initialization
    static char rand[100][11];
    po = (char*)malloc(sizeof(char)*200);
    afterpre = (char*)malloc(sizeof(char)*200);

    preprocess(out, afterpre);   //preprocessor
    postorder(afterpre, po, rand);   //get postorder

    Tree t = NULL;
    t = CreateTree(po, rand);   //create tree of po(result)
    backT = t;

    if(check0(t, NULL, 0)){   //if the result != 0

        if(check1(backT, NULL, 0)){     //if the result != 1

            out1 = inor_sim(backT, out1, NULL);  //get simple inorder
            *(out1) = '\0';

        }else strcpy(out1, "1");   //if the result == 1
        
    }else{  //if the result == 0
        strcpy(out1, "0");
    }
    return;
}

void preprocess(char* in, char *out){     //preprocessor
    char *st = in;
    while(*in){
        if(*in=='-'&&(in==st||*(in-1)<'0'&&*(in-1)!=')')){    //if a single '-'
            *out = 'M';    //change it to minus function to differ from subtraction
            out++;      //move the pointer to next char
            in++;
        }else if(*in=='('&&in!=st){    //if find function
            if(*(in-1)=='p'){      //if find exp()
                strcpy(out, "(E,");     //change it to binocular ooperation
                out = out+3;
                in++;
                continue;
            }else if(*(in-1)=='n'&&*(in-2)=='l'){   //if find ln()
                strcpy(out, "(E,");     //change it to binocular operation
                out = out+3;
                in++;
                continue;
            }
        }
        *out = *in;    //not need pretreatement
        out++;
        in++;
    }
    *out = '\0';  //end the treatement
    return;
}

void postorder(char* in, char* po, char (*operand)[11]){    //get postorder
    char *operator = NULL;   
    static char tmp[11];
    int p_rand = 0, p_rator = 0, p_tmp = 0;     //set pointers
    int i, flag=0;    //flag point to if minus
    operator = (char*)malloc(sizeof(char)*100);   //set the stack of operator

    while(*in){     //if the input not end
        switch (*in) {    //check the current char

        case ',':
            while(operator[--p_rator]!='('){    //pop until reach '('
                *po = operator[p_rator];    //pop the operator
                po++;
            }
            p_rator++;
            break;

        case '+':
        case '-':
            if(p_rator==0){    //if the stack empty
                operator[p_rator] = *in;   //push the operator
                p_rator++;

            }else if(operator[p_rator-1]=='('){   //if the stack top is '('
                operator[p_rator] = *in;   //push the operator
                p_rator++;

            }else{    
                while(p_rator&&operator[p_rator-1]!='('){  //pop until reach '('
                    *po = operator[p_rator-1];    //pop the operator
                    po++;
                    p_rator--;
                }
                operator[p_rator] = *in;   //push the current operator
                p_rator++;
            } 
            break;

        case '*':
        case '/':
            if(p_rator==0){       //if the stack is empty
                operator[p_rator] = *in;   //push
                p_rator++;  

            }else if(operator[p_rator-1]=='^'||operator[p_rator-1]=='*'||operator[p_rator-1]=='/'){
                while(p_rator&&operator[p_rator-1]!='('&&operator[p_rator-1]!='+'&&operator[p_rator-1]!='-'){   //pop until reach lower priority
                    *po = operator[p_rator-1];   //pop
                    po++;
                    p_rator--;
                }
                operator[p_rator] = *in;   //push the current
                p_rator++;  

            }else{
                operator[p_rator] = *in;    //push
                p_rator++; 
            }
            break;

        case '^':
            if(p_rator==0){      //if stack empty
                operator[p_rator] = *in;   //push
                p_rator++; 

            }else if(operator[p_rator-1]=='^'){   
                while(p_rator&&operator[p_rator-1]=='^'){    //pop until reach '^'
                    *po = operator[p_rator-1];   //pop
                    po++;
                    p_rator--;
                }
                operator[p_rator] = *in;  //push the current
                p_rator++; 

            }else{
                operator[p_rator] = *in;   //push
                p_rator++; 
            }
            break;

        case '(':
            if(*(in-1)<='z'&&*(in-1)>='a'){   //if function
                switch(*(in-1)){    //check the last char

                    case 'n':    //if sin, ln or tan
                        if(*(in-2)=='l'){  //if ln
                            operator[p_rator]='L';   //using L to mark ln
                            p_rator++;
                        }else if(*(in-2)=='i'){   //if sin
                            operator[p_rator] = 'S';  //using S to mark sin
                            p_rator++;
                        }else{   //if tan
                            operator[p_rator] = 'T';  //using T to mark tan
                            p_rator++;
                        }
                        break;

                    case 's':  //if cos
                        operator[p_rator] = 'C';  //using C to mark cos
                        p_rator++;
                        break;

                    case 'g':  //if log
                        operator[p_rator] = 'L';  //using L to mark log
                        p_rator++;
                        break;

                    case 'w':    //if pow
                    case 'p':    //if exp
                        operator[p_rator] = 'P';   //using P to mark pow and exp
                        p_rator++;
                        break;
                }
                operator[p_rator] = *in;  //push '('
                p_rator++;

            }else{   //if not function
                operator[p_rator] = *in;   //push '('
                p_rator++;
            }
            break;

        case ')':
            while(operator[--p_rator]!='('){   //pop until '('
                *po = operator[p_rator];  //pop
                po++;
            }
            if(operator[p_rator-1]<='Z'&&operator[p_rator-1]>='A'){  //if function
                p_rator--;  //pop function
                *po = operator[p_rator];
                po++;
            }
            break;

        default:  //if operand or minus

            if(*in=='M'){  //if minus
                flag=1;  //set flag
                in++;
            }
            tmp[p_tmp]=*in;   //push into tmp
            p_tmp++;

            if(*(in+1)=='('){   //if function
                p_tmp=0;   //wipe off

            }else if(*(in+1)<'0'||*(in+1)=='^'){   //if operand end
                tmp[p_tmp]='\0';  //end the string
                strcpy(operand[p_rand], tmp);  //put operand into stack
                for(i=0; i<p_tmp; i++){  //output
                    *po = tmp[i];
                    po++;
                }

                if(flag==1){  //if minus
                    *po = 'M';  //output it after string
                    po++;
                    flag = 0;  //reset flag
                }
                p_tmp = 0;  //reset tmp
                p_rand++;   //move top
                
            }
            break;
        }
        in++;

    }

    while(p_rator){   //after reading, pop all the remaining operator
        p_rator--;
        *po = operator[p_rator];
        po++;       
    }
    *po = '\0';  //end the output
} 

Tree CreateTree(char* po, char (*operand)[11]){   //create tree with postorder
    
    Tree tempt;
    static Tree stack[100];  //set stack of nodes
    int top = 0;

    int p_rand = 0;
    while(*po){

        tempt = (Tree)malloc(sizeof(struct tree));   //malloc the space for new node

        if(*po<='z'&&*po>='a'||*po<='9'&&*po>='0'||*po=='E'){   //if operand
            
            strcpy(tempt->c, operand[p_rand]);   //set node
            
            tempt->left = NULL;   //operand is leaf
            tempt->right = NULL;
            stack[top] = tempt;  //push the node
            top++;
            po = po+strlen(operand[p_rand]);  //move to next element
            p_rand++;  

        }else if(*po=='M'){   //change minus to (-1)*

            tempt->left = NULL;   //create node of -1
            tempt->right = NULL;
            strcpy(tempt->c, "-1");   
            stack[top] = tempt;   //push

            tempt = (Tree)malloc(sizeof(struct tree));   //create node of *
            *(tempt->c) = '*';
            *(tempt->c+1) = '\0';
            tempt->left = stack[top];   //connect two leaves
            tempt->right = stack[--top];
            stack[top] = tempt;
            top++;
            po++;

        }else if(*po=='C'||*po=='S'||*po=='T'){   //if sin, cos or tan
            *(tempt->c) = *po;
            *(tempt->c+1) = '\0';
            tempt->right = stack[--top];  //only right leaf
            tempt->left = NULL;
            stack[top] = tempt;
            top++;
            po++;

        }else{    //other operator
            if(*po=='P')*po='^';   //change p to ^
            *(tempt->c) = *po;
            *(tempt->c+1) = '\0';   //end the string
            tempt->right = stack[--top];
            if(top>0)tempt->left = stack[--top];  
            else tempt->left = NULL;
            stack[top] = tempt;
            top++;
            po++;
        }
    }
    if(top<=0)return NULL;   //empty tree
    else return stack[--top];   //return root

}

void sort_operand(char (*operand)[11], char (*variable)[11]){    //sort variable
    int n = 0;
    static char (*tmp)[11], min[11], premin[11];
    char (*tmpv)[11];
    tmpv = variable;
    tmp = operand;

    while(*tmp[0]){   //get the number of operand
        n++;
        tmp++;
    }

    tmp = operand;   //reset pointer
    while(*tmp[0]){
        if(*tmp[0]>='a'){    //if variable
            if(min[0]=='\0'||strcmp(tmp, min)<0){   //if no min or current < min
                strcpy(min, tmp);
            }

        }else n--;   //if integer
        tmp++;
    }

    tmp = operand;
    while(*tmp[0]){     //if not empty
        if(strcmp(tmp, min)==0){   //if same with min
            n--;   //wipe the relativen number off
        }
        tmp++;
    }
    strcpy(premin, min);   //set premin
    
    strcpy(tmpv, min);    //store sorted variable
    tmpv++;
    while(n){    //there remains operand
        tmp = operand;
        strcpy(min,"");    //reset min
        while(*tmp[0]){    
            if(*tmp[0]>='a'&&n&&strcmp(tmp, premin)>0){   //bigger than premin
                if(min[0]=='\0'||strcmp(min, tmp)>0){   //min in remained
                    strcpy(min, tmp);
                }
            }
            tmp++;
        }
        tmp = operand;
        while(*tmp[0]&&n){
            if(strcmp(tmp, min)==0)n--;  //if same with min
            tmp++;
        }
        strcpy(premin, min);    //set premin
        strcpy(tmpv, min);    //store sorted variable
        tmpv++;
    }

    return;
}

char* autograd(Tree t, char *variable, char* out){    //autograd the tree
    if(t==NULL)return out;  //protect the program
    switch(*(t->c)){
        case '+':
        case '-':     //(left' +- right')
            if(*(t->c+1)!='\0'){
                *(out++)='0';
                break;
            }
            *(out++) = '(';
            out = autograd(t->left, variable, out);
            *(out++) = *(t->c);
            out = autograd(t->right, variable, out);
            *(out++) = ')';
            break;
        case '*':    //(left'*right+right'*left)
            *(out++) = '(';
            out = autograd(t->left, variable, out);
            *(out++) = '*';
            out = inor(t->right, out);
            *(out++) = '+';
            out = autograd(t->right, variable, out);
            *(out++) = '*';
            out = inor(t->left, out);
            *(out++) = ')';
            break;
        case '/':    //((left'*right-right'*left)/right^2)
            *(out++) = '(';
            *(out++) = '(';
            out = autograd(t->left, variable, out);
            *(out++) = '*';
            out = inor(t->right, out);
            *(out++) = '-';
            out = autograd(t->right, variable, out);
            *(out++) = '*';
            out = inor(t->left, out);
            *(out++) = ')';
            *(out++) = '/';
            out = inor(t->right, out);
            strcpy(out, "^2)");
            out = out+3;
            break;
        case '^':
        case 'P':   //(right*left'*left^(right-1)+right'*ln(left)*left^right)
            *(out++) = '(';
            out = inor(t->right, out);
            *(out++) = '*';
            out = autograd(t->left, variable, out);
            *(out++) = '*';
            out = inor(t->left, out);
            *(out++) = '^';
            *(out++) = '(';
            out = inor(t->right, out);
            strcpy(out, "-1)+");
            out = out+4;
            out = autograd(t->right, variable, out);
            strcpy(out, "*ln(");
            out = out+4;
            out = inor(t->left, out);
            *(out++) = ')';
            *(out++) = '*';
            out = inor(t, out);
            *(out++) = ')';
            break;           
        case 'L':    //(((-1)*left')/(left*ln(right)*log(left,right)^2)+(right'/(right*ln(left))))
            strcpy(out, "(((-1)*");
            out = out+7;
            out = autograd(t->left, variable, out);
            strcpy(out, ")/(");
            out = out+3;
            out = inor(t->left, out);
            strcpy(out, "*ln(");
            out = out+4;
            out = inor(t->right, out);
            strcpy(out, ")*log(");
            out = out+6;
            out = inor(t->left, out);
            *(out++) = ',';
            out = inor(t->right, out);
            strcpy(out, ")^2)+(");
            out = out+6;
            out = autograd(t->right, variable, out);
            *(out++) = '/';
            *(out++) = '(';
            out = inor(t->right, out);
            strcpy(out, "*ln(");
            out = out+4;            
            out = inor(t->left, out);
            strcpy(out, "))))");
            out = out+4;
            break;
        case 'S':   //(right'*cos(right))
            *(out++) = '(';
            out = autograd(t->right, variable, out);
            strcpy(out, "*cos(");
            out = out+5;
            out = inor(t->right, out);
            *(out++) = ')';
            *(out++) = ')';
            break;
        case 'C':  //((-1)*right'*sin(right))
            strcpy(out, "((-1)*");
            out = out+6;
            out = autograd(t->right, variable, out);
            strcpy(out, "*sin(");
            out = out+5;
            out = inor(t->right, out);
            *(out++) = ')';
            *(out++) = ')';
            break;
        case 'T':   //(right'/cos(right)^2)
            *(out++) = '(';
            out = autograd(t->right, variable, out);
            strcpy(out, "/cos(");
            out = out+5;
            out = inor(t->right, out);
            strcpy(out, ")^2)");
            out = out+4;
            break;
        default:    //if variable
            if(strcmp(t->c, variable)==0)*(out++)='1';
            else *(out++)='0';
            break;
    }
    return out;
}

char *inor(Tree t, char* out){   //inorder the tree and store in out
    if(t==NULL)return out;   //protect
    else if(strcmp(t->c, "L")==0){   //(log(left,right))
        strcpy(out, "(log(");
        out = out+5;
        if(t->left!=NULL)out = inor(t->left, out);
        *(out++) = ',';
        if(t->right!=NULL)out = inor(t->right, out);
        *(out++) = ')';
        *(out++) = ')';
        return out;
    }else if(strcmp(t->c, "C")==0||strcmp(t->c, "S")==0||strcmp(t->c, "T")==0){  //(cos/sin/tan(right))
        if(strcmp(t->c, "C")==0)strcpy(out, "(cos");
        else if(strcmp(t->c, "S")==0)strcpy(out, "(sin");
        else strcpy(out, "(tan");
        out = out+4;
        if(t->right!=NULL)out = inor(t->right, out);
        *(out++) = ')';
        return out;
    }

    *(out++) = '(';    //(left t->c right)
    if(t->left!=NULL)out = inor(t->left, out);
    if(strcmp(t->c, "P")==0)strcpy(out, "^");
    else strcpy(out, t->c);
    out = out+strlen(t->c);
    if(t->right!=NULL)out = inor(t->right, out);
    *(out++) = ')';
    return out;
}



int check0(Tree t, Tree parent, int lor){   //to wipe the item with 0

    if(t->left==NULL&&t->right==NULL){  //if leaf
        if(strcmp(t->c, "0")==0){   //if 0
            return 0;
        }else{
            return 1;
        }
    }

    if(t->left!=NULL&&!check0(t->left, t, 0)){  //if left include 0
        switch (*(t->c)){
        case '*':   //0*x
        case '^':   //0^x
        case '/':   //0/x
            return 0;  //=0
            break;
        case '-':   //0-x
            if(check0(t->right, t, 1)==0)return 0;  //0-0=0
            else {    //0-x(x!=0)
                strcpy(t->left->c, "0");
                return 1;
            }
            break;
        case '+':   //0+x
            if(check0(t->right, t, 1)==0)return 0;  //0+0=0
            else {  //0+x=x (x!=0)
                if(parent!=NULL){
                    if(lor==0)parent->left = t->right;
                    else parent->right = t->right;
                }
                else backT = t->right;
                return 1;
            }
            break;
        }
    }else if(t->right!=NULL&&!check0(t->right, t, 1)){   //left!=0, right=0
        switch (*(t->c)){
        case '*':   //x*0
        case 'S':   //sin0
        case 'T':   //tan0
            return 0;  //=0
            break;
        case '-':   //x-0
        case '+':   //x+0
            if(parent!=NULL){   //=x
                if(lor==0)parent->left = t->left;
                else parent->right = t->left;
            }
            else backT = t->left;
            return 1;
            break;
        case '^':   //x^0
        case 'C':   //cos0
            strcpy(t->c, "1");   //=1
            t->right = NULL;
            t->left = NULL;
            return 1;
            break;
        }
    }
    return 1;
}

char *inor_sim(Tree t, char* out, Tree parent){   //a shorter inorder
    if(t==NULL)return out;   //protect
    switch(*(t->c)){   
        case 'L':   //log(E,x)=ln(x)
            if(strcmp(t->left->c, "E")==0){
                strcpy(out, "ln(");
                out = out+3;
                if(t->right!=NULL)out = inor_sim(t->right, out, t);
                *(out++) = ')';
            }else{   //log(left,right)
                strcpy(out, "log(");
                out = out+4;
                if(t->left!=NULL)out = inor_sim(t->left, out, t);
                *(out++) = ',';
                if(t->right!=NULL)out = inor_sim(t->right, out, t);
                *(out++) = ')';
            }
            break;
        case 'S':
        case 'C':
        case 'T':   //sin/cos/tan(right)
            if(strcmp(t->c, "C")==0)strcpy(out, "cos(");
            else if(strcmp(t->c, "S")==0)strcpy(out, "sin(");
            else strcpy(out, "tan(");
            out = out+4;
            if(t->right!=NULL)out = inor_sim(t->right, out, t);
            *(out++) = ')';
            break;
        case '+':
        case '-':
        case '/':
            if(parent!=NULL&&(*(parent->c)=='*'||*(parent->c)=='/'||*(parent->c)=='^')){  //lower priority, need of ()
                *(out++) = '(';
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
                *(out++) = ')';
            }else{     //higher priority, no need of ()
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
            }
            break;
        case '*':

            if(parent!=NULL&&(*(parent->c)=='/'||*(parent->c)=='^')){   //lower priority, need of ()
                *(out++) = '(';
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
                *(out++) = ')';
            }else{    //higher priority, no need of ()
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
            }
            break;
        case '^':
            if(parent!=NULL&&*(parent->c)=='^'){   //lower priority, need of ()
                *(out++) = '(';
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
                *(out++) = ')';
            }else{      //higher priority, no need of ()
                out = inor_sim(t->left, out, t);
                strcpy(out, t->c);
                out = out+strlen(t->c);
                out = inor_sim(t->right, out, t);
            }
            break;
        default:    //variable or integer, just output
            strcpy(out, t->c);
            out = out+strlen(t->c);
            break;
    }
    return out;
}

int check1(Tree t, Tree parent, int lor){   //wipe off *1

    if(t->left==NULL&&t->right==NULL){    //if leaf
        if(strcmp(t->c, "1")==0){
            return 0;
        }else{
            return 1;
        }
    }

    if(t->left!=NULL&&!check1(t->left, t, 0)){    //if left =1
        if(*(t->c)=='*'){   //1*
            if(!check1(t->right, t, 1)){   //1*1 = 1
                strcpy(t->c, "1");
                return 0;
            }else if(parent==NULL){   //t=root, right
                backT = t->right;
                return 1;
            }else if(lor==0){   //t!=root, wipe *1
                parent->left = t->right;
                return 1;
            }else{
                parent->right = t->right;
                return 1;
            }
        }
    }else if(t->right!=NULL&&!check1(t->right, t, 1)){   //left!=1, right=1
        if(*(t->c)=='*'){    //x*1=x
            if(parent==NULL){  //t=root
                backT = t->left;
                return 1;
            }else if(lor==0){   //t!=root
                parent->left = t->left;
                return 1;
            }else{
                parent->right = t->left;
                return 1;
            }
        }
    }
    return 1;
}
