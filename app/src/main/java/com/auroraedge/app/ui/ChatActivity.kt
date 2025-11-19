package com.auroraedge.app.ui

import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import com.auroraedge.app.R
import com.auroraedge.app.databinding.ActivityChatBinding
import com.auroraedge.app.model.ChatMessage
import com.auroraedge.app.adapter.ChatAdapter
import com.auroraedge.app.ai.AIModelManager
import kotlinx.coroutines.launch
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

/**
 * Activity principal do chat do Aurora Edge
 * 
 * Esta tela permite ao usuário interagir com a IA offline através de mensagens de texto.
 * Todas as operações são realizadas localmente, sem conexão com a internet.
 */
class ChatActivity : AppCompatActivity() {
    
    private lateinit var binding: ActivityChatBinding
    private lateinit var chatAdapter: ChatAdapter
    private lateinit var aiModelManager: AIModelManager
    
    private val chatMessages = mutableListOf<ChatMessage>()
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityChatBinding.inflate(layoutInflater)
        setContentView(binding.root)
        
        setupRecyclerView()
        setupClickListeners()
        initializeAIModel()
    }
    
    /**
     * Configura o RecyclerView para exibir as mensagens do chat
     */
    private fun setupRecyclerView() {
        chatAdapter = ChatAdapter(chatMessages)
        binding.recyclerViewChat.apply {
            layoutManager = LinearLayoutManager(this@ChatActivity)
            adapter = chatAdapter
        }
    }
    
    /**
     * Configura os listeners dos botões
     */
    private fun setupClickListeners() {
        // Botão de enviar mensagem
        binding.buttonSend.setOnClickListener {
            sendMessage()
        }
        
        // Botão de resumir texto
        binding.buttonSummarize.setOnClickListener {
            showSummarizeDialog()
        }
        
        // Enviar ao pressionar Enter no campo de texto
        binding.editTextMessage.setOnEditorActionListener { _, _, _ ->
            sendMessage()
            true
        }
    }
    
    /**
     * Inicializa o modelo de IA
     * Carrega o modelo quantizado do armazenamento interno do app
     */
    private fun initializeAIModel() {
        binding.progressBarLoading.visibility = View.VISIBLE
        binding.textViewStatus.text = getString(R.string.loading_model)
        
        lifecycleScope.launch {
            try {
                aiModelManager = AIModelManager(this@ChatActivity)
                val success = withContext(Dispatchers.IO) {
                    aiModelManager.loadModel()
                }
                
                withContext(Dispatchers.Main) {
                    binding.progressBarLoading.visibility = View.GONE
                    if (success) {
                        binding.textViewStatus.text = getString(R.string.model_ready)
                        binding.buttonSend.isEnabled = true
                        binding.buttonSummarize.isEnabled = true
                        addSystemMessage(getString(R.string.welcome_message))
                    } else {
                        binding.textViewStatus.text = getString(R.string.model_error)
                        Toast.makeText(
                            this@ChatActivity,
                            getString(R.string.model_load_failed),
                            Toast.LENGTH_LONG
                        ).show()
                    }
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    binding.progressBarLoading.visibility = View.GONE
                    binding.textViewStatus.text = getString(R.string.model_error)
                    Toast.makeText(
                        this@ChatActivity,
                        "Erro: ${e.message}",
                        Toast.LENGTH_LONG
                    ).show()
                }
            }
        }
    }
    
    /**
     * Envia uma mensagem do usuário e obtém resposta da IA
     */
    private fun sendMessage() {
        val messageText = binding.editTextMessage.text.toString().trim()
        
        if (messageText.isEmpty()) {
            return
        }
        
        if (!::aiModelManager.isInitialized || !aiModelManager.isModelLoaded()) {
            Toast.makeText(this, getString(R.string.model_not_ready), Toast.LENGTH_SHORT).show()
            return
        }
        
        // Adiciona mensagem do usuário ao chat
        val userMessage = ChatMessage(
            text = messageText,
            isUser = true,
            timestamp = System.currentTimeMillis()
        )
        chatMessages.add(userMessage)
        chatAdapter.notifyItemInserted(chatMessages.size - 1)
        binding.recyclerViewChat.smoothScrollToPosition(chatMessages.size - 1)
        
        // Limpa o campo de texto
        binding.editTextMessage.text?.clear()
        
        // Mostra indicador de digitação
        val typingMessage = ChatMessage(
            text = getString(R.string.thinking),
            isUser = false,
            timestamp = System.currentTimeMillis(),
            isTyping = true
        )
        chatMessages.add(typingMessage)
        chatAdapter.notifyItemInserted(chatMessages.size - 1)
        binding.recyclerViewChat.smoothScrollToPosition(chatMessages.size - 1)
        
        // Obtém resposta da IA em background
        lifecycleScope.launch {
            try {
                val response = withContext(Dispatchers.IO) {
                    aiModelManager.generateResponse(messageText)
                }
                
                withContext(Dispatchers.Main) {
                    // Remove mensagem de "digitando"
                    chatMessages.removeAt(chatMessages.size - 1)
                    chatAdapter.notifyItemRemoved(chatMessages.size)
                    
                    // Adiciona resposta da IA
                    val aiMessage = ChatMessage(
                        text = response,
                        isUser = false,
                        timestamp = System.currentTimeMillis()
                    )
                    chatMessages.add(aiMessage)
                    chatAdapter.notifyItemInserted(chatMessages.size - 1)
                    binding.recyclerViewChat.smoothScrollToPosition(chatMessages.size - 1)
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    // Remove mensagem de "digitando"
                    chatMessages.removeAt(chatMessages.size - 1)
                    chatAdapter.notifyItemRemoved(chatMessages.size)
                    
                    // Adiciona mensagem de erro
                    val errorMessage = ChatMessage(
                        text = getString(R.string.error_response, e.message),
                        isUser = false,
                        timestamp = System.currentTimeMillis()
                    )
                    chatMessages.add(errorMessage)
                    chatAdapter.notifyItemInserted(chatMessages.size - 1)
                }
            }
        }
    }
    
    /**
     * Mostra diálogo para resumir texto
     */
    private fun showSummarizeDialog() {
        // TODO: Implementar diálogo para inserir texto a ser resumido
        Toast.makeText(this, "Funcionalidade de resumo em desenvolvimento", Toast.LENGTH_SHORT).show()
    }
    
    /**
     * Adiciona mensagem do sistema ao chat
     */
    private fun addSystemMessage(text: String) {
        val systemMessage = ChatMessage(
            text = text,
            isUser = false,
            timestamp = System.currentTimeMillis(),
            isSystem = true
        )
        chatMessages.add(systemMessage)
        chatAdapter.notifyItemInserted(chatMessages.size - 1)
    }
    
    override fun onDestroy() {
        super.onDestroy()
        if (::aiModelManager.isInitialized) {
            aiModelManager.release()
        }
    }
}




