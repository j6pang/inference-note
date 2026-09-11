# inference-note

从零开始用 C++ 实现一个 LLM 推理框架的学习笔记仓库，参考 [KuiperLLama](https://github.com/suchiming/KuiperLLama) 的架构，逐个算子手工实现。

## 构建

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## 路线图

- [ ] 01 tensor：Tensor 类 + CPU Buffer/Allocator
- [ ] 02 layer：Layer 基类与 forward 接口
- [ ] 03 embedding：查表实现
- [ ] 04 rmsnorm：手写数值计算，对照 PyTorch 验证
- [ ] 05 matmul-armadillo：先用 Armadillo 建立正确性基线
- [ ] 06 matmul-native：手写行主序循环替换 Armadillo
- [ ] 07 quantize：INT8 权重打包/解包 + 量化 matmul
- [ ] 08 matmul-avx2：SIMD 向量化
- [ ] 09 rope：旋转位置编码
- [ ] 10 mha：多头注意力
- [ ] 11 swiglu：FFN
- [ ] 12 llama3：Transformer block 组装 + 权重加载
- [ ] 13 tokenizer：SentencePiece 接入
- [ ] 14 sampler-argmax：greedy 生成循环
- [ ] 15 sampler-topk-topp：top-k / top-p / temperature
- [ ] 16 kv-cache：推理提速
- [ ] 17 cuda：CUDA 算子
- [ ] 18 qwen3：safetensors 权重加载

## 目录规划

```
infer/
  include/{base,op,tensor,model,sampler}
  source/{base,op,tensor,model,sampler}
test/
demo/
```
