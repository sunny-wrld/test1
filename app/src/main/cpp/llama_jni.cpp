#include <jni.h>
#include <string>
#include <vector>
#include <map>

// Nota: Estes includes dependem da biblioteca llama.cpp estar disponível
// Se você não tiver llama.cpp compilado, comente estas linhas e use stubs
#ifdef LLAMA_CPP_AVAILABLE
#include "llama.h"
#endif

// Estrutura para armazenar o contexto do modelo
struct ModelContext {
#ifdef LLAMA_CPP_AVAILABLE
    llama_context* ctx;
    llama_model* model;
    // gpt_params params; // Removido - usar parâmetros diretamente
#else
    void* ctx;
    void* model;
#endif
    std::string modelPath;
    int n_ctx;
    int n_threads;
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
#ifdef LLAMA_CPP_AVAILABLE
        ModelContext* ctx = new ModelContext();
        
        // Configura parâmetros padrão
        ctx->modelPath = std::string(path);
        ctx->n_ctx = 2048;
        ctx->n_threads = 4;
        
        // Configura parâmetros do modelo
        llama_model_params model_params = llama_model_default_params();
        model_params.n_gpu_layers = 0; // CPU apenas por padrão
        
        // Carrega o modelo
        ctx->model = llama_load_model_from_file(ctx->modelPath.c_str(), model_params);
        if (ctx->model == nullptr) {
            delete ctx;
            env->ReleaseStringUTFChars(modelPath, path);
            return 0;
        }
        
        // Configura parâmetros do contexto
        llama_context_params ctx_params = llama_context_default_params();
        ctx_params.n_ctx = ctx->n_ctx;
        ctx_params.n_threads = ctx->n_threads;
        
        // Cria contexto
        ctx->ctx = llama_new_context_with_model(ctx->model, ctx_params);
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
#else
        // Stub: retorna erro se llama.cpp não estiver disponível
        env->ReleaseStringUTFChars(modelPath, path);
        return 0;
#endif
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
    if (ctx == nullptr) {
        return env->NewStringUTF("Erro: Contexto inválido");
    }
    
#ifdef LLAMA_CPP_AVAILABLE
    if (ctx->ctx == nullptr) {
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
            if (keep > 0 && keep < (int)tokens.size()) {
                tokens = std::vector<llama_token>(tokens.end() - keep, tokens.end());
            }
        }
        
        // Cria batch para tokens iniciais
        llama_batch batch = llama_batch_init(tokens.size(), 0);
        for (size_t i = 0; i < tokens.size(); i++) {
            batch.token[i] = tokens[i];
            batch.pos[i] = i;
            batch.n_seq_id[i] = 1;
            batch.seq_id[i][0] = 0;
            batch.logits[i] = (i == tokens.size() - 1);
        }
        batch.n_tokens = tokens.size();
        
        // Decodifica tokens iniciais
        if (llama_decode(ctx->ctx, batch) != 0) {
            llama_batch_free(batch);
            env->ReleaseStringUTFChars(prompt, promptStr);
            return env->NewStringUTF("Erro ao decodificar prompt");
        }
        
        n_past = tokens.size();
        int currentTokenIndex = tokens.size() - 1;
        llama_batch_free(batch);
        
        // Gera tokens
        std::string response;
        
        for (int i = 0; i < maxTokens; i++) {
            // Obtém logits do último token processado
            float* logits = llama_get_logits_ith(ctx->ctx, currentTokenIndex);
            int n_vocab = llama_n_vocab(ctx->model);
            
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
            char token_str[32];
            int n_tokens = llama_token_to_piece(ctx->model, new_token_id, token_str, sizeof(token_str), false);
            if (n_tokens > 0) {
                response += std::string(token_str, n_tokens);
            }
            
            // Cria batch para novo token
            llama_batch batch_next = llama_batch_init(1, 0);
            batch_next.token[0] = new_token_id;
            batch_next.pos[0] = n_past;
            batch_next.n_seq_id[0] = 1;
            batch_next.seq_id[0][0] = 0;
            batch_next.logits[0] = true;
            batch_next.n_tokens = 1;
            
            // Decodifica novo token
            if (llama_decode(ctx->ctx, batch_next) != 0) {
                llama_batch_free(batch_next);
                break;
            }
            
            n_past += 1;
            currentTokenIndex = 0; // O novo token está na posição 0 do batch
            llama_batch_free(batch_next);
        }
        
        env->ReleaseStringUTFChars(prompt, promptStr);
        return env->NewStringUTF(response.c_str());
        
    } catch (...) {
        env->ReleaseStringUTFChars(prompt, promptStr);
        return env->NewStringUTF("Erro ao gerar resposta");
    }
#else
    // Stub: retorna mensagem de erro
    return env->NewStringUTF("Erro: Biblioteca llama.cpp não está disponível. Compile a biblioteca nativa primeiro.");
#endif
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
#ifdef LLAMA_CPP_AVAILABLE
        if (ctx->ctx != nullptr) {
            llama_free(ctx->ctx);
        }
        if (ctx->model != nullptr) {
            llama_free_model(ctx->model);
        }
#endif
        delete ctx;
        modelCache.erase(handle);
    }
}

} // extern "C"



