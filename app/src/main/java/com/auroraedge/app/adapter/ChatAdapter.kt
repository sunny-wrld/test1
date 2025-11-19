package com.auroraedge.app.adapter

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.auroraedge.app.R
import com.auroraedge.app.model.ChatMessage

/**
 * Adapter para exibir mensagens do chat no RecyclerView
 */
class ChatAdapter(private val messages: List<ChatMessage>) :
    RecyclerView.Adapter<ChatAdapter.MessageViewHolder>() {
    
    companion object {
        private const val VIEW_TYPE_USER = 1
        private const val VIEW_TYPE_AI = 2
        private const val VIEW_TYPE_SYSTEM = 3
    }
    
    override fun getItemViewType(position: Int): Int {
        return when {
            messages[position].isSystem -> VIEW_TYPE_SYSTEM
            messages[position].isUser -> VIEW_TYPE_USER
            else -> VIEW_TYPE_AI
        }
    }
    
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MessageViewHolder {
        val layoutId = when (viewType) {
            VIEW_TYPE_USER -> R.layout.item_message_user
            VIEW_TYPE_SYSTEM -> R.layout.item_message_system
            else -> R.layout.item_message_ai
        }
        
        val view = LayoutInflater.from(parent.context)
            .inflate(layoutId, parent, false)
        
        return MessageViewHolder(view)
    }
    
    override fun onBindViewHolder(holder: MessageViewHolder, position: Int) {
        holder.bind(messages[position])
    }
    
    override fun getItemCount(): Int = messages.size
    
    class MessageViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val textMessage: TextView = itemView.findViewById(R.id.textMessage)
        
        fun bind(message: ChatMessage) {
            textMessage.text = message.text
            
            // Se for mensagem de "digitando", adiciona animação
            if (message.isTyping) {
                textMessage.alpha = 0.5f
            } else {
                textMessage.alpha = 1.0f
            }
        }
    }
}




