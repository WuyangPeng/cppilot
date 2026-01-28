#include "llama.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    // 1. 系统初始化
    llama_backend_init();

    // 2. 加载模型参数
    llama_model_params model_params = llama_model_default_params();
    llama_model* model = llama_model_load_from_file("F:/cppilot/models/qwen2.5-coder-7b-instruct-q6_k.gguf", model_params);

    if (!model) {
        std::cerr << "加载模型失败！" << std::endl;
        return 1;
    }

    // 3. 创建上下文
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048; // 上下文窗口大小
    llama_context* ctx = llama_init_from_model(model, ctx_params);

    // 4. 用户提问
    std::string user_prompt = "Hello, tell me a joke in one line.";
    std::vector<llama_token> tokens_list;
    tokens_list.resize(user_prompt.size() + 4);
    int n_tokens = llama_tokenize(llama_model_get_vocab(model), user_prompt.c_str(), user_prompt.size(), tokens_list.data(), tokens_list.size(), true, true);
    tokens_list.resize(n_tokens);

    // 5. 模型处理输入 (手动填充 llama_batch)
    llama_batch batch = llama_batch_init(512, 0, 1); // 分配足够大的批处理空间

    // 手动填充批处理
    batch.n_tokens = n_tokens;
    for (int32_t i = 0; i < batch.n_tokens; i++) {
        batch.token[i]    = tokens_list[i];
        batch.pos[i]      = i;
        batch.n_seq_id[i] = 1;
        batch.seq_id[i][0] = 0;
        batch.logits[i]   = false;
    }
    // 我们需要最后一个 token 的 logits 来进行采样
    batch.logits[batch.n_tokens - 1] = true;

    if (llama_decode(ctx, batch) != 0) {
        std::cerr << "llama_decode failed on prompt" << std::endl;
        return 1;
    }

    std::cout << "\nNexus Thinking...\n" << std::endl;
    std::cout << "Nexus: ";

    // 6. 生成回复
    int n_cur = batch.n_tokens;
    llama_token new_token_id = 0;
    const int n_len = 200; // 最大生成长度

    for (int i = 0; i < n_len; ++i) {
        // 从 logits 中采样下一个 token
        auto logits = llama_get_logits_ith(ctx, batch.n_tokens - 1);
        new_token_id = llama_sample_token_greedy(ctx, logits);

        // 如果是 EOS (end-of-sequence) token，则停止生成
        if (new_token_id == llama_token_eos(model)) {
            break;
        }

        // 将新生成的 token 打印出来
        std::string token_text = llama_token_to_piece(ctx, new_token_id);
        std::cout << token_text << std::flush;

        // 为下一次迭代准备批处理
        batch.n_tokens = 1;
        batch.token[0]    = new_token_id;
        batch.pos[0]      = n_cur;
        batch.n_seq_id[0] = 1;
        batch.seq_id[0][0] = 0;
        batch.logits[0]   = true;

        // 解码新的 token 以预测再下一个
        if (llama_decode(ctx, batch) != 0) {
            std::cerr << "llama_decode failed during generation" << std::endl;
            return 1;
        }
        n_cur++;
    }

    std::cout << std::endl;

    // 清理资源
    llama_batch_free(batch);
    llama_free(ctx);
    llama_model_free(model);
    llama_backend_free();

    return 0;
}
