#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using Int = std::size_t;

class Circulator{
    Int *m_begin,*m_end,*m_current;
public:
    Circulator(Int* begin,Int* end,Int* current):m_begin(begin),m_end(end),m_current(current){}
    Circulator(Int* begin,Int* end):m_begin(begin),m_end(end),m_current(begin){}
    Circulator& operator+=(int offset){
        m_begin  +=offset;
        m_end    +=offset;
        m_current+=offset;
        return *this;
    }
    Int& operator*(){
        return *m_current;
    }
    Circulator& operator++(){
        if(m_current==m_end){
            m_current = m_begin;
        }else{
            ++m_current;
        }
        return *this;
    }
    Circulator operator++(int){
        Circulator temp(*this);
        ++(*this);
        return temp;
    }
    Circulator& operator--(){
        if(m_current==m_begin){
            m_current = m_end;
        }else{
            --m_current;
        }
        return *this;
    }
    Circulator operator--(int){
        Circulator temp(*this);
        --(*this);
        return temp;
    }
    Int range_size(){
        return 1+(m_end-m_begin);
    }
    operator Int*(){
        return m_current;
    }
    Int* lower_bound(){
        return m_begin;
    }
    Int*& at_iterator(){
        return m_current;
    }
};

class SquareTable{
    Int *m_array,*m_end,c_row;
public:
    explicit SquareTable(Int row):m_array(nullptr),m_end(nullptr),c_row(row){
        row*=row;
        m_array = new Int[row]();
        m_end = m_array + row;
    }
    SquareTable(const SquareTable&) = delete;
    SquareTable(SquareTable&&) = default;
    SquareTable& operator=(const SquareTable&) = delete;
    SquareTable& operator=(SquareTable&&) = default;
    ~SquareTable(){delete[] m_array;}
    Int* operator[](Int index){
        return m_array + c_row * index;
    }
    Int* begin(){
        return m_array;
    }
    Int* end(){
        return m_end;
    }
    Int row_count(){
        return c_row;
    }
    friend std::ostream& operator<<(std::ostream& out,const SquareTable& table){
        Int size = table.c_row;
        Int fill_count = 0;
        while(size) ++fill_count,size/=10;
        Int* itr = table.m_array;
        for(Int i=0;i<table.c_row;++i){
            for(Int j=0;j<table.c_row;++j){
                out<<std::setw(fill_count)<<std::setfill(' ')<< *itr<<' ';++itr;
            }
            out<<'\n';
        }
        return out;
    }
};
//don't touch it!
void fill_table(Int* symbols,Int* pairings,const Int size,Int square_size){
    if(size==2){
        pairings[0] = symbols[1];
        pairings[1] = symbols[0];
    }else{
        Int n = size/2;
        bool flag = (n%2==0);
        Circulator left(pairings,pairings+n-1,pairings);
        Circulator right(pairings+n,pairings+size-1,pairings+size-1);
        Circulator left_symb(symbols,symbols+n-1,symbols);
        Circulator right_symb(symbols+n,symbols+size-1,symbols+size-1);

        //Even conversation of groups
        for(Int i=0;i<n-1+flag;++i){
            --left;
            ++right;
            for(Int j=0;j<n;++j){
                *right = *left_symb;
                *left  = *right_symb;
                ++right;
                ++left_symb;
                --left;
                --right_symb;
            }
            left += square_size;
            right+= square_size;
        }

        if(flag){
            fill_table(left_symb.lower_bound() ,left.lower_bound() ,n,square_size);
            fill_table(right_symb.lower_bound(),right.lower_bound(),n,square_size);
        }else{
            //Odd conversation of groups
            --left;
            ++right;
            for(Int i=0;i<n;++i){
                *left = *right_symb;
                *right= *left_symb ;

                Circulator left_write(left);
                Circulator left_read(left_symb);
                Circulator right_write(right);
                Circulator right_read(right_symb);

                for(Int j=0;j<n-1;++j){
                    ++left_write;
                    --left_read;
                    *left_write = *left_read;
                    --right_write;
                    ++right_read;
                    *right_write= *right_read;
                }
                left +=square_size; ++right_symb; ++left;
                right+=square_size; ++left_symb ; ++right;
            }
        }
    }
}

int main(){
    Int n=0;
    std::cout<<"Table size[2*n]: n = ";
    if(!(std::cin>>n) || n==0){
        std::cout<<"That's not right..."<<std::endl;
        return 1;
    }
    n<<=1;
    SquareTable table(n);
    for(Int i=0;i<n;++i){
        table[0][i]=i+1;
    }
    fill_table(table.begin(),table[1],n,n);
    std::ostringstream sstream;
    sstream <<"n = " << n/2 << ".txt";
    std::ofstream save_file(sstream.str().c_str());
    save_file<<table<<std::endl;
    std::cout<<"Filled table saved to file: \""<<sstream.str()<<"\".\n";
    if(n<28){
        std::cout<<"\nThe table: \n"<<table<<std::endl;
    }else{
        std::cout<<"Console might be not big enough to print it there.\n";
    }
    return 0;
}
