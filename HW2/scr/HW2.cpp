#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Polynomial;  //���I�sPolynomial���O 

//�ΨӪ�ܳ涵��(��Ӷ���)�����O
class Term
{
    friend class Polynomial;   //�� Polynomial���O�����s��Term���p������
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);

private:
    float coef;  //�Y�� 
    int exp;  	 //���� 
};

// ��ܦh������ Polynomial ���O
class Polynomial
{
public:
    Polynomial();  //�غc�l
    Polynomial Add(const Polynomial& poly) const;   //�h�����ۥ[
    Polynomial Mult(const Polynomial& poly) const;   //�h�����ۭ�
    float Eval(float f) const;  //�p��h�����b�Y�I����
    friend ostream& operator<<(ostream& out, const Polynomial& poly);  //��X�h����
    friend istream& operator>>(istream& in, Polynomial& poly);  //��J�h����

private:
    Term* termArray;  //�x�s�h�������D�s��
    int capacity;	 //termArray���j�p
    int terms;       //��e�D�s�����ƶq

    void resize();   //�վ� termArray ���j�p
};

//�w�]�غc�l�G�إߪŦh����
Polynomial::Polynomial()
{
    capacity = 10;               	 //�w�]�̦h���x�s10��
    terms = 0;                   	 //��l�ɦh���ƬO�Ū�
    termArray = new Term[capacity];  //�t�m�O����
}

//�ʺA�վ�termArray�j�p�����
void Polynomial::resize()
{
    capacity *= 2;  //�N�e�q*2�X�W 
    Term* newArray = new Term[capacity];
    for (int i = 0; i < terms; i++)
	{
        newArray[i] = termArray[i];
    }
    delete[] termArray;    //�R���쥻���e 
    termArray = newArray;  //�����¸�ƨ�s�}�C 
}

//�h�����ۥ[
Polynomial Polynomial::Add(const Polynomial& poly) const
{
    Polynomial result;
    int i = 0, j = 0;
    while (i < terms && j < poly.terms)
	{
        if (termArray[i].exp == poly.termArray[j].exp)  //�@�˦���ۥ[ 
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
        else if (termArray[i].exp > poly.termArray[j].exp)  //this�h������������� 
		{
            result.termArray[result.terms++] = termArray[i++];
        }
        else  //�ǤJ�h������������� 
		{
            result.termArray[result.terms++] = poly.termArray[j++];
        }
        if (result.terms == result.capacity) 
		{ 
			result.resize();
    	} 
	}
    while (i < terms)  //��ѤU���[�J
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

//�h�����ۭ�
Polynomial Polynomial::Mult(const Polynomial& poly) const
{
    Polynomial result;
    for (int i = 0; i < terms; i++)
	{
        for (int j = 0; j < poly.terms; j++)
		{
            float prodCoef = termArray[i].coef * poly.termArray[j].coef;  //�Y�Ƭۭ�
            int prodExp = termArray[i].exp + poly.termArray[j].exp;  	  //����ۥ[
            bool found = false;
            for (int k = 0; k < result.terms; k++)
			{
                if (result.termArray[k].exp == prodExp)  //�p�G���ۦP���誺�X�֦b�@�_ 
				{
                    result.termArray[k].coef += prodCoef;
                    found = true;
                    break;
                }
            }
            if (!found)  //�p�G�S���ۦP���N�s�W�@��
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

//�p��h�����b�Y�I����
float Polynomial::Eval(float f) const
{
    float result = 0;
    for (int i = 0; i < terms; i++)
	{
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

//��J�h����
istream& operator>>(istream& in, Polynomial& poly)
{
    cout << "��J�h����������: ";
    in >> poly.terms;
    if (poly.terms > poly.capacity)
	{
        delete[] poly.termArray;
        poly.capacity = poly.terms;
        poly.termArray = new Term[poly.capacity];
    }
    cout << "�̦���J�Y�ƻP����]�Ҧp�G3 2 ��� 3x^2�^\n";
    for (int i = 0; i < poly.terms; i++)
	{
        in >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return in;
}

//��X�h����
ostream& operator<<(ostream& out, const Polynomial& poly)
{
    for (int i = 0; i < poly.terms; i++)
	{
        out << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        if (i < poly.terms - 1) out << " + ";
    }
    return out;
}

//�D�{��
int main()
{
    Polynomial p1, p2, p3;
    cout << "��J�Ĥ@�Ӧh����:\n";
    cin >> p1;
    cout << "��J�ĤG�Ӧh����:\n";
    cin >> p2;

    cout << "�Ĥ@�Ӧh����: " << p1 << endl;
    cout << "�ĤG�Ӧh����: " << p2 << endl;

    p3 = p1.Add(p2);
    cout << "�[�k���G: " << p3 << endl;

    p3 = p1.Mult(p2);
    cout << "���k���G: " << p3 << endl;

    float evalPoint;
    cout << "��J�@�ӼƦr�p��Ĥ@�Ӧh��������: ";
    cin >> evalPoint;
    cout << "�p�⵲�G: " << p1.Eval(evalPoint) << endl;

    return 0;
}

