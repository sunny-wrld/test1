package com.auroraedge.app.model

/**
 * Modelo de dados para uma mensagem do chat
 * 
 * @param text Texto da mensagem
 * @param isUser true se a mensagem é do usuário, false se é da IA
 * @param timestamp Timestamp da mensagem em milissegundos
 * @param isTyping true se é uma mensagem de "digitando..."
 * @param isSystem true se é uma mensagem do sistema
 */
data class ChatMessage(
    val text: String,
    val isUser: Boolean = false,
    val timestamp: Long = System.currentTimeMillis(),
    val isTyping: Boolean = false,
    val isSystem: Boolean = false
)



