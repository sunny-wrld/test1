#include <jni.h>
#include <string>
#include <vector>
#include "llama.h"

// Estrutura para armazenar o contexto do modelo
struct ModelContext {
    llama_context* ctx;
    llama_model* model;
    gpt_params params;
};

// Cache de contextos de modelo (um por thread/instância)
static std::map<long, ModelContext*> modelCache;

extern "C" {

/**
 * Carrega um modelo GGUF
 * @param env Ambiente JNI
 * @param thiz Referência do objeto Java
 * @param modelPath Caminho do arquivo GGUF
 * @return Handle do modelo (ponteiro como long)
 */
JNIEXPORT jlong JNICALL
Java_com_auroraedge_app_ai_AIModelManager_loadModelNative(JNIEnv *env, jobject thiz, jstring modelPath) {
    const char *path = env->GetStringUTFChars(modelPath, nullptr);
    
    if (path == nullptr) {
        return 0;
    }
    
    try {
        ModelContext* ctx = new ModelContext();
        
        // Configura parâmetros padrão
        ctx->params.model = std::string(path);
        ctx->params.n_ctx = 2048;
        ctx->params.n_threads = 4;
        ctx->params.n_gpu_layers = 0; // CPU apenas por padrão
        
        // Carrega o modelo
        ctx->model = llama_load_model_from_file(ctx->params.model.c_str(), ctx->params.model_params);
        if (ctx->model == nullptr) {
            delete ctx;
            env->ReleaseStringUTFChars(modelPath, path);
            return 0;
        }
        
        // Cria contexto
        ctx->ctx = llama_new_context_with_model(ctx->model, ctx->params.ctx_params);
        if (ctx->ctx == nullptr) {
            llama_free_model(ctx->model);
            delete ctx;
            env->ReleaseStringUTFChars(modelPath, path);
            return 0;
        }
        
        // Armazena no cache usando o endereço como handle
        long handle = reinterpret_cast<long>(ctx);
        modelCache[handle] = ctx;
        
        env->ReleaseStringUTFChars(modelPath, path);
        return handle;
        
    } catch (...) {
        env->ReleaseStringUTFChars(modelPath, path);
        return 0;
    }
}

/**
 * Gera resposta usando o modelo carregado
 */
JNIEXPORT jstring JNICALL
Java_com_auroraedge_app_ai_AIModelManager_generateResponseNative(
    JNIEnv *env,
    jobject thiz,
    jlong handle,
    jstring prompt,
    jint maxTokens,
    jfloat temperature,
    jfloat topP
) {
    if (handle == 0) {
        return env->NewStringUTF("Erro: Modelo não carregado");
    }
    
    ModelContext* ctx = modelCache[handle];
    if (ctx == nullptr || ctx->ctx == nullptr) {
        return env->NewStringUTF("Erro: Contexto inválido");
    }
    
    const char *promptStr = env->GetStringUTFChars(prompt, nullptr);
    if (promptStr == nullptr) {
        return env->NewStringUTF("Erro: Prompt inválido");
    }
    
    try {
        // Tokeniza o prompt
        std::vector<llama_token> tokens = llama_tokenize(ctx->ctx, promptStr, true);
        
        // Avalia tokens iniciais
        int n_past = 0;
        int n_ctx = llama_n_ctx(ctx->ctx);
        
        if (tokens.size() + maxTokens > n_ctx) {
            // Trunca se necessário
            int keep = n_ctx - maxTokens - 1;
            tokens = std::vector<llama_token>(tokens.end() - keep, tokens.end());
        }
        
        llama_decode(ctx->ctx, llama_batch_get_one(tokens.data(), tokens.size(), n_past, 0));
        n_past += tokens.size();
        
        // Gera tokens
        std::string response;
        int n_cur = tokens.size();
        
        for (int i = 0; i < maxTokens; i++) {
            // Obtém logits
            auto logits = llama_get_logits_ith(ctx->ctx, n_cur - 1);
            auto n_vocab = llama_n_vocab(ctx->model);
            
            // Aplica temperatura e top-p
            std::vector<llama_token_data> candidates;
            candidates.reserve(n_vocab);
            
            for (llama_token token_id = 0; token_id < n_vocab; token_id++) {
                candidates.emplace_back(llama_token_data{token_id, logits[token_id], 0.0f});
            }
            
            llama_token_data_array candidates_p = {candidates.data(), candidates.size(), false};
            
            // Sampling
            llama_sample_top_p(ctx->ctx, &candidates_p, topP, 1);
            llama_sample_temp(ctx->ctx, &candidates_p, temperature);
            
            llama_token new_token_id = llama_sample_token(ctx->ctx, &candidates_p);
            
            // Verifica fim de sequência
            if (new_token_id == llama_token_eos(ctx->model)) {
                break;
            }
            
            // Decodifica token para string
            std::string token_str = llama_token_to_piece(ctx->ctx, new_token_id);
            response += token_str;
            
            // Avalia novo token
            llama_decode(ctx->ctx, llama_batch_get_one(&new_token_id, 1, n_past, 0));
            n_past += 1;
            n_cur += 1;
        }
        
        env->ReleaseStringUTFChars(prompt, promptStr);
        return env->NewStringUTF(response.c_str());
        
    } catch (...) {
        env->ReleaseStringUTFChars(prompt, promptStr);
        return env->NewStringUTF("Erro ao gerar resposta");
    }
}

/**
 * Libera recursos do modelo
 */
JNIEXPORT void JNICALL
Java_com_auroraedge_app_ai_AIModelManager_releaseModelNative(JNIEnv *env, jobject thiz, jlong handle) {
    if (handle == 0) {
        return;
    }
    
    ModelContext* ctx = modelCache[handle];
    if (ctx != nullptr) {
        if (ctx->ctx != nullptr) {
            llama_free(ctx->ctx);
        }
        if (ctx->model != nullptr) {
            llama_free_model(ctx->model);
        }
        delete ctx;
        modelCache.erase(handle);
    }
}

} // extern "C"


