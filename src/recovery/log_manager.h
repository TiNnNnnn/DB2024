/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <mutex>
#include <vector>
#include <iostream>
#include "log_defs.h"
#include "common/config.h"
#include "record/rm_defs.h"

/* 日志记录对应操作的类型 */
enum LogType: int {
    UPDATE = 0,
    INSERT,
    DELETE,
    BEGIN,
    COMMIT,
    ABORT,
    CHECKPOINT,
    HEADER,
};


static std::string LogTypeStr[] = {
    "UPDATE",
    "INSERT",
    "DELETE",
    "BEGIN",
    "COMMIT",
    "ABORT",
    "CHECKPOINT"
    "HEADER"
};

class LogRecord {
public:
    LogType log_type_;         /* 日志对应操作的类型 */
    lsn_t lsn_;                /* 当前日志的lsn */
    uint32_t log_tot_len_;     /* 整个日志记录的长度 */
    txn_id_t log_tid_;         /* 创建当前日志的事务ID */
    lsn_t prev_lsn_;           /* 事务创建的前一条日志记录的lsn*/

    // 把日志记录序列化到dest中
    virtual void serialize (char* dest){
        memcpy(dest + OFFSET_LOG_TYPE, &log_type_, sizeof(LogType));
        memcpy(dest + OFFSET_LSN, &lsn_, sizeof(lsn_t));
        memcpy(dest + OFFSET_LOG_TOT_LEN, &log_tot_len_, sizeof(uint32_t));
        memcpy(dest + OFFSET_LOG_TID, &log_tid_, sizeof(txn_id_t));
        memcpy(dest + OFFSET_PREV_LSN, &prev_lsn_, sizeof(lsn_t));
    }
    // 从src中反序列化出一条日志记录
    virtual void deserialize(const char* src) {
        log_type_ = *reinterpret_cast<const LogType*>(src);
        lsn_ = *reinterpret_cast<const lsn_t*>(src + OFFSET_LSN);
        log_tot_len_ = *reinterpret_cast<const uint32_t*>(src + OFFSET_LOG_TOT_LEN);
        log_tid_ = *reinterpret_cast<const txn_id_t*>(src + OFFSET_LOG_TID);
        prev_lsn_ = *reinterpret_cast<const lsn_t*>(src + OFFSET_PREV_LSN);
    }
    // used for debug
    virtual void format_print() {
        std::cout << "log type in father_function: " << LogTypeStr[log_type_] << "\n";
        printf("Print Log Record:\n");
        printf("log_type_: %s\n", LogTypeStr[log_type_].c_str());
        printf("lsn: %d\n", lsn_);
        printf("log_tot_len: %d\n", log_tot_len_);
        printf("log_tid: %d\n", log_tid_);
        printf("prev_lsn: %d\n", prev_lsn_);
    }
};

//log file header info (for recovery)
class HeaderRecord: public LogRecord {
public:
    HeaderRecord(){
        log_type_ = LogType::HEADER;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }

    HeaderRecord(lsn_t global_lsn,lsn_t c_lsn,size_t c_cnt): HeaderRecord(){
        global_lsn_ = global_lsn;
        checkpoint_lsn_ = c_lsn;
        checkpoint_cnt_ = c_cnt;
        log_tot_len_+= sizeof(lsn_t)*2 + sizeof(size_t);
    }

    void serialize(char* dest) override {
        LogRecord::serialize(dest);
        int offset = LOG_HEADER_SIZE;
        memcpy(dest+offset,&global_lsn_,sizeof(lsn_t));
        offset += sizeof(global_lsn_);
        memcpy(dest+offset,&checkpoint_lsn_,sizeof(lsn_t));
        offset += sizeof(lsn_t);
        memcpy(dest+offset,&checkpoint_cnt_,sizeof(size_t));
    }

    // 从src中反序列化出一条Begin日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        int offset = LOG_HEADER_SIZE;
        memcpy(&global_lsn_,src+offset,sizeof(lsn_t));
        offset += sizeof(lsn_t);
        memcpy(&checkpoint_lsn_,src+offset,sizeof(lsn_t));
        offset += sizeof(lsn_t);
        memcpy(&checkpoint_cnt_,src+offset,sizeof(checkpoint_cnt_));
    }

    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[log_type_] << "\n";
        LogRecord::format_print();
    }

    lsn_t global_lsn_;
    lsn_t checkpoint_lsn_;
    size_t checkpoint_cnt_;
};

class CheckPointRecord: public LogRecord {
public:
    CheckPointRecord(){
        log_type_ = LogType::CHECKPOINT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }
    
    // 序列化Begin日志记录到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
    }

    // 从src中反序列化出一条Begin日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);   
    }

    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[log_type_] << "\n";
        LogRecord::format_print();
    }
};


class BeginLogRecord: public LogRecord {
public:
    BeginLogRecord() {
        log_type_ = LogType::BEGIN;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }

    BeginLogRecord(txn_id_t txn_id): BeginLogRecord(){
        log_tid_ = txn_id;
    }

    // 序列化Begin日志记录到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
    }

    // 从src中反序列化出一条Begin日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);   
    }
    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[log_type_] << "\n";
        LogRecord::format_print();
    }
};

/**
 * TODO: commit操作的日志记录
*/
class CommitLogRecord: public LogRecord {
public:
    CommitLogRecord() {
        log_type_ = LogType::COMMIT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }

    CommitLogRecord(txn_id_t txn_id) : CommitLogRecord() {
        log_tid_ = txn_id;
    }

    // 序列化Commit日志记录到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
    }

    // 从src中反序列化出一条Commit日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
    }

    void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[log_type_] << "\n";
        LogRecord::format_print();
    }
};

/**
 * TODO: abort操作的日志记录
*/
class AbortLogRecord: public LogRecord {
public:
       AbortLogRecord() {
        log_type_ = LogType::ABORT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }

    AbortLogRecord(txn_id_t txn_id) : AbortLogRecord() {
        log_tid_ = txn_id;
    }

    // 序列化Abort日志记录到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
    }

    // 从src中反序列化出一条Abort日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
    }

    void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[log_type_] << "\n";
        LogRecord::format_print();
    }
};

class InsertLogRecord: public LogRecord {
public:
    InsertLogRecord() {
        log_type_ = LogType::INSERT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }

    InsertLogRecord(txn_id_t txn_id, RmRecord& insert_value, Rid& rid, std::string table_name) 
        : InsertLogRecord() {
        log_tid_ = txn_id;
        insert_value_ = insert_value;
        rid_ = rid;
        log_tot_len_ +=  insert_value.getSize();
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.size();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }

    // 把insert日志记录序列化到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        offset += insert_value_.serialize(dest+offset);
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }

    // 从src中反序列化出一条Insert日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);  
        int offset = OFFSET_LOG_DATA;
        offset += insert_value_.desrialize(src+OFFSET_LOG_DATA);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_] ;
        memcpy(table_name_, src + offset, table_name_size_);
    }
    void format_print() override {
        printf("insert record\n");
        LogRecord::format_print();
        printf("insert_value: %s\n", insert_value_.data);
        printf("insert rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord insert_value_;     // 插入的记录
    Rid rid_;                   // 记录插入的位置
    char* table_name_;          // 插入记录的表名称
    size_t table_name_size_;    // 表名称的大小
    //std::unordered_set<Page*> page_set_;    // 插入记录时修改的页面
};

/**
 * TODO: delete操作的日志记录
*/
class DeleteLogRecord: public LogRecord {
public:
DeleteLogRecord() {
        log_type_ = LogType::DELETE;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }

    DeleteLogRecord(txn_id_t txn_id, RmRecord& delete_value, Rid& rid, std::string table_name) 
        : DeleteLogRecord() {
        log_tid_ = txn_id;
        delete_value_ = delete_value;
        rid_ = rid;
        log_tot_len_ += delete_value_.getSize();
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.size();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }

    // 把delete日志记录序列化到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        // memcpy(dest + offset, &delete_value_.size, sizeof(int));
        // offset += sizeof(int);
        // memcpy(dest + offset, delete_value_.data, delete_value_.size);
        // offset += delete_value_.size;
        offset += delete_value_.serialize(dest+offset);
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }

    // 从src中反序列化出一条Delete日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);  
        //delete_value_.Deserialize(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA;
        offset +=delete_value_.desrialize(src+offset);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, src + offset, table_name_size_);
    }

    void format_print() override {
        printf("delete record\n");
        LogRecord::format_print();
        printf("delete_value: %s\n", delete_value_.data);
        printf("delete rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord delete_value_;     // 删除的记录
    Rid rid_;                   // 记录删除的位置
    char* table_name_;          // 删除记录的表名称
    size_t table_name_size_;    // 表名称的大小

};

/**
 * TODO: update操作的日志记录
*/
class UpdateLogRecord: public LogRecord {
public:
      UpdateLogRecord() {
        log_type_ = LogType::UPDATE;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }

    UpdateLogRecord(txn_id_t txn_id, RmRecord& old_value, RmRecord& new_value, Rid& rid, std::string table_name) 
        : UpdateLogRecord() {
        log_tid_ = txn_id;
        old_value_ = old_value;
        new_value_ = new_value;
        rid_ = rid;
        //log_tot_len_ += sizeof(int) * 2;
        log_tot_len_+= old_value_.getSize() + new_value_.getSize();
        //log_tot_len_ += old_value_.size + new_value_.size;
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.size();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }

    // 把update日志记录序列化到dest中
    void serialize(char* dest) override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        // memcpy(dest + offset, &old_value_.size, sizeof(int));
        // offset += sizeof(int);
        // memcpy(dest + offset, old_value_.data, old_value_.size);
        // offset += old_value_.size;
        // memcpy(dest + offset, &new_value_.size, sizeof(int));
        // offset += sizeof(int);
        // memcpy(dest + offset, new_value_.data, new_value_.size);
        // offset += new_value_.size;
        offset += old_value_.serialize(dest+offset);
        offset += new_value_.serialize(dest+offset);
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }

    // 从src中反序列化出一条Update日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);  
        //old_value_.Deserialize(src + OFFSET_LOG_DATA);
        //int offset = OFFSET_LOG_DATA + old_value_.size + sizeof(int);
        //new_value_.Deserialize(src + offset);
        //offset += new_value_.size + sizeof(int);
        int offset = OFFSET_LOG_DATA;
        offset += old_value_.desrialize(src+offset);
        offset += new_value_.desrialize(src+offset);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, src + offset, table_name_size_);
    }

    void format_print() override {
        printf("update record\n");
        LogRecord::format_print();
        printf("old_value: %s\n", old_value_.data);
        printf("new_value: %s\n", new_value_.data);
        printf("update rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord old_value_;        // 更新前的记录
    RmRecord new_value_;        // 更新后的记录
    Rid rid_;                   // 记录更新的位置
    char* table_name_;          // 更新记录的表名称
    size_t table_name_size_;    // 表名称的大小

};



/* 日志缓冲区，只有一个buffer，因此需要阻塞地去把日志写入缓冲区中 */

class LogBuffer {
public:
    LogBuffer() { 
        offset_ = 0; 
        memset(buffer_, 0, sizeof(buffer_));
    }

    bool is_full(int append_size) {
        if(offset_ + append_size > LOG_BUFFER_SIZE)
            return true;
        return false;
    }

    bool clear() {
        offset_ = 0; 
        memset(buffer_, 0, sizeof(buffer_));
        return true;
    }

    char buffer_[LOG_BUFFER_SIZE+1];
    int offset_;    // 写入log的offset
};


/* 日志管理器，负责把日志写入日志缓冲区，以及把日志缓冲区中的内容写入磁盘中 */
class LogManager {
public:
    LogManager(DiskManager* disk_manager,BufferPoolManager* buf_mgr) { 
        disk_manager_ = disk_manager;
        buf_mgr_ = buf_mgr;
    }

    void recovery_log_info(){
        //恢复global_lsn_
        int hdr_sz = LOG_HEADER_SIZE+sizeof(lsn_t)*2+sizeof(size_t);
        char buf[hdr_sz];
        disk_manager_->read_log_header(buf,hdr_sz);
        HeaderRecord h_rec;
        h_rec.deserialize(buf);

        if(h_rec.log_type_ != LogType::HEADER){
            throw InternalError("log typw is not logheader");
        }
        global_lsn_.store(h_rec.global_lsn_);
    }
    
    lsn_t add_log_to_buffer(LogRecord* log_record);
    void flush_log_to_disk();

    BufferPoolManager* get_bp(){return buf_mgr_;}
    DiskManager* get_dm(){return disk_manager_;}
private:    
    std::atomic<lsn_t> global_lsn_{0};  // 全局lsn，递增，用于为每条记录分发lsn
             
    lsn_t flushed_to_disk_lsn;          // 记录已经持久化到磁盘中的最后一条日志的日志号(flushed_to_disk_lsn<=global_lsn_)
    
    std::mutex latch_;                  // 用于对log_buffer_的互斥访问
    LogBuffer log_buffer_;              // 日志缓冲区
    DiskManager* disk_manager_;
    
    lsn_t prev_lsn_ = 0;

    //TransactionManager* txn_mgr_;
    BufferPoolManager* buf_mgr_;
}; 
