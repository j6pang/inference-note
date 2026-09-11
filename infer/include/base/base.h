#ifndef INFER_INCLUDE_BASE_BASE_H_
#define INFER_INCLUDE_BASE_BASE_H_

#include <cstddef>
#include <cstdint>

namespace base {

// TODO: DeviceType / DataType / DataTypeSize / NoCopyable
// 参考: KuiperLLama/kuiper/include/base/base.h

enum class DeviceType: uint8_t {
    kDeviceUnknown = 0,
    kDeviceCPU = 1,
    kDeviceCUDA = 2,
};

enum class DataType: uint8_t {
    kDataTypeUnknown = 0,
    kDataTypeFp32 = 1,
    kDataTypeInt8 = 2,
    kDataTypeInt32 = 3,
};

inline size_t DataTypeSize(DataType data_type) {
    if (data_type == DataType::kDataTypeFp32) {
        return sizeof(float);
    } else if (data_type == DataType::kDataTypeInt8) {
        return sizeof(int8_t);
    } else if (data_type == DataType::kDataTypeInt32) {
        return sizeof(int32_t);
    } else {
        return 0;
    }
}

/**
 * @brief 不可拷贝基类：继承它的类自动获得"禁止拷贝"属性
 *
 * 背景：拥有资源（裸指针）的对象一旦被复制，析构时会对同一块内存
 * 释放两次。解法是在基类中 = delete 拷贝构造与拷贝赋值，编译期拦截。
 *
 * 构造/析构为 protected：本类不可单独实例化，只能作为基类被继承。
 */
class NoCopyable {
 protected:
    // = default 显式请回默认构造：下面的拷贝构造声明会抑制编译器的自动生成，
    // 没有这行，子类构造时找不到可用的父类构造，整条继承链编译失败
    NoCopyable() = default;  // protected：不能单独实例化，只能被继承

    ~NoCopyable() = default;

    NoCopyable(const NoCopyable&) = delete;  // 删除拷贝构造：拦截 Buffer b = a;（初始化场景）

    NoCopyable& operator=(const NoCopyable&) = delete;  // 删除拷贝赋值：拦截 b = a;（已存在对象赋值场景）
};



}  // namespace base

#endif  // INFER_INCLUDE_BASE_BASE_H_
