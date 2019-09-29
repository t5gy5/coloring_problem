#ifndef TOTALPAIRING_H
#define TOTALPAIRING_H


template<class Random_Access_Iterator>
class TotalPairing{
    typedef Random_Access_Iterator Iterator;
    typedef typename Iterator::difference_type Int;
    Iterator symbols;
    Iterator pairings;
    Int m_size;
    Int repeat_count;
    Int iteration_count;
    bool algorithm;

    class Circulator{
        Iterator m_begin,current,m_end;
    public:
        Circulator(Iterator begin,Iterator end,Iterator current):m_begin(begin),current(current),m_end(end){}
        void translate(Int offset){
            m_begin+=offset;
            current+=offset;
            m_end  +=offset;
        }
        Circulator& operator++(){
            if(current == m_end) {
                current = m_begin;
            }else{
                ++current;
            }
            return *this;
        }
        Circulator& operator--(){
            if(current == m_begin){
                current = m_end;
            }else{
                --current;
            }
            return *this;
        }
        typename Iterator::reference operator*(){
            return *current;
        }
    };

    typedef typename TotalPairing::Circulator circulator;

    void even_polygon_pairing(){
        Int n = m_size/2;
        circulator left      (pairings  ,pairings+n-1     ,pairings + n-1 - iteration_count);
        circulator right     (pairings+n,pairings+m_size-1,pairings + n   + iteration_count);
        circulator left_symb (symbols   ,symbols +n-1     ,symbols                         );
        circulator right_symb(symbols +n,symbols +m_size-1,symbols  + m_size-1             );
        for(Int i=0; i<= repeat_count;++i){
            for(Int j=0;j<n;++j){
                *right = *left_symb;
                *left  = *right_symb;
                ++right;
                ++left_symb;
                --left;
                --right_symb;
            }
            left.translate(m_size);
            right.translate(m_size);
            left_symb.translate(m_size);
            right_symb.translate(m_size);
        }
    }
    void odd_polygon_pairing(){
        Int n = m_size/2;
        circulator left      (pairings  ,pairings+n-1     ,pairings + iteration_count);
        circulator left_symb (symbols   ,symbols +n-1     ,symbols  + iteration_count);

        circulator right     (pairings+n,pairings+m_size-1,pairings +n + (n-1 + iteration_count)%n );
        circulator right_symb(symbols +n,symbols +m_size-1,symbols  +n + (n-1 + iteration_count)%n );

        for(Int i=0;i<= repeat_count;++i){
            *left = *right_symb;
            *right= *left_symb ;

            circulator left_write(left);
            circulator left_read(left_symb);
            circulator right_write(right);
            circulator right_read(right_symb);

            for(Int j=0;j<n-1;++j){
                ++left_write;
                --left_read;
                *left_write = *left_read;
                --right_write;
                ++right_read;
                *right_write= *right_read;
            }
            left.translate(m_size);
            right.translate(m_size);
            left_symb.translate(m_size);
            right_symb.translate(m_size);
        }
    }
    void final_pairing(){
        Iterator itr_symb = symbols ;
        Iterator itr_pair = pairings;
        for(Int i=0;i<=repeat_count;++i){
            itr_pair[0] = itr_symb[1];
            itr_pair[1] = itr_symb[0];
            itr_pair+=2;
            itr_symb+=2;
        }
    }

public:
    TotalPairing(Iterator symbols,Iterator destination,Int size):symbols(symbols),pairings(destination),m_size(size){
        if(m_size%2) throw "Number of symbols must be even!";
        repeat_count = 0;
        iteration_count = 0;
        algorithm = true;
    }
    TotalPairing(Iterator symbols_begin,Iterator symbols_end,Iterator destination):symbols(symbols_begin),pairings(destination),m_size(symbols_end-symbols_begin){
        if(m_size%2) throw "Number of symbols must be even!";
        repeat_count = 0;
        iteration_count = 0;
        algorithm = true;
    }
    bool operator()(){
        if(m_size == 0){
            return false;
        }
        if(m_size == 2){
            final_pairing();
            m_size = 0;
            return true;
        }
        if(algorithm){
            even_polygon_pairing();
            ++iteration_count;
            if(iteration_count == m_size/2-1 && m_size%4){
                algorithm = false;
                iteration_count = 0;
            }else if(iteration_count == m_size/2){
                iteration_count = 0;
                m_size/=2;
                repeat_count = 2*repeat_count+1;
            }
        }else{
            odd_polygon_pairing();
            ++iteration_count;
            if(iteration_count == m_size/2) {
                m_size = 0;
            }
        }
        return true;
    }
};

#endif // TOTALPAIRING_H
