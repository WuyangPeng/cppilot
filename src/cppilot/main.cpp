#include "llama.h"
#include <iostream>
#include <vector>

int test()
{
    // 1. 系统初始化
    llama_backend_init();

    // 2. 加载模型参数
    llama_model_params model_params = llama_model_default_params();
    llama_model* model = llama_model_load_from_file("F:/cppilot/models/qwen2.5-coder-7b-instruct-q6_k.gguf", model_params);

    if (!model)
    {
        std::cerr << "加载模型失败！" << std::endl;
        return 1;
    }

    // 3. 创建上下文
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048; // 上下文窗口大小
    llama_context* ctx = llama_init_from_model(model, ctx_params);

    // 4. 用户提问
    std::string user_prompt = "Hello, tell me a joke in one line.";
    std::vector<llama_token> tokens(user_prompt.size() + 4);
    int n_tokens = llama_tokenize(llama_model_get_vocab(model), user_prompt.c_str(), user_prompt.size(), tokens.data(), tokens.size(), true, true);
    tokens.resize(n_tokens);

    std::cout << "\nNexus Thinking...\n" << std::endl;

    // 5. 简单推理（此处仅展示逻辑，实际需要循环推理 tokens）
    // 注意：完整推理需要处理 KV Cache 和 Sampling，为了演示先确认加载成功
    std::cout << "大脑已准备就绪。Token 数量: " << n_tokens << std::endl;

    // 清理
    llama_free(ctx);
    llama_model_free(model);
    llama_backend_free();

    return 0;
}