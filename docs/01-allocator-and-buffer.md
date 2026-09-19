# 01 资源管理：DeviceAllocator 与 Buffer

> 对应课程：资源管理 | 源码：`infer/include/base/` `infer/source/base/`

## 核心设计（用自己的话回答）

<!-- 2-5 句话：为什么要把"分配内存"抽象成 DeviceAllocator？Buffer 解决什么问题？ -->

## 知识点清单

<!-- 每条用自己的话写 2-3 句；写不出来的 = 还没懂，回去问 -->

- [x] include guard 的作用
* #include起到一个文本复制的作用，同一头文件可能被直接或者间接的复制多次
* 如果复制多次之后会出现的问题是类或者枚举可能会被定义多次，编译器会报"重复定义的错误"
* 在实际的读取过程中编译器做了这样的处理，每个.cpp文件会维护一张宏表，当预处理时碰到#ifndef，在宏未定义时条件为真，后面的内容才会继续执行，宏已定义则直接跳过这部分内容，这一切都发生在编译阶段之前
- [x] namespace 的作用
避免同名的函数或者变量发生冲突
- [x] `enum class` 和普通 `enum` 的区别
两条优势：
* 作用域隔离：枚举值不裸露到外层，必须`DeviceType::kDeviceCPU`带姓访问；
  普通 enum的值是散装的，可以直接访问，直接用名字容易起冲突
* 强类型：不隐式的转为int，不同枚举之间不能进行比较
- [x] `= default` 与 `= delete`
* 这两个是在写NoCopyable这个类的时候遇到的，= default的意思是保持编译器原有的行为，= delete的意思是禁止调用这个函数，调用了之后会直接编译报错，拷贝构造函数的声明会抑制默认构造生成， =default会请回被抑制的默认构造，没有它继承链会出现编译失败的问题
- [x] 拷贝构造 vs 拷贝赋值（语法上怎么区分、编译器怎么选）
* 拷贝构造是指 Buffer b = a 这种写法，是一种初始化的语法糖。等价`Buffer b(a)`，写这句代码的时候b还没有初始化，触发拷贝构造 `Buffer(const Buffer&)`
* 拷贝赋值是指 `b = a` 这种写法，b 这个时候其实已经存在了，是给旧对象替换内容，触发拷贝赋值 `operator=(const Buffer&)`
- [x] `this` / `*this` / 引用 vs 指针
this是类内隐藏的Buffer指针，*this是解引用，拿到对象本身
- [x] `const` 的三种用法（变量 / 参数 / 成员函数）
* const 放在变量前
```cpp
const int a =  1024; //这个值出生后就不会发生改变了
```
* const 放在参数里
```cpp
const Buffer& buffer //保证不会修改调用者传递进来的对象 
```
* const 放在成员函数末尾
```cpp
size_t byte_size() const //本质：把隐藏的this从Buffer*变成const Buffer* (指向只读对象)
```
* const 引用只能调 const 函数：非 const 函数没做过只读承诺，编译器"只认徽章"，
  禁止它在只读场景被调用。所以 getter 必加 const：如实申报只读 + 换取全场景可用
* 万能口诀：**const 贴着谁，谁就只读**——任何 const 场景只问"它贴着谁"
* 星号左右规则：`const void* p`（const 在 * 左边）锁【指向的内容】，指针本身可换地址；
  `void* const p`（const 在 * 右边）锁【指针本身】。memcpy 源参数用 `const void* src_ptr`：
  源数据只读但指针可移动——每个 const 的位置都在精确声明语义
* 尾部 const 是函数签名的一部分：子类 override 必须原样带上；类外定义
  （alloc_cpu.cpp）也必须带——少写就变成"另一个函数"，链接报 undefined reference
* 判据：不改任何成员 → 必加 const。例：allocate() 申请新内存返回给调用者，
  没碰分配器自身成员，所以能加；const 管的是"这个对象"，不管"新申请的内存"
* const 锁的是"本次访问的视角"而非对象本身——同一对象可同时被只读引用和普通引用看
* mutable 逃生舱（第 17 课预告）：逻辑上不变、物理上要动的成员（缓存/计数器）
  加 mutable，const 函数里也允许改
- [x] protected 构造函数 = "只能当爹，不能单独存在"
当给构造函数加上protected之后，这个class自身是不能直接实例化的，因为protected方法不能从外界调用，子类内部是可以继承这个方法的，而子类Buffer构造时必须先构造父类的部分，这次调用是发生在"家族内部"合法。
- [ ] posix_memalign 与内存对齐（为什么 AVX 需要 32 字节对齐）
这里完全没懂

## 踩坑记录

<!-- 记录自己真实犯过的错，例如：enum 少写 `};` 导致整个文件补全全灭 -->
1. enum/class 定义结尾漏写 `};` → 整个文件解析失败，IntelliSense 补全/悬停/跳转全灭。
   教训：任何语法错误都会让智能提示整体失效，先修 Problems 面板里最上面的那个
2. 枚举成员大小写拼错：`kDeviceCpu` → 应为 `kDeviceCPU`。C++ 大小写敏感，
   后续引用对不上会报 not a member。（此坑在写笔记时又犯了一遍，证明它值得记录）
3. 同一个类名敲出三个版本：`DevideAllocator` / `DevviceAllocator` / `DeviceAllocator`——
   标识符精确匹配，三个是互不相干的名字。教训：长名字用 Ctrl+Space 补全，不手敲
4. 参数声明为 `devide_type`，初始化列表里用了 `device_type` → undeclared identifier。
   教训：声明的名字和使用的名字必须逐字符一致

## 疑问 & 答案

<!-- 当时没懂、后来弄明白的问题，写 Q&A 形式 -->
- Q:
  A:

## 相关源码

- `infer/include/base/base.h`
- `infer/include/base/alloc.h`（待实现）
- `infer/include/base/buffer.h`（待实现）
