
#ifndef _LISTMACRO_H_
#define _LISTMACRO_H_

#define delete_list_items() delete [] items_

extern void ListImpl_range_error(long index);
extern long ListImpl_best_new_count(long count, unsigned size);


#define declarePList(List,T)\
\
  typedef T *T##_pointer;\
\
  declareList(List,T##_pointer);

#define implementPList(List,T)\
\
  typedef T *T##_pointer;\
\
  implementList(List,T##_pointer);


#define declareList(List,T)\
class List { \
public: \
    List(long size = 0); \
    List(const List &); \
    ~List(); \
\
    long count() const; \
    T& item(long index) const; \
    T* array(long index, long count); \
\
    void prepend(const T&); \
    void append(const T&); \
    void insert(long index,const T&); \
    void remove(long index); \
    void remove_all(); \
    List &operator=(const List &); \
private: \
    T* items_; \
    long size_; \
    long count_; \
    long free_; \
}; \
\
inline long List::count() const { return count_; } \
\
inline T& List::item(long index) const { \
    if (index < 0 || index >= count_) { \
	ListImpl_range_error(index); \
    } \
    long i = index < free_ ? index : index + size_ - count_; \
    return items_[i]; \
} \
\
inline void List::append(const T& item) { insert(count_, item); } \
inline void List::prepend(const T& item) { insert(0, item); }


#define implementList(List,T) \
List::List(long size) { \
    if (size > 0) { \
        size_ = ListImpl_best_new_count(size, sizeof(T)); \
        items_ = new T[size_]; \
    } else { \
        size_ = 0; \
        items_ = 0; \
    } \
    count_ = 0; \
    free_ = 0; \
} \
\
List::List(const List& list) : items_(0), size_(0), count_(0), free_(0) { \
    for (long i = 0; i < list.count(); ++i) { \
	append(list.item(i)); \
    } \
} \
\
List::~List() { \
    delete_list_items(); \
} \
\
T* List::array(long index, long count) { \
    if (index + count <= free_) { \
        return items_ + index; \
    } else if (index >= free_) { \
        return items_ + index + size_ - count_; \
    } else { \
        long i; \
        for (i = 0; i < index + count - free_; ++i) { \
            items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
        } \
        free_ = index + count; \
    } \
    return items_ + index; \
} \
\
void List::insert(long index, const T& item) { \
    if (count_ == size_) { \
        long size = ListImpl_best_new_count(size_ + 1, sizeof(T)); \
        T* items = new T[size]; \
        if (items_ != 0) { \
            register long i; \
            for (i = 0; i < free_; ++i) { \
                items[i] = items_[i]; \
            } \
            for (i = 0; i < count_ - free_; ++i) { \
                items[free_ + size - count_ + i] = \
                    items_[free_ + size_ - count_ + i]; \
            } \
            delete_list_items(); \
        } \
        items_ = items; \
        size_ = size; \
    } \
    if (index >= 0 && index <= count_) { \
	if (index < free_) { \
            for (register long i = free_ - index - 1; i >= 0; --i) { \
                items_[index + size_ - count_ + i] = items_[index + i]; \
            } \
        } else if (index > free_) { \
            for (register long i = 0; i < index - free_; ++i) { \
                items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
            } \
        } \
        free_ = index + 1; \
        count_ += 1; \
        items_[index] = (T)item; \
    } \
} \
\
void List::remove(long index) { \
    if (index >= 0 && index <= count_) { \
        if (index < free_) { \
            for (register long i = free_ - index - 2; i >= 0; --i) { \
                items_[size_ - count_ + index + 1 + i] = \
		    items_[index + 1 + i]; \
            } \
        } else if (index > free_) { \
            for (register long i = 0; i < index - free_; ++i) { \
                items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
            } \
        } \
        free_ = index; \
        count_ -= 1; \
    } \
} \
\
void List::remove_all() { \
    count_ = 0; \
    free_ = 0; \
} \
\
List & List::operator=(const List& list) { \
    if (this != &list) { \
	remove_all(); \
	for (long i = 0; i < list.count(); ++i) { \
	    append(list.item(i)); \
	} \
    } \
    return *this; \
}


#endif

