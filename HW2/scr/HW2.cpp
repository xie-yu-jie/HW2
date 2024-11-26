#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Polynomial;  //先呼叫Polynomial類別 

//用來表示單項式(單個項目)的類別
class Term
{
    friend class Polynomial;   //讓 Polynomial類別直接存取Term的私有成員
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);

private:
    float coef;  //係數 
    int exp;  	 //次方 
};

// 表示多項式的 Polynomial 類別
class Polynomial
{
public:
    Polynomial();  //建構子
    Polynomial Add(const Polynomial& poly) const;   //多項式相加
    Polynomial Mult(const Polynomial& poly) const;   //多項式相乘
    float Eval(float f) const;  //計算多項式在某點的值
    friend ostream& operator<<(ostream& out, const Polynomial& poly);  //輸出多項式
    friend istream& operator>>(istream& in, Polynomial& poly);  //輸入多項式

private:
    Term* termArray;  //儲存多項式的非零項
    int capacity;	 //termArray的大小
    int terms;       //當前非零項的數量

    void resize();   //調整 termArray 的大小
};

//預設建構子：建立空多項式
Polynomial::Polynomial()
{
    capacity = 10;               	 //預設最多能儲存10項
    terms = 0;                   	 //初始時多項事是空的
    termArray = new Term[capacity];  //配置記憶體
}

//動態調整termArray大小的函數
void Polynomial::resize()
{
    capacity *= 2;  //將容量*2擴增 
    Term* newArray = new Term[capacity];
    for (int i = 0; i < terms; i++)
	{
        newArray[i] = termArray[i];
    }
    delete[] termArray;    //刪除原本內容 
    termArray = newArray;  //拷貝舊資料到新陣列 
}

//多項式相加
Polynomial Polynomial::Add(const Polynomial& poly) const
{
    Polynomial result;
    int i = 0, j = 0;
    while (i < terms && j < poly.terms)
	{
        if (termArray[i].exp == poly.termArray[j].exp)  //一樣次方相加 
		{
            float sumCoef = termArray[i].coef + poly.termArray[j].coef;
            if (sumCoef != 0)
			{
                result.termArray[result.terms].coef = sumCoef;
                result.termArray[result.terms].exp = termArray[i].exp;
                result.terms++;
            }
            i++;
            j++;
        }
        else if (termArray[i].exp > poly.termArray[j].exp)  //this多項式的次方較高 
		{
            result.termArray[result.terms++] = termArray[i++];
        }
        else  //傳入多項式的次方較高 
		{
            result.termArray[result.terms++] = poly.termArray[j++];
        }
        if (result.terms == result.capacity) 
		{ 
			result.resize();
    	} 
	}
    while (i < terms)  //把剩下的加入
	{
        result.termArray[result.terms++] = termArray[i++];
        if (result.terms == result.capacity) result.resize();
    }
    while (j < poly.terms)
	{
        result.termArray[result.terms++] = poly.termArray[j++];
        if (result.terms == result.capacity) result.resize();
    }
    return result;
}

//多項式相乘
Polynomial Polynomial::Mult(const Polynomial& poly) const
{
    Polynomial result;
    for (int i = 0; i < terms; i++)
	{
        for (int j = 0; j < poly.terms; j++)
		{
            float prodCoef = termArray[i].coef * poly.termArray[j].coef;  //係數相乘
            int prodExp = termArray[i].exp + poly.termArray[j].exp;  	  //次方相加
            bool found = false;
            for (int k = 0; k < result.terms; k++)
			{
                if (result.termArray[k].exp == prodExp)  //如果找到相同次方的合併在一起 
				{
                    result.termArray[k].coef += prodCoef;
                    found = true;
                    break;
                }
            }
            if (!found)  //如果沒有相同的就新增一項
			{
                result.termArray[result.terms].coef = prodCoef;
                result.termArray[result.terms].exp = prodExp;
                result.terms++;
                if (result.terms == result.capacity) result.resize();
            }
        }
    }
    return result;
}

//計算多項式在某點的值
float Polynomial::Eval(float f) const
{
    float result = 0;
    for (int i = 0; i < terms; i++)
	{
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

//輸入多項式
istream& operator>>(istream& in, Polynomial& poly)
{
    cout << "輸入多項式的項數: ";
    in >> poly.terms;
    if (poly.terms > poly.capacity)
	{
        delete[] poly.termArray;
        poly.capacity = poly.terms;
        poly.termArray = new Term[poly.capacity];
    }
    cout << "依次輸入係數與次方（例如：3 2 表示 3x^2）\n";
    for (int i = 0; i < poly.terms; i++)
	{
        in >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return in;
}

//輸出多項式
ostream& operator<<(ostream& out, const Polynomial& poly)
{
    for (int i = 0; i < poly.terms; i++)
	{
        out << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        if (i < poly.terms - 1) out << " + ";
    }
    return out;
}

//主程式
int main()
{
    Polynomial p1, p2, p3;
    cout << "輸入第一個多項式:\n";
    cin >> p1;
    cout << "輸入第二個多項式:\n";
    cin >> p2;

    cout << "第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;

    p3 = p1.Add(p2);
    cout << "加法結果: " << p3 << endl;

    p3 = p1.Mult(p2);
    cout << "乘法結果: " << p3 << endl;

    float evalPoint;
    cout << "輸入一個數字計算第一個多項式的值: ";
    cin >> evalPoint;
    cout << "計算結果: " << p1.Eval(evalPoint) << endl;

    return 0;
}

