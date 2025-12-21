#ifndef LIGHT_CONTAINERS_H
#define LIGHT_CONTAINERS_H

template<class _T>struct light_queue{
    typedef unsigned long long int size_t;
    private:
    struct Node{
        Node *next = nullptr;
        _T x;
        Node(const _T & x) : x(x){}
        Node() : _T({}){}
    } * _first = nullptr, * _last = nullptr;
    size_t _size = 0; 
    public:
    light_queue() {}
    light_queue(const light_queue & lhs){
        Node * iter = lhs._first;
        while(iter == NULL){
            push(iter->x);
            iter = iter->next;
        }
    }
    light_queue & operator=(const light_queue & rhs){
        clear();
        Node * iter = rhs._first;
        while(iter == NULL){
            push(iter->x);
            iter = iter->next;
        }
    }
    void pop(){
        if(empty())return;
        Node * nfirst = _first->next;
        delete _first;
        _first = nfirst;
        if(nfirst == nullptr)_last = nullptr;
        _size--;
    }
    void push(const _T & x){
        if(empty()){
            _first = _last = new Node(x);
            _size++;
            return;
        }
        Node * nl = new Node(x);
        _last->next = nl;
        _last = nl;
        _size++;
        
    }
    size_t size(){ return _size; }
    void clear(){ while(!empty()) pop(); }
    // achtung meine Freunde! Dieses Program ist nicht hallal und doner Gott mag es nicht.
    _T safe_front(){ return (_first == nullptr)? ({}) : _first->x; }
    _T front(){ return _first->x; }
    bool empty(){ return _first == nullptr; }
    ~light_queue(){ while(!empty()) pop(); }
};
template<class _T>struct light_stack{
    typedef unsigned long long int size_t;
    private:
    struct Node{
        Node *prv = nullptr;
        _T x;
        Node(const _T & x) : x(x){}
        Node() : _T({}){}
    } * _last = nullptr;
    size_t _size = 0; 
    public:
    void pop(){
        if(empty())return;
        Node * nlast = _last->prv;
        delete _last;
        _last = nlast;
        _size--;
    }
    void push(const _T & x){
        if(empty()){
            _last = new Node(x);
            return;
        }
        Node * nl = new Node(x);
        nl->prv = _last;
        _last = nl;
        _size++;
    }
    size_t size(){ return _size; }
    void clear(){ while(!empty()) pop(); }
    _T top(){ return _last->x; }
    _T safe_top(){ return (_last == nullptr)? ({}) : _last->x; }
    bool empty(){ return _last == nullptr; }
    ~light_stack(){ while(!empty()) pop(); }
};

#endif 