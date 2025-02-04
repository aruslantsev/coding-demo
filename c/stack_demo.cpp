#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
// #include <iomanip> // for setw
#include "stack.h"
#include <chrono>
#include <thread>


#define CMDLENGTH 20

using namespace std;

void print_help()
{
/*    cout << "Модель математического сопроцессора с обратной польской записью" << endl 
         << "Введите h или help для помощи, q, e, quit или exit для выхода из программы" << endl 
         << "init для приведения стека в исходное состояние" << endl 
         << "push число для добавления числа в стек, pop для удаления элемента из стека," << endl 
         << "save для сохранения вершины стека, load для возврата сохраненного значения в стек," << endl
         << "top для вывода верхнего элемента стека, list для вывода стека на экран," << endl
         << "add для сложения, sub для вычитания, mul для умножения, div для деления," << endl
         << "inv для смены знака, abs для вычисления абсолютного значения, rnd для округления до целого," << endl         
         << "xchg для того, чтобы поменять верхние 2 элемента местами" << endl
         << "sqroot для извлечения квадратного корня, pow2 для вычисления 2^x, log2 для вычисления логарифма" << endl
         << "sin для вычисления синуса, cos для вычисления косинуса, tan для вычисления тангенса," << endl
         << "asin для вычисления арксинуса, acos для вычисления арккосинуса, atan для вычисления арктангенса," << endl
         << "sinh для вычисления гиперболического синуса, cosh для вычисления гиперболического косинуса," << endl
         << "asinh для вычисления гиперболического арксинуса, acohs для вычисления гиперболического арккосинуса" << endl
         << "cmp для сравнения первого и второго чисел в стеке между собой:" << endl
         << "возвращает в стек 0 если числа равны, 1 если первое число больше, -1 если второе число больше," << endl
         << "test возвращает 0 если число на вершине стека равно 0, 1 если число больше 0, -1 если число меньше 0" << endl;*/
    cout << "Список доступных команд: \ninit, push, pop, save, load, top, list, add, sub, mul, div, inv, abs, rnd, xchg, sqroot, pow2, log2, sin, cos, tan, asin, acos, atan, sinh, cosh, asinh, acosh, cmp, test, wait." << endl;
    return;
}

int main(int argc, char *argv[])
{
    stack s;
    ifstream inf;
    ofstream of;
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    // setw(10); cout.width(10);
    cout.precision(3);
    double val, a, b, t;
    bool saved = 0, fileio = 0;
    char op[CMDLENGTH];
    
    if (argc == 3)
    {
        fileio = 1;
        inf.open(argv[1], ios::in);
        of.open(argv[2], ios::out);
    }
    
    if (!fileio)
        print_help();
    
    do
    {
        if (!strcmp(op, "h") || !strcmp(op, "help"))
            print_help();
        
        if (!strcmp(op, "init"))
            s.init();
        
        if (!strcmp(op, "push"))
        {
            if (!fileio)
                cin >> val; // scanf("%lf", &val);
            else
                inf >> val;
            s.push(val);
        }
        
        if (!strcmp(op, "pop") && s.size() >= 1) 
        {
            s.pop();
        }
                
        if (!strcmp(op, "top") && s.size() >= 1) 
        {
            double top;
            top = s.pop();
            s.push(top);
            if (!fileio)
                cout << top << endl; // printf("%lf\n", top);
            else
                of << top << endl;
        }
                
        if (!strcmp(op, "list") && s.size() > 0)
            s.list();
        
        if (!strcmp(op, "save") && s.size() >= 1) 
        {
            t = s.pop();
            s.push(t);
            saved = 1;
            
        }
        
        if (!strcmp(op, "load") && saved)
        {
            s.push(t);
        }
        
                    
        if (!strcmp(op, "xchg") && s.size() >= 2)
        {
            a = s.pop();
            b = s.pop();
            s.push(a);
            s.push(b);
            
        }
            
        if (!strcmp(op, "comp") && s.size() >= 2)
        {
            a = s.pop();
            b = s.pop();
            s.push(b);
            s.push(a);
            if (a > b)
                s.push(1);
            if (a < b)
                s.push(-1);
            if (a == b)
                s.push(0);
        }
        
        if (!strcmp(op, "test") && s.size() >= 1)
        {
            a = s.pop();
            s.push(a);
            if (a > 0)
                s.push(1);
            if (a < 0)
                s.push(-1);
            if (a == 0)
                s.push(0);
        }
            
        if (!strcmp(op, "add") && s.size() >= 2)
        {
            // a = s.pop();
            // b = s.pop();
            // s.push(a + b);
            s.push(s.pop() + s.pop());       
        }
            
        if (!strcmp(op, "sub") && s.size() >= 2)
        {
            s.push(s.pop() - s.pop());
        }
            
        if (!strcmp(op, "mul") && s.size() >= 2)
        {
            s.push(s.pop() * s.pop());
        }
            
        if (!strcmp(op, "div") && s.size() >= 2)
        {
            s.push(s.pop() / s.pop());
        }
            
        if (!strcmp(op, "inv") && s.size() >= 1)
        {
            s.push(-s.pop());
        }
            
        
        if (!strcmp(op, "abs") && s.size() >= 1)
        {
            s.push(abs(s.pop()));
        }
            
        if (!strcmp(op, "rnd") && s.size() >= 1)
        {
            s.push(floor(s.pop() + 0.5));
        }
            
        if (!strcmp(op, "sqroot") && s.size() >= 1)
        {
            s.push(sqrt(s.pop()));
        }
            
        if (!strcmp(op, "sin") && s.size() >= 1)
        {
            s.push(sin(s.pop()));
        }
            
        if (!strcmp(op, "cos") && s.size() >= 1)
        {
            s.push(cos(s.pop()));
        }
            
        if (!strcmp(op, "tan") && s.size() >= 1)
        {
            s.push(tan(s.pop()));
        }
            
        if (!strcmp(op, "asin") && s.size() >= 1)
        {
            s.push(asin(s.pop()));
        }
            
        if (!strcmp(op, "acos") && s.size() >= 1)
        {
            s.push(acos(s.pop()));
        }
            
        if (!strcmp(op, "atan") && s.size() >= 1)
        {
            s.push(atan(s.pop()));
        }
            
        if (!strcmp(op, "sinh") && s.size() >= 1)
        {
            s.push(sinh(s.pop()));
        }
            
        if (!strcmp(op, "cosh") && s.size() >= 1)
        {
            s.push(cosh(s.pop()));
        }
            
        if (!strcmp(op, "asinh") && s.size() >= 1)
        {
            s.push(asinh(s.pop()));
        }
            
        if (!strcmp(op, "acosh") && s.size() >= 1)
        {
            s.push(acosh(s.pop()));
        }
            
        if (!strcmp(op, "log2") && s.size() >= 1)
        {
            s.push(log2(s.pop()));
        }
        if (!strcmp(op, "pow2") && s.size() >= 1)
        {
            s.push(pow(2, s.pop())); 
        }
        
        if (!strcmp(op, "wait") && s.size() >= 1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }
        
        /*if (!strcmp(op, "size") && s.size() >= 1)
        {
            cout << sizeof(s) << endl; 
        }*/
                
        if (argc == 1)
        {
            cout << "--> ";
            cin >> op;
        }
        else 
            inf >> op;
    }
    while (strcmp(op, "q") && strcmp(op, "e") && strcmp(op, "quit") && strcmp(op, "exit") && !(inf.eof() && fileio));
    if (fileio)
    {
        inf.close();
        of.close();
    }
    return 0;
}


