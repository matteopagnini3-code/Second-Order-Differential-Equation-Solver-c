#include <stdio.h>
#define PI 3.14159265358979323846

int mystrcspn(char* s,char rm){
    int i=0;
    while(s[i]!='\0' && s[i]!=rm) i++;
    return i;
}

float mysin(float x){
    float term=x, sum=x;
    for(int i=1;i<6;i++){
        term *= -x*x/((2*i)*(2*i+1));
        sum += term;
    }
    return sum;
}

float mycos(float x){
    float term=1, sum=1;
    for(int i=1;i<6;i++){
        term *= -x*x/((2*i-1)*(2*i));
        sum += term;
    }
    return sum;
}

float mye(float x){
    float term=1, sum=1;
    for(int i=1;i<10;i++){
        term *= x/i;
        sum += term;
    }
    return sum;
}

float mylog(float x){
    float y=(x-1)/(x+1);
    float y2=y*y;
    float term=y, sum=0;
    for(int i=1;i<10;i+=2){
        sum += term/i;
        term *= y2;
    }
    return 2*sum;
}

/* Algorithm:
value = 0
if at the beginning -> skip until '='
while not at the end:
    if current part is evaluable -> value += expression
    move index to next evaluable part

Expression evaluation:
store operator
if starts with number or y, y', t -> update value
if starts with sin -> value += sin(evaluate(arg))
if '(' -> evaluate inside
if ')' -> return value to previous recursion
otherwise -> skip character
*/

float valuta(char* eqdiff, float t, float y, float yp, int* i) {
    float value = 0;

    if(*i == 0) {
        while(eqdiff[*i] != '=' && eqdiff[*i] != '\0')
            (*i)++;
        if(eqdiff[*i] == '=') (*i)++;
    }

    char op = '+';
    while(eqdiff[*i] != '\0') {

        // numbers (integer or decimal)
        if((eqdiff[*i] >= '0' && eqdiff[*i] <= '9') || eqdiff[*i] == '.') {
            float num = 0, dec = 0.1;
            int decimal = 0;

            while((eqdiff[*i] >= '0' && eqdiff[*i] <= '9') || eqdiff[*i] == '.') {
                if(eqdiff[*i] == '.') decimal = 1;
                else {
                    if(!decimal) num = num * 10 + (eqdiff[*i] - '0');
                    else {
                        num += (eqdiff[*i] - '0') * dec;
                        dec *= 0.1;
                    }
                }
                (*i)++;
            }

            if(op == '+') value += num;
            else if(op == '-') value -= num;
            else if(op == '*') value *= num;
            else if(op == '/') value /= num;
        }

        // y'
        else if(eqdiff[*i] == 'y' && eqdiff[*i+1] == '\'') {
            if(op == '+') value += yp;
            else if(op == '-') value -= yp;
            else if(op == '*') value *= yp;
            else if(op == '/') value /= yp;
            (*i) += 2;
        }

        // y
        else if(eqdiff[*i] == 'y') {
            if(op == '+') value += y;
            else if(op == '-') value -= y;
            else if(op == '*') value *= y;
            else if(op == '/') value /= y;
            (*i)++;
        }

        // t
        else if(eqdiff[*i] == 't') {
            if(op == '+') value += t;
            else if(op == '-') value -= t;
            else if(op == '*') value *= t;
            else if(op == '/') value /= t;
            (*i)++;
        }

        // functions: sin, cos, exp, log
        else if(eqdiff[*i] == 's' && eqdiff[*i+1] == 'i' && eqdiff[*i+2] == 'n') {
            (*i) += 3;
            if(eqdiff[*i] == '(') (*i)++;
            float arg = valuta(eqdiff, t, y, yp, i);
            if(eqdiff[*i] == ')') (*i)++;
            if(op == '+') value += mysin(arg);
            else if(op == '-') value -= mysin(arg);
            else if(op == '*') value *= mysin(arg);
            else if(op == '/') value /= mysin(arg);
        }
        else if(eqdiff[*i] == 'c' && eqdiff[*i+1] == 'o' && eqdiff[*i+2] == 's') {
            (*i) += 3;
            if(eqdiff[*i] == '(') (*i)++;
            float arg = valuta(eqdiff, t, y, yp, i);
            if(eqdiff[*i] == ')') (*i)++;
            if(op == '+') value += mycos(arg);
            else if(op == '-') value -= mycos(arg);
            else if(op == '*') value *= mycos(arg);
            else if(op == '/') value /= mycos(arg);
        }
        else if(eqdiff[*i] == 'e' && eqdiff[*i+1] == 'x' && eqdiff[*i+2] == 'p') {
            (*i) += 3;
            if(eqdiff[*i] == '(') (*i)++;
            float arg = valuta(eqdiff, t, y, yp, i);
            if(eqdiff[*i] == ')') (*i)++;
            if(op == '+') value += mye(arg);
            else if(op == '-') value -= mye(arg);
            else if(op == '*') value *= mye(arg);
            else if(op == '/') value /= mye(arg);
        }
        else if(eqdiff[*i] == 'l' && eqdiff[*i+1] == 'o' && eqdiff[*i+2] == 'g') {
            (*i) += 3;
            if(eqdiff[*i] == '(') (*i)++;
            float arg = valuta(eqdiff, t, y, yp, i);
            if(eqdiff[*i] == ')') (*i)++;
            if(op == '+') value += mylog(arg);
            else if(op == '-') value -= mylog(arg);
            else if(op == '*') value *= mylog(arg);
            else if(op == '/') value /= mylog(arg);
        }

        // parentheses: opening
        else if(eqdiff[*i] == '(') {
            (*i)++;
            float arg = valuta(eqdiff, t, y, yp, i);
            if(eqdiff[*i] == ')') (*i)++;
            if(op == '+') value += arg;
            else if(op == '-') value -= arg;
            else if(op == '*') value *= arg;
            else if(op == '/') value /= arg;
        }

        // closing parenthesis: return from recursion
        else if(eqdiff[*i] == ')') {
            (*i)++;
            return value;
        }

        // operators
        else if(eqdiff[*i] == '+' || eqdiff[*i] == '-' || eqdiff[*i] == '*' || eqdiff[*i] == '/') {
            op = eqdiff[*i];
            (*i)++;
        }

        else {
            (*i)++; // ignore unrecognized characters
        }
    }

    return value;
}

void rk3(char* eqdiff,float t,float* y,float* v,float h){

    float k1,k2,k3,l1,l2,l3;
    int index;

    index=0;
    k1=valuta(eqdiff,t,*y,*v,&index);
    l1=*v;

    index=0;
    k2=valuta(eqdiff,t+h/2,*y+h*l1/2,*v+h*k1/2,&index);
    l2=*v+h*k1/2;

    index=0;
    k3=valuta(eqdiff,t+h,*y-h*l1+2*h*l2,*v-h*k1+2*h*k2,&index);
    l3=*v+h*k2;

    *v += (h/6)*(k1+4*k2+k3);
    *y += (h/6)*(l1+4*l2+l3);
}

int main(){

    char eqdiff[100];
    float t0, y0, v0;
    float h;
    int n;

    printf("Enter a differential equation y''(t)=f(y'(t),y(t),t):\n");
    fgets(eqdiff, 100, stdin);
    eqdiff[mystrcspn(eqdiff,'\n')] = '\0';

    printf("Enter t0, y(t0), y'(t0): ");
    scanf("%f %f %f", &t0, &y0, &v0);

    printf("Enter step size h and number of steps n: ");
    scanf("%f %d", &h, &n);

    float y = y0, v = v0;

    printf("\n");
    for(int i = 0; i < n; i++){
        rk3(eqdiff, t0 + i*h, &y, &v, h);
        printf("t = %f, y(t) = %f, y'(t) = %f\n", t0 + i*h, y, v);
    }

    return 0;
}
