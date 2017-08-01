/*
 * Factory.h 简单工厂类
 *
 *  Created on: 2016年7月23日
 *  	Author: jcz
 */

#pragma once

#include <map>

template <typename Product, typename AbstractProduct>
AbstractProduct* FactoryCreator()
{
    return new Product();
}

template <typename AbstractProduct>
class Factory
{
public:
    Factory(){}
    virtual ~Factory(){}

public:
    AbstractProduct* Create(int productType) const
    {
        typename CreatorFuncMap::const_iterator iter = creatorFuncMap.find(productType);
        if (iter == creatorFuncMap.end())
        {
            return nullptr;
        }

        return (iter->second)();
    }

protected:
    template <typename Product>
    void RegisterProduct(int productType)
    {
        creatorFuncMap[productType] = FactoryCreator<Product, AbstractProduct>;
    }

private:
    typedef AbstractProduct* (*CreatorFunc)();
    typedef std::map<int, CreatorFunc> CreatorFuncMap;

    CreatorFuncMap creatorFuncMap;     // 对象创建函数
};


