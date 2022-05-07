#ifndef __RCSTRING_H
#define __RCSTRING_H
class RCString{//ReferenceCount String
    public:

    private:
        struct StringValue{//用stuct把对象和计数绑定在一起。
            int refCount;//引用计数
            char *data;//实际string
            StringValue(const char *initValue);
            ~StringValue();
        };
        StringValue *value;//实际数据成员
};

#endif