#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include<stdarg.h>
template <class _Type>
class BasicMatrix{
	
public:
#ifdef _IOSTREAM_
	template <class _Type>
	friend std::istream& operator>>(std::istream& in, BasicMatrix<_Type>& a);
	template <class _Type>
	friend std::ostream& operator<<(std::ostream& out, BasicMatrix<_Type>& a);
#endif
	std::vector<std::vector<_Type>> _elem;
	BasicMatrix(){
	}
	BasicMatrix(size_t height){
		_elem.resize(height, std::vector<_Type>(height));
	}
	BasicMatrix(size_t height, size_t width){
		_elem.resize(height, std::vector<_Type>(width));
	}
	BasicMatrix(const BasicMatrix& _new){
		_elem = _new._elem;
	}
	size_t width(){
		return _elem[0].size();
	}
	size_t height(){
		return _elem.size();
	}

	std::vector<_Type> line(size_t i){
		return _elem[i];
	}
	bool operator!=(const int& x){
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				if (_elem[i][j] != x)return true;
			}
		}
		return false;
	}
	bool operator!=(const BasicMatrix& x){
		if (_elem.size() < x._elem.size() || _elem[0].size() < x._elem[0].size())
			return true;
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				if (_elem[i][j] != x._elem[i][j])return true;
			}
		}
		return false;
	}
	bool operator==(const int& x){
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				if (_elem[i][j] != x)return false;
			}
		}
		return true;
	}
	bool operator==(const BasicMatrix& x){
		if (_elem.size() < x._elem.size() || _elem[0].size() < x._elem[0].size())
			return false
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				if (_elem[i][j] != x._elem[i][j])return false;
			}
		}
		return true;
	}
	std::vector<_Type>& operator[](size_t i){
		return _elem[i];
	}
};
template <class _Type>
class Matrix : public BasicMatrix<_Type>{
public:
	Matrix() :BasicMatrix(){}
	Matrix(size_t height) : BasicMatrix(height){}
	Matrix(size_t height, size_t width): BasicMatrix(height,width){}
	Matrix(const BasicMatrix& _new): BasicMatrix(_new){}
	Matrix(size_t height, size_t width, _Type x){
		_elem.resize(height, std::vector<_Type>(width, x));
	}
	
	Matrix(std::vector<_Type> _new){
		_elem.resize(_new._elem.size(), std::vector<_Type>(_new._elem[0].size()));
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				_elem[i][j] = _new._elem[i][j];
			}
		}
	}
	Matrix(_Type** _new, size_t height, size_t width){
		_elem.resize(height, std::vector<_Type>(width));
		for (size_t i = 0; i<_elem.size(); i++){
			for (size_t j = 0; j<_elem[i].size(); j++){
				_elem[i][j] = _new[i][j];
			}
		}
	}
	Matrix(size_t height, size_t width, _Type x, _Type x1, ...){
		_elem.resize(height, std::vector<_Type>(width, x));
		va_list argptr;
		va_start(argptr, width);
		_Type t;
		for (size_t i = 0; i<height; i++){
			for (size_t j = 0; j<width; j++){
				t = va_arg(argptr, _Type);
				_elem[i][j] = t;
			}
		}
		va_end(argptr);
	}
	void eraseLine(size_t i){
		_elem.erase(_elem.begin()+i);
		}
	void eraseColumn(size_t j){
		for(size_t i=0;i<height();i++){
				_elem[i].erase(_elem[i].begin()+j);

			}
		}
	Matrix& operator = (const int & x){
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				_elem[i][j] = x;
				}
			}
		}
	Matrix& operator = (const Matrix & x){
		this->_elem.clear();
		this->_elem.resize(x._elem.size(),std::vector<_Type>(x._elem[0].size()));
			//Обработка ошибки
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				_elem[i][j] = x._elem[i][j];
				}
			}
		return *this;
		}
	
	void operator+= (const int& x){
		Matrix temp(_elem.size(),_elem[0].size(),x);
		*this= *this + temp;
	}
	void operator+= (const Matrix& x){
		*this = *this + x;
	}
	void operator-= (const int& x){
		Matrix temp(_elem.size(),_elem[0].size(),x);
		*this= *this - temp;
	}
	void operator-= (const Matrix& x){
		*this= *this - x;
	}
	void operator*= (const int& x){
		Matrix temp(_elem.size(),_elem[0].size(),x);
		*this = *this * temp;
	}
	void operator*= (const Matrix& x){
		*this = *this * x;
	}
/*	void operator/= (const int& x){
		Matrix temp(_elem.size(),_elem[i].size(),x);
		*this = *this / temp;
	}
	void operator/= (const Matrix& x){
		*this = *this / x
	}*/
	Matrix<_Type> operator+(const int& x){
		Matrix temp(_elem.size(),_elem[0].size(),x);
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				temp._elem[i][j] += _elem[i][j];
				}
			}
		return temp;
		}
	Matrix<_Type> operator+(const Matrix& x){
		if(_elem.size() < x._elem.size() || _elem[0].size() < x._elem[0].size()){}
			//Обработка ошибки
		Matrix<_Type> temp(x);
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				temp._elem[i][j] += _elem[i][j];
				}
			}
		return temp;
		}
	Matrix operator-(const int& x){
		Matrix temp(_elem.size(),_elem[0].size(),x);
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				temp._elem[i][j] -= _elem[i][j];
				}
			}
		return temp;
		}
	Matrix operator-(const Matrix& x){
		if(_elem.size() < x._elem.size() || _elem[0].size() < x._elem[0].size()){}
			//Обработка ошибки
		Matrix temp(x);
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				temp._elem[i][j] -= _elem[i][j];
				}
			}
		return temp;
		}
	Matrix operator*(const int& x){
		Matrix temp =*this;
		for(size_t i = 0;i<_elem.size();i++){
			for(size_t j = 0;j<_elem[i].size();j++){
				temp._elem[i][j] *= x;
				}
			}
		return temp;
	}
	Matrix operator*(const Matrix& x){
		if(_elem[0].size() != x._elem.size()){
			//оюработка ошибки
			}
		Matrix c(_elem.size(),x._elem[0].size());
		for(int i = 0; i < _elem.size(); i++){
			for(int j = 0; j < x._elem[0].size(); j++){
				c[i][j] = 0;
				for(int k = 0; k < _elem.size(); k++){
					c[i][j] += _elem[i][k] * x._elem[k][j];
					}
				}
			}
		return c;
	}
/*	Matrix inv(){

		}
	Matrix operator/(const int& x){
		Matrix temp(*p / x);
		return temp;
	}
	Matrix operator/(const Matrix& x){
		Matrix temp(*p / *x.p);
		return temp;
	}
	Matrix operator%(const int& x){
		Matrix temp(*p % x);
		return temp;
	}
	Matrix operator%(const Matrix& x){
		Matrix temp(*p % *x.p);
		return temp;
	}
*/
	std::vector<_Type>& operator[](size_t i){
		return _elem[i];
		}
	double det(){
		if(_elem.size() != _elem[0].size())return 0;
		size_t n = _elem.size();
		int deep = 0;
		return det(_elem,n,deep);
		}
	Matrix<_Type> T(){
		Matrix<_Type> temp(_elem[0].size(),_elem.size());
		for(size_t i=0;i<_elem[0].size();i++){
			for(size_t j=0;j<_elem.size();j++){
				temp[i][j] = _elem[j][i];
				}
			}
		return temp;
		}
	private:
	double det(std::vector<std::vector<int>> elem,size_t n,int deep){
		if(n==2)return elem[0][0+deep]*elem[1][1+deep]-elem[0][1+deep]*elem[1][0+deep];
		int znak=1,rez=0;
		for(int i=0;i<n;i++){
			std::vector<std::vector<int>> M2(n-1);
			for(int j=0,k=0;j<n;j++){
				if(j==i)continue;
				M2[k]=elem[j];
				k++;	
				}
			rez+=det(elem,M2.size(),deep+1)*znak*elem[i][deep];
			znak*=-1;
			M2.clear();
			}
		return rez;
		}
	};
	template <class _Type>
	std::istream& operator>>(std::istream& in,BasicMatrix<_Type>& a){
		for(size_t i=0;i<a._elem.size();i++){
			for(size_t j=0;j<a._elem[0].size();j++){
				in>>a._elem[i][j];
				}
			}
		return in;
		}
	template <class _Type>
	std::ostream& operator<<(std::ostream& out,BasicMatrix<_Type>& a){
		for(size_t i=0;i<a._elem.size();i++){
			for(size_t j=0;j<a._elem[0].size();j++){
				out<<a._elem[i][j]<<" ";
				}
			out<<std::endl;
			}
		return out;
	}

	//класс для произвольной матицы( ступенчатая, возможность вставки и удаления строк
template <class _Type>
class FormMatrix :public  BasicMatrix<_Type>
{
public:
	FormMatrix(){
	}
	FormMatrix(size_t height){
		_elem.resize(height, std::vector<_Type>(height));
	}
	FormMatrix(size_t height, size_t width){
		_elem.resize(height, std::vector<_Type>(width));
	}
	FormMatrix(const BasicMatrix& _new){
		_elem = _new._elem;
	}
	FormMatrix(const std::vector<std::vector<_Type> >& _new)
	{
		_elem = _new;
	}
	virtual void pushLine(const std::vector<_Type>& _new)
	{
		_elem.push_back(_new);
	}
	virtual void popLine()
	{
		_elem.pop_back();
	}
	bool operator == (const BasicMatrix<_Type> eval)
	{
		return _elem == eval._elem;
	}
};


/*#ifdef TREE_BINARY_H
	template<class KeyT> class AssociativeMatrix : public BasicMatrix<KeyT>
	{
		AvlTreeBase<KeyT, std::pair<int, int>> data;
		int cur;
		int max;
	public:
		AssociativeMatrix(){}
		AssociativeMatrix(size_t height) : BasicMatrix(height)
		{
			cur = 0;
			max = height*height;
		}
		AssociativeMatrix(size_t height, size_t width) : BasicMatrix(height, width)
		{
			cur = 0;
			max = height*width;
		}
		bool insert(KeyT k)
		{
			if (data.find(k) == std::make_pair<int, int>(0, 0) && _elem[0][0] != k)
			{
				if (cur < max)
				{
					data.insert(k, std::make_pair<int, int>(cur/_elem.size(),cur % _elem[0].size()));
					_elem[cur/_elem.size()][cur % _elem[0].size()] = k;
					cur++;
					return true;
				}
				else return false;
			}
			else return false;
		}
		std::pair<int, int> find(KeyT k)
		{
			return data.find(k);
		}
	};
#endif*/
#endif