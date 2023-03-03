```cpp
void ComQueue::sendComBuffer(Fw::ComBuffer& comBuffer) {
    FW_ASSERT(this->m_state == READY);
    this->comQueueSend_out(0, comBuffer, 0);
    this->m_state = WAITING;
}

void ComQueue::sendBuffer(Fw::Buffer& buffer) {
    // Retry buffer expected to be cleared as we are either transferring ownership or have already deallocated it.
    FW_ASSERT(this->m_state == READY);
    this->buffQueueSend_out(0, buffer);
    this->m_state = WAITING;
}
```
